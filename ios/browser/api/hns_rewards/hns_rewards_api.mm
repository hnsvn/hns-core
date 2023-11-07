/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#import "hns_rewards_api.h"
#import <UIKit/UIKit.h>

#include "base/base64.h"
#include "base/containers/flat_map.h"
#include "base/ios/ios_util.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/logging.h"
#include "base/strings/sys_string_conversions.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/threading/sequence_bound.h"
#include "base/time/time.h"
#include "base/types/cxx23_to_underlying.h"
#include "base/values.h"
#include "hns/build/ios/mojom/cpp_transformations.h"
#include "hns/components/hns_rewards/common/rewards_flags.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/rewards_database.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"
#import "hns/ios/browser/api/hns_rewards/promotion_solution.h"
#import "hns/ios/browser/api/hns_rewards/rewards.mojom.objc+private.h"
#import "hns/ios/browser/api/hns_rewards/rewards_client_bridge.h"
#import "hns/ios/browser/api/hns_rewards/rewards_client_ios.h"
#import "hns/ios/browser/api/hns_rewards/rewards_notification.h"
#import "hns/ios/browser/api/hns_rewards/rewards_observer.h"
#import "hns/ios/browser/api/hns_rewards/rewards_types.mojom.objc+private.h"
#import "hns/ios/browser/api/common/common_operations.h"
#include "components/os_crypt/sync/os_crypt.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "url/gurl.h"
#include "url/origin.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace hns_rewards::internal {

template <typename T>
struct task_deleter {
 private:
  scoped_refptr<base::TaskRunner> task_runner;

 public:
  task_deleter()
      : task_runner(base::SequencedTaskRunner::GetCurrentDefault()) {}
  ~task_deleter() = default;

  void operator()(T* ptr) const {
    if (base::SequencedTaskRunner::GetCurrentDefault() != task_runner) {
      task_runner->PostTask(FROM_HERE,
                            base::BindOnce([](T* ptr) { delete ptr; }, ptr));
    } else {
      delete ptr;
    }
  }
};

template <typename T, typename... Args>
std::unique_ptr<T, task_deleter<T>> make_task_ptr(Args&&... args) {
  return {new T(std::forward<Args>(args)...), task_deleter<T>()};
}

}  // namespace hns_rewards::internal

#define LLOG(verbose_level, format, ...)                  \
  [self log:(__FILE__)                                    \
       line:(__LINE__)verboseLevel:(verbose_level)message \
           :base::SysNSStringToUTF8(                      \
                [NSString stringWithFormat:(format), ##__VA_ARGS__])]

NSString* const HnsRewardsErrorDomain = @"HnsRewardsErrorDomain";
NSNotificationName const HnsRewardsNotificationAdded =
    @"BATHnsRewardsNotificationAdded";

HnsGeneralRewardsNotificationID const
    BATHnsGeneralRewardsNotificationIDWalletDisconnected =
        @"wallet_disconnected";

static NSString* const kContributionQueueAutoincrementID =
    @"BATContributionQueueAutoincrementID";
static NSString* const kUnblindedTokenAutoincrementID =
    @"BATUnblindedTokenAutoincrementID";

static NSString* const kExternalWalletsPrefKey = @"external_wallets";
static NSString* const kTransferFeesPrefKey = @"transfer_fees";

static const auto kOneDay =
    base::Time::kHoursPerDay * base::Time::kSecondsPerHour;

/// ---

@interface HnsRewardsAPI () <RewardsClientBridge> {
  // DO NOT ACCESS DIRECTLY, use `postEngineTask` or ensure you are accessing
  // _engine from a task posted in `_engineTaskRunner`
  std::unique_ptr<hns_rewards::internal::RewardsEngineImpl,
                  hns_rewards::internal::task_deleter<
                      hns_rewards::internal::RewardsEngineImpl>>
      _engine;
  std::unique_ptr<RewardsClientIOS,
                  hns_rewards::internal::task_deleter<RewardsClientIOS>>
      _rewardsClient;
  base::SequenceBound<hns_rewards::internal::RewardsDatabase> rewardsDatabase;
  scoped_refptr<base::SequencedTaskRunner> databaseQueue;
  scoped_refptr<base::SequencedTaskRunner> _engineTaskRunner;
}

@property(nonatomic, copy) NSString* storagePath;
@property(nonatomic) HnsRewardsRewardsParameters* rewardsParameters;
@property(nonatomic) HnsRewardsBalance* balance;
@property(nonatomic) dispatch_queue_t fileWriteThread;
@property(nonatomic) NSMutableDictionary<NSString*, NSString*>* state;
@property(nonatomic) HnsCommonOperations* commonOps;
@property(nonatomic) NSMutableDictionary<NSString*, __kindof NSObject*>* prefs;

@property(nonatomic) NSMutableArray<HnsRewardsPromotion*>* mPendingPromotions;
@property(nonatomic)
    NSMutableArray<HnsRewardsPromotion*>* mFinishedPromotions;

@property(nonatomic) NSHashTable<RewardsObserver*>* observers;

@property(nonatomic, getter=isInitialized) BOOL initialized;
@property(nonatomic) BOOL initializing;
@property(nonatomic) BOOL dataMigrationFailed;
@property(nonatomic) HnsRewardsResult initializationResult;
@property(nonatomic, getter=isLoadingPublisherList) BOOL loadingPublisherList;
@property(nonatomic, getter=isInitializingWallet) BOOL initializingWallet;

/// Notifications

@property(nonatomic) NSTimer* notificationStartupTimer;
@property(nonatomic) NSDate* lastNotificationCheckDate;

/// Temporary blocks

@end

@implementation HnsRewardsAPI

- (instancetype)initWithStateStoragePath:(NSString*)path {
  if ((self = [super init])) {
    _engineTaskRunner = base::ThreadPool::CreateSingleThreadTaskRunner(
        {base::MayBlock(), base::WithBaseSyncPrimitives(),
         base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN},
        base::SingleThreadTaskRunnerThreadMode::DEDICATED);

    self.storagePath = path;
    self.commonOps =
        [[HnsCommonOperations alloc] initWithStoragePath:path
                                                taskRunner:_engineTaskRunner];
    self.state = [[NSMutableDictionary alloc]
                     initWithContentsOfFile:self.randomStatePath]
                     ?: [[NSMutableDictionary alloc] init];
    self.fileWriteThread =
        dispatch_queue_create("com.rewards.file-write", DISPATCH_QUEUE_SERIAL);
    self.mPendingPromotions = [[NSMutableArray alloc] init];
    self.mFinishedPromotions = [[NSMutableArray alloc] init];
    self.observers = [NSHashTable weakObjectsHashTable];

    self.prefs =
        [[NSMutableDictionary alloc] initWithContentsOfFile:[self prefsPath]];
    if (!self.prefs) {
      self.prefs = [[NSMutableDictionary alloc] init];
    }

    NSString* walletProviderRegionsKey = @"parameters.wallet_provider_regions";
    if (!self.prefs[walletProviderRegionsKey] ||
        ![self.prefs[walletProviderRegionsKey] isKindOfClass:NSString.class]) {
      self.prefs[walletProviderRegionsKey] = @"{}";
    }

    [self handleFlags:hns_rewards::RewardsFlags::ForCurrentProcess()];

    databaseQueue = base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});

    const auto* dbPath = [self rewardsDatabasePath].UTF8String;

    rewardsDatabase =
        base::SequenceBound<hns_rewards::internal::RewardsDatabase>(
            databaseQueue, base::FilePath(dbPath));

    _engineTaskRunner->PostTask(
        FROM_HERE, base::BindOnce(^{
          self->_rewardsClient =
              hns_rewards::internal::make_task_ptr<RewardsClientIOS>(self);
          self->_engine = hns_rewards::internal::make_task_ptr<
              hns_rewards::internal::RewardsEngineImpl>(
              self->_rewardsClient->MakeRemote());
        }));

    // Add notifications for standard app foreground/background
    [NSNotificationCenter.defaultCenter
        addObserver:self
           selector:@selector(applicationDidBecomeActive)
               name:UIApplicationDidBecomeActiveNotification
             object:nil];
    [NSNotificationCenter.defaultCenter
        addObserver:self
           selector:@selector(applicationDidBackground)
               name:UIApplicationDidEnterBackgroundNotification
             object:nil];
  }
  return self;
}

- (void)dealloc {
  [NSNotificationCenter.defaultCenter removeObserver:self];
  [self.notificationStartupTimer invalidate];
}

- (void)handleFlags:(const hns_rewards::RewardsFlags&)flags {
  if (flags.environment) {
    switch (*flags.environment) {
      case hns_rewards::RewardsFlags::Environment::kDevelopment:
        hns_rewards::internal::_environment =
            hns_rewards::mojom::Environment::DEVELOPMENT;
        break;
      case hns_rewards::RewardsFlags::Environment::kStaging:
        hns_rewards::internal::_environment =
            hns_rewards::mojom::Environment::STAGING;
        break;
      case hns_rewards::RewardsFlags::Environment::kProduction:
        hns_rewards::internal::_environment =
            hns_rewards::mojom::Environment::PRODUCTION;
        break;
    }
  }

  if (flags.debug) {
    hns_rewards::internal::is_debug = true;
  }

  if (flags.reconcile_interval) {
    hns_rewards::internal::reconcile_interval = *flags.reconcile_interval;
  }

  if (flags.retry_interval) {
    hns_rewards::internal::retry_interval = *flags.retry_interval;
  }
}

- (void)postEngineTask:
    (void (^)(hns_rewards::internal::RewardsEngineImpl*))task {
  _engineTaskRunner->PostTask(FROM_HERE, base::BindOnce(^{
                                CHECK(self->_engine != nullptr);
                                task(self->_engine.get());
                              }));
}

- (void)initializeRewardsService:(nullable void (^)())completion {
  if (self.initialized || self.initializing) {
    return;
  }
  self.initializing = YES;

  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->Initialize(base::BindOnce(^(hns_rewards::mojom::Result result) {
      self.initialized =
          (result == hns_rewards::mojom::Result::OK ||
           result == hns_rewards::mojom::Result::NO_LEGACY_STATE ||
           result == hns_rewards::mojom::Result::NO_PUBLISHER_STATE);
      self.initializing = NO;
      if (self.initialized) {
        [self getRewardsParameters:nil];
        [self fetchBalance:nil];
      } else {
        LLOG(0, @"Rewards Initialization Failed with error: %d",
             base::to_underlying(result));
      }
      self.initializationResult = static_cast<HnsRewardsResult>(result);
      if (completion) {
        dispatch_async(dispatch_get_main_queue(), ^{
          completion();
        });
      }
      dispatch_async(dispatch_get_main_queue(), ^{
        for (RewardsObserver* observer in [self.observers copy]) {
          if (observer.walletInitalized) {
            observer.walletInitalized(self.initializationResult);
          }
        }
      });
    }));
  }];
}

- (NSString*)rewardsDatabasePath {
  return [self.storagePath stringByAppendingPathComponent:@"Rewards.db"];
}

- (void)resetRewardsDatabase {
  const auto dbPath = [self rewardsDatabasePath];
  [NSFileManager.defaultManager removeItemAtPath:dbPath error:nil];
  [NSFileManager.defaultManager
      removeItemAtPath:[dbPath stringByAppendingString:@"-journal"]
                 error:nil];
  rewardsDatabase =
      base::SequenceBound<hns_rewards::internal::RewardsDatabase>(
          databaseQueue, base::FilePath(base::SysNSStringToUTF8(dbPath)));
}

- (NSString*)randomStatePath {
  return
      [self.storagePath stringByAppendingPathComponent:@"random_state.plist"];
}

- (NSString*)prefsPath {
  return [self.storagePath stringByAppendingPathComponent:@"ledger_pref.plist"];
}

- (void)savePrefs {
  NSDictionary* prefs = [self.prefs copy];
  NSString* path = [[self prefsPath] copy];
  dispatch_async(self.fileWriteThread, ^{
    [prefs writeToURL:[NSURL fileURLWithPath:path isDirectory:NO] error:nil];
  });
}

#pragma mark - Observers

- (void)addObserver:(RewardsObserver*)observer {
  [self.observers addObject:observer];
}

- (void)removeObserver:(RewardsObserver*)observer {
  [self.observers removeObject:observer];
}

#pragma mark - Wallet

- (void)createWallet:(void (^)(NSError* _Nullable))completion {
  const auto __weak weakSelf = self;
  // Results that can come from CreateRewardsWallet():
  //   - OK: Good to go
  //   - ERROR: Already initialized
  //   - BAD_REGISTRATION_RESPONSE: Request credentials call failure or
  //   malformed data
  //   - REGISTRATION_VERIFICATION_FAILED: Missing master user token
  self.initializingWallet = YES;
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->CreateRewardsWallet(
        "", base::BindOnce(^(
                hns_rewards::mojom::CreateRewardsWalletResult create_result) {
          const auto strongSelf = weakSelf;
          if (!strongSelf) {
            return;
          }

          hns_rewards::mojom::Result result =
              create_result ==
                      hns_rewards::mojom::CreateRewardsWalletResult::kSuccess
                  ? hns_rewards::mojom::Result::OK
                  : hns_rewards::mojom::Result::FAILED;

          NSError* error = nil;
          if (result != hns_rewards::mojom::Result::OK) {
            std::map<hns_rewards::mojom::Result, std::string>
                errorDescriptions{
                    {hns_rewards::mojom::Result::FAILED,
                     "The wallet was already initialized"},
                    {hns_rewards::mojom::Result::BAD_REGISTRATION_RESPONSE,
                     "Request credentials call failure or malformed data"},
                    {hns_rewards::mojom::Result::
                         REGISTRATION_VERIFICATION_FAILED,
                     "Missing master user token from registered persona"},
                };
            NSDictionary* userInfo = @{};
            const auto description =
                errorDescriptions[static_cast<hns_rewards::mojom::Result>(
                    result)];
            if (description.length() > 0) {
              userInfo = @{
                NSLocalizedDescriptionKey : base::SysUTF8ToNSString(description)
              };
            }
            error = [NSError errorWithDomain:HnsRewardsErrorDomain
                                        code:static_cast<NSInteger>(result)
                                    userInfo:userInfo];
          }

          [strongSelf startNotificationTimers];
          strongSelf.initializingWallet = NO;

          dispatch_async(dispatch_get_main_queue(), ^{
            if (completion) {
              completion(error);
            }

            for (RewardsObserver* observer in [strongSelf.observers copy]) {
              if (observer.walletInitalized) {
                observer.walletInitalized(
                    static_cast<HnsRewardsResult>(result));
              }
            }
          });
        }));
  }];
}

- (void)currentWalletInfo:
    (void (^)(HnsRewardsRewardsWallet* _Nullable wallet))completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetRewardsWallet(
        base::BindOnce(^(hns_rewards::mojom::RewardsWalletPtr wallet) {
          const auto bridgedWallet = wallet.get() != nullptr
                                         ? [[HnsRewardsRewardsWallet alloc]
                                               initWithRewardsWallet:*wallet]
                                         : nil;
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(bridgedWallet);
          });
        }));
  }];
}

- (void)getRewardsParameters:
    (void (^)(HnsRewardsRewardsParameters* _Nullable))completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetRewardsParameters(
        base::BindOnce(^(hns_rewards::mojom::RewardsParametersPtr info) {
          if (info) {
            self.rewardsParameters = [[HnsRewardsRewardsParameters alloc]
                initWithRewardsParametersPtr:std::move(info)];
          } else {
            self.rewardsParameters = nil;
          }
          const auto __weak weakSelf = self;
          dispatch_async(dispatch_get_main_queue(), ^{
            if (completion) {
              completion(weakSelf.rewardsParameters);
            }
          });
        }));
  }];
}

- (void)fetchBalance:(void (^)(HnsRewardsBalance* _Nullable))completion {
  const auto __weak weakSelf = self;
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->FetchBalance(base::BindOnce(
        ^(base::expected<hns_rewards::mojom::BalancePtr,
                         hns_rewards::mojom::FetchBalanceError> result) {
          const auto strongSelf = weakSelf;
          if (result.has_value()) {
            strongSelf.balance = [[HnsRewardsBalance alloc]
                initWithBalancePtr:std::move(result.value())];
          }
          dispatch_async(dispatch_get_main_queue(), ^{
            if (completion) {
              completion(strongSelf.balance);
            }
          });
        }));
  }];
}

- (void)legacyWallet:
    (hns_rewards::mojom::RewardsEngineClient::GetLegacyWalletCallback)
        callback {
  NSDictionary* externalWallets =
      self.prefs[kExternalWalletsPrefKey] ?: [[NSDictionary alloc] init];
  std::string wallet;
  NSData* data = [NSJSONSerialization dataWithJSONObject:externalWallets
                                                 options:0
                                                   error:nil];
  if (data != nil) {
    NSString* dataString = [[NSString alloc] initWithData:data
                                                 encoding:NSUTF8StringEncoding];
    if (dataString != nil) {
      wallet = base::SysNSStringToUTF8(dataString);
    }
  }
  std::move(callback).Run(wallet);
}

#pragma mark - Publishers

- (void)listActivityInfoFromStart:(unsigned int)start
                            limit:(unsigned int)limit
                           filter:(HnsRewardsActivityInfoFilter*)filter
                       completion:
                           (void (^)(NSArray<HnsRewardsPublisherInfo*>*))
                               completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    auto cppFilter = filter ? filter.cppObjPtr
                            : hns_rewards::mojom::ActivityInfoFilter::New();
    if (filter.excluded == HnsRewardsExcludeFilterFilterExcluded) {
      engine->GetExcludedList(base::BindOnce(
          ^(std::vector<hns_rewards::mojom::PublisherInfoPtr> list) {
            const auto publishers = NSArrayFromVector(
                &list, ^HnsRewardsPublisherInfo*(
                    const hns_rewards::mojom::PublisherInfoPtr& info) {
                  return [[HnsRewardsPublisherInfo alloc]
                      initWithPublisherInfo:*info];
                });
            dispatch_async(dispatch_get_main_queue(), ^{
              completion(publishers);
            });
          }));
    } else {
      engine->GetActivityInfoList(
          start, limit, std::move(cppFilter),
          base::BindOnce(
              ^(std::vector<hns_rewards::mojom::PublisherInfoPtr> list) {
                const auto publishers = NSArrayFromVector(
                    &list, ^HnsRewardsPublisherInfo*(
                        const hns_rewards::mojom::PublisherInfoPtr& info) {
                      return [[HnsRewardsPublisherInfo alloc]
                          initWithPublisherInfo:*info];
                    });
                dispatch_async(dispatch_get_main_queue(), ^{
                  completion(publishers);
                });
              }));
    }
  }];
}

- (void)fetchPublisherActivityFromURL:(NSURL*)URL
                           faviconURL:(nullable NSURL*)faviconURL
                        publisherBlob:(nullable NSString*)publisherBlob
                                tabId:(uint64_t)tabId {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    if (!URL.absoluteString) {
      return;
    }

    GURL parsedUrl(base::SysNSStringToUTF8(URL.absoluteString));

    if (!parsedUrl.is_valid()) {
      return;
    }

    url::Origin origin = url::Origin::Create(parsedUrl);
    std::string baseDomain = GetDomainAndRegistry(
        origin.host(),
        net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);

    if (baseDomain == "") {
      return;
    }

    hns_rewards::mojom::VisitDataPtr visitData =
        hns_rewards::mojom::VisitData::New();
    visitData->domain = visitData->name = baseDomain;
    visitData->path = parsedUrl.PathForRequest();
    visitData->url = origin.Serialize();

    if (faviconURL.absoluteString) {
      visitData->favicon_url =
          base::SysNSStringToUTF8(faviconURL.absoluteString);
    }

    std::string blob = std::string();
    if (publisherBlob) {
      blob = base::SysNSStringToUTF8(publisherBlob);
    }

    engine->GetPublisherActivityFromUrl(tabId, std::move(visitData), blob);
  }];
}

- (void)refreshPublisherWithId:(NSString*)publisherId
                    completion:(void (^)(HnsRewardsPublisherStatus status))
                                   completion {
  if (self.loadingPublisherList) {
    completion(HnsRewardsPublisherStatusNotVerified);
    return;
  }
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->RefreshPublisher(
        base::SysNSStringToUTF8(publisherId),
        base::BindOnce(^(hns_rewards::mojom::PublisherStatus status) {
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(static_cast<HnsRewardsPublisherStatus>(status));
          });
        }));
  }];
}

#pragma mark - Tips

- (void)listRecurringTips:
    (void (^)(NSArray<HnsRewardsPublisherInfo*>*))completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetRecurringTips(base::BindOnce(
        ^(std::vector<hns_rewards::mojom::PublisherInfoPtr> list) {
          const auto publishers = NSArrayFromVector(
              &list, ^HnsRewardsPublisherInfo*(
                  const hns_rewards::mojom::PublisherInfoPtr& info) {
                return [[HnsRewardsPublisherInfo alloc]
                    initWithPublisherInfo:*info];
              });
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(publishers);
          });
        }));
  }];
}

- (void)removeRecurringTipForPublisherWithId:(NSString*)publisherId {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->RemoveRecurringTip(
        base::SysNSStringToUTF8(publisherId),
        base::BindOnce(^(hns_rewards::mojom::Result result){
            // Not Used
        }));
  }];
}

#pragma mark - Grants

- (NSArray<HnsRewardsPromotion*>*)pendingPromotions {
  return [self.mPendingPromotions copy];
}

- (NSArray<HnsRewardsPromotion*>*)finishedPromotions {
  return [self.mFinishedPromotions copy];
}

- (NSString*)notificationIDForPromo:
    (const hns_rewards::mojom::PromotionPtr)promo {
  bool isUGP = promo->type == hns_rewards::mojom::PromotionType::UGP;
  const auto prefix = isUGP ? @"rewards_grant_" : @"rewards_grant_ads_";
  const auto promotionId = base::SysUTF8ToNSString(promo->id);
  return [NSString stringWithFormat:@"%@%@", prefix, promotionId];
}

- (void)updatePendingAndFinishedPromotions:(void (^)())completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetAllPromotions(base::BindOnce(^(
        base::flat_map<std::string, hns_rewards::mojom::PromotionPtr> map) {
      NSMutableArray* promos = [[NSMutableArray alloc] init];
      for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->second.get() != nullptr) {
          [promos addObject:[[HnsRewardsPromotion alloc]
                                initWithPromotion:*it->second]];
        }
      }
      [self.mFinishedPromotions removeAllObjects];
      [self.mPendingPromotions removeAllObjects];
      for (HnsRewardsPromotion* promotion in promos) {
        if (promotion.status == HnsRewardsPromotionStatusFinished) {
          [self.mFinishedPromotions addObject:promotion];
        } else if (promotion.status == HnsRewardsPromotionStatusActive ||
                   promotion.status == HnsRewardsPromotionStatusAttested) {
          [self.mPendingPromotions addObject:promotion];
        }
      }
      dispatch_async(dispatch_get_main_queue(), ^{
        if (completion) {
          completion();
        }
        for (RewardsObserver* observer in [self.observers copy]) {
          if (observer.promotionsAdded) {
            observer.promotionsAdded(self.pendingPromotions);
          }
          if (observer.finishedPromotionsAdded) {
            observer.finishedPromotionsAdded(self.finishedPromotions);
          }
        }
      });
    }));
  }];
}

- (void)fetchPromotions:
    (nullable void (^)(NSArray<HnsRewardsPromotion*>* grants))completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->FetchPromotions(base::BindOnce(
        ^(hns_rewards::mojom::Result result,
          std::vector<hns_rewards::mojom::PromotionPtr> promotions) {
          if (result != hns_rewards::mojom::Result::OK) {
            return;
          }
          [self updatePendingAndFinishedPromotions:^{
            if (completion) {
              dispatch_async(dispatch_get_main_queue(), ^{
                completion(self.pendingPromotions);
              });
            }
          }];
        }));
  }];
}

- (void)claimPromotion:(NSString*)promotionId
             publicKey:(NSString*)deviceCheckPublicKey
            completion:(void (^)(HnsRewardsResult result,
                                 NSString* _Nonnull nonce))completion {
  const auto payload = [NSDictionary dictionaryWithObject:deviceCheckPublicKey
                                                   forKey:@"publicKey"];
  const auto jsonData = [NSJSONSerialization dataWithJSONObject:payload
                                                        options:0
                                                          error:nil];
  if (!jsonData) {
    LLOG(0, @"Missing JSON payload while attempting to claim promotion");
    return;
  }
  const auto jsonString = [[NSString alloc] initWithData:jsonData
                                                encoding:NSUTF8StringEncoding];
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->ClaimPromotion(
        base::SysNSStringToUTF8(promotionId),
        base::SysNSStringToUTF8(jsonString),
        base::BindOnce(^(hns_rewards::mojom::Result result,
                         const std::string& nonce) {
          const auto bridgedNonce = base::SysUTF8ToNSString(nonce);
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(static_cast<HnsRewardsResult>(result), bridgedNonce);
          });
        }));
  }];
}

- (void)attestPromotion:(NSString*)promotionId
               solution:(PromotionSolution*)solution
             completion:(void (^)(HnsRewardsResult result,
                                  HnsRewardsPromotion* _Nullable promotion))
                            completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->AttestPromotion(
        base::SysNSStringToUTF8(promotionId),
        base::SysNSStringToUTF8(solution.JSONPayload),
        base::BindOnce(^(hns_rewards::mojom::Result result,
                         hns_rewards::mojom::PromotionPtr promotion) {
          if (promotion.get() == nullptr) {
            if (completion) {
              dispatch_async(dispatch_get_main_queue(), ^{
                completion(static_cast<HnsRewardsResult>(result), nil);
              });
            }
            return;
          }

          const auto bridgedPromotion =
              [[HnsRewardsPromotion alloc] initWithPromotion:*promotion];
          if (result == hns_rewards::mojom::Result::OK) {
            [self fetchBalance:nil];
          }

          dispatch_async(dispatch_get_main_queue(), ^{
            if (completion) {
              completion(static_cast<HnsRewardsResult>(result),
                         bridgedPromotion);
            }
            if (result == hns_rewards::mojom::Result::OK) {
              for (RewardsObserver* observer in [self.observers copy]) {
                if (observer.promotionClaimed) {
                  observer.promotionClaimed(bridgedPromotion);
                }
              }
            }
          });
        }));
  }];
}

#pragma mark - Reconcile

- (void)onReconcileComplete:(hns_rewards::mojom::Result)result
               contribution:
                   (hns_rewards::mojom::ContributionInfoPtr)contribution {
  // TODO we changed from probi to amount, so from string to double
  if (result == hns_rewards::mojom::Result::OK) {
    [self fetchBalance:nil];
  }
}

#pragma mark - Misc

- (void)rewardsInternalInfo:
    (void (^)(HnsRewardsRewardsInternalsInfo* _Nullable info))completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetRewardsInternalsInfo(
        base::BindOnce(^(hns_rewards::mojom::RewardsInternalsInfoPtr info) {
          auto bridgedInfo = info.get() != nullptr
                                 ? [[HnsRewardsRewardsInternalsInfo alloc]
                                       initWithRewardsInternalsInfo:*info.get()]
                                 : nil;
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(bridgedInfo);
          });
        }));
  }];
}

- (void)allContributions:
    (void (^)(NSArray<HnsRewardsContributionInfo*>* contributions))
        completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetAllContributions(base::BindOnce(
        ^(std::vector<hns_rewards::mojom::ContributionInfoPtr> list) {
          const auto convetedList = NSArrayFromVector(
              &list, ^HnsRewardsContributionInfo*(
                  const hns_rewards::mojom::ContributionInfoPtr& info) {
                return [[HnsRewardsContributionInfo alloc]
                    initWithContributionInfo:*info];
              });
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(convetedList);
          });
        }));
  }];
}

- (void)fetchAutoContributeProperties:
    (void (^)(HnsRewardsAutoContributeProperties* _Nullable properties))
        completion {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->GetAutoContributeProperties(base::BindOnce(
        ^(hns_rewards::mojom::AutoContributePropertiesPtr props) {
          auto properties =
              props.get() != nullptr
                  ? [[HnsRewardsAutoContributeProperties alloc]
                        initWithAutoContributePropertiesPtr:std::move(props)]
                  : nil;
          dispatch_async(dispatch_get_main_queue(), ^{
            completion(properties);
          });
        }));
  }];
}

#pragma mark - Reporting

- (void)setSelectedTabId:(UInt32)selectedTabId {
  if (!self.initialized) {
    return;
  }

  const auto time = [[NSDate date] timeIntervalSince1970];
  if (_selectedTabId != selectedTabId) {
    const auto oldTabId = _selectedTabId;
    [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
      engine->OnHide(oldTabId, time);
    }];
  }
  _selectedTabId = selectedTabId;
  if (_selectedTabId > 0) {
    [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
      engine->OnShow(selectedTabId, time);
    }];
  }
}

- (void)applicationDidBecomeActive {
  if (!self.initialized) {
    return;
  }

  const auto time = [[NSDate date] timeIntervalSince1970];
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->OnForeground(self.selectedTabId, time);
  }];

  // Check if the last notification check was more than a day ago
  if (fabs([self.lastNotificationCheckDate timeIntervalSinceNow]) > kOneDay) {
    [self checkForNotificationsAndFetchGrants];
  }
}

- (void)applicationDidBackground {
  if (!self.initialized) {
    return;
  }

  const auto time = [[NSDate date] timeIntervalSince1970];
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->OnBackground(self.selectedTabId, time);
  }];
}

- (void)reportLoadedPageWithURL:(NSURL*)url tabId:(UInt32)tabId {
  if (!self.initialized) {
    return;
  }

  const auto time = [[NSDate date] timeIntervalSince1970];
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    GURL parsedUrl(base::SysNSStringToUTF8(url.absoluteString));
    url::Origin origin = url::Origin::Create(parsedUrl);
    const std::string baseDomain = GetDomainAndRegistry(
        origin.host(),
        net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);

    if (baseDomain == "") {
      return;
    }

    const std::string publisher_url =
        origin.scheme() + "://" + baseDomain + "/";

    hns_rewards::mojom::VisitDataPtr data =
        hns_rewards::mojom::VisitData::New();
    data->name = baseDomain;
    data->domain = origin.host();
    data->path = parsedUrl.path();
    data->tab_id = tabId;
    data->url = publisher_url;

    engine->OnLoad(std::move(data), time);
  }];
}

- (void)reportXHRLoad:(NSURL*)url
                tabId:(UInt32)tabId
        firstPartyURL:(NSURL*)firstPartyURL
          referrerURL:(NSURL*)referrerURL {
  if (!self.initialized) {
    return;
  }

  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    base::flat_map<std::string, std::string> partsMap;
    const auto urlComponents = [[NSURLComponents alloc] initWithURL:url
                                            resolvingAgainstBaseURL:NO];
    for (NSURLQueryItem* item in urlComponents.queryItems) {
      std::string value =
          item.value != nil ? base::SysNSStringToUTF8(item.value) : "";
      partsMap[base::SysNSStringToUTF8(item.name)] = value;
    }

    auto visit = hns_rewards::mojom::VisitData::New();
    visit->path = base::SysNSStringToUTF8(url.absoluteString);
    visit->tab_id = tabId;

    std::string ref = referrerURL != nil
                          ? base::SysNSStringToUTF8(referrerURL.absoluteString)
                          : "";
    std::string fpu =
        firstPartyURL != nil
            ? base::SysNSStringToUTF8(firstPartyURL.absoluteString)
            : "";

    engine->OnXHRLoad(tabId, base::SysNSStringToUTF8(url.absoluteString),
                      partsMap, fpu, ref, std::move(visit));
  }];
}

- (void)reportTabNavigationOrClosedWithTabId:(UInt32)tabId {
  if (!self.initialized) {
    return;
  }

  const auto time = [[NSDate date] timeIntervalSince1970];
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->OnUnload(tabId, time);
  }];
}

#pragma mark - Preferences

- (void)setMinimumVisitDuration:(int)minimumVisitDuration {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->SetPublisherMinVisitTime(minimumVisitDuration);
  }];
}

- (void)setMinimumNumberOfVisits:(int)minimumNumberOfVisits {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->SetPublisherMinVisits(minimumNumberOfVisits);
  }];
}

- (void)setContributionAmount:(double)contributionAmount {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->SetAutoContributionAmount(contributionAmount);
  }];
}

- (void)setAutoContributeEnabled:(bool)autoContributeEnabled {
  [self postEngineTask:^(hns_rewards::internal::RewardsEngineImpl* engine) {
    engine->SetAutoContributeEnabled(autoContributeEnabled);
  }];
}

- (void)setBooleanState:(const std::string&)name
                  value:(bool)value
               callback:(hns_rewards::mojom::RewardsEngineClient::
                             SetBooleanStateCallback)callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = [NSNumber numberWithBool:value];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)
    booleanState:(const std::string&)name
        callback:
            (hns_rewards::mojom::RewardsEngineClient::GetBooleanStateCallback)
                callback {
  const auto key = base::SysUTF8ToNSString(name);
  if (![self.prefs objectForKey:key]) {
    std::move(callback).Run(false);
    return;
  }
  std::move(callback).Run([self.prefs[key] boolValue]);
}

- (void)setIntegerState:(const std::string&)name
                  value:(int32_t)value
               callback:(hns_rewards::mojom::RewardsEngineClient::
                             SetIntegerStateCallback)callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = [NSNumber numberWithInt:value];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)
    integerState:(const std::string&)name
        callback:
            (hns_rewards::mojom::RewardsEngineClient::GetIntegerStateCallback)
                callback {
  const auto key = base::SysUTF8ToNSString(name);
  std::move(callback).Run([self.prefs[key] intValue]);
}

- (void)setDoubleState:(const std::string&)name
                 value:(double)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetDoubleStateCallback)callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = [NSNumber numberWithDouble:value];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)
    doubleState:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetDoubleStateCallback)
               callback {
  const auto key = base::SysUTF8ToNSString(name);
  std::move(callback).Run([self.prefs[key] doubleValue]);
}

- (void)setStringState:(const std::string&)name
                 value:(const std::string&)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetStringStateCallback)callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = base::SysUTF8ToNSString(value);
  [self savePrefs];
  std::move(callback).Run();
}

- (void)
    stringState:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetStringStateCallback)
               callback {
  const auto key = base::SysUTF8ToNSString(name);
  const auto value = (NSString*)self.prefs[key];
  if (!value) {
    std::move(callback).Run("");
    return;
  }
  std::move(callback).Run(base::SysNSStringToUTF8(value));
}

- (void)
    setInt64State:(const std::string&)name
            value:(int64_t)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::SetInt64StateCallback)
                 callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = [NSNumber numberWithLongLong:value];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)int64State:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::GetInt64StateCallback)
                  callback {
  const auto key = base::SysUTF8ToNSString(name);
  std::move(callback).Run([self.prefs[key] longLongValue]);
}

- (void)setUint64State:(const std::string&)name
                 value:(uint64_t)value
              callback:(hns_rewards::mojom::RewardsEngineClient::
                            SetUint64StateCallback)callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = [NSNumber numberWithUnsignedLongLong:value];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)
    uint64State:(const std::string&)name
       callback:
           (hns_rewards::mojom::RewardsEngineClient::GetUint64StateCallback)
               callback {
  const auto key = base::SysUTF8ToNSString(name);
  std::move(callback).Run([self.prefs[key] unsignedLongLongValue]);
}

- (void)
    setValueState:(const std::string&)name
            value:(base::Value)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::SetValueStateCallback)
                 callback {
  std::string json;
  if (base::JSONWriter::Write(value, &json)) {
    const auto key = base::SysUTF8ToNSString(name);
    self.prefs[key] = base::SysUTF8ToNSString(json);
    [self savePrefs];
  }
  std::move(callback).Run();
}

- (void)valueState:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::GetValueStateCallback)
                  callback {
  const auto key = base::SysUTF8ToNSString(name);
  const auto json = (NSString*)self.prefs[key];
  if (!json) {
    std::move(callback).Run(base::Value());
    return;
  }

  auto value = base::JSONReader::Read(base::SysNSStringToUTF8(json));
  if (!value) {
    std::move(callback).Run(base::Value());
    return;
  }

  std::move(callback).Run(std::move(*value));
}

- (void)
    setTimeState:(const std::string&)name
           value:(base::Time)value
        callback:
            (hns_rewards::mojom::RewardsEngineClient::SetTimeStateCallback)
                callback {
  const auto key = base::SysUTF8ToNSString(name);
  self.prefs[key] = @(value.ToDoubleT());
  [self savePrefs];
  std::move(callback).Run();
}

- (void)timeState:(const std::string&)name
         callback:
             (hns_rewards::mojom::RewardsEngineClient::GetTimeStateCallback)
                 callback {
  const auto key = base::SysUTF8ToNSString(name);
  std::move(callback).Run(
      base::Time::FromDoubleT([self.prefs[key] doubleValue]));
}

- (void)clearState:(const std::string&)name
          callback:
              (hns_rewards::mojom::RewardsEngineClient::ClearStateCallback)
                  callback {
  const auto key = base::SysUTF8ToNSString(name);
  [self.prefs removeObjectForKey:key];
  [self savePrefs];
  std::move(callback).Run();
}

- (void)getClientCountryCode:
    (hns_rewards::mojom::RewardsEngineClient::GetClientCountryCodeCallback)
        callback {
  std::move(callback).Run("");
}

- (void)isAutoContributeSupportedForClient:
    (hns_rewards::mojom::RewardsEngineClient::
         IsAutoContributeSupportedForClientCallback)callback {
  std::move(callback).Run(true);
}

#pragma mark - Notifications

- (void)clearAllNotifications {
  // Not used on iOS
}

- (void)startNotificationTimers {
  dispatch_async(dispatch_get_main_queue(), ^{
    // Startup timer, begins after 30-second delay.
    self.notificationStartupTimer =
        [NSTimer scheduledTimerWithTimeInterval:30
                                         target:self
                                       selector:@selector
                                       (checkForNotificationsAndFetchGrants)
                                       userInfo:nil
                                        repeats:NO];
  });
}

- (void)checkForNotificationsAndFetchGrants {
  self.lastNotificationCheckDate = [NSDate date];

  [self fetchPromotions:nil];
}

#pragma mark - State

- (void)loadLegacyState:
    (hns_rewards::mojom::RewardsEngineClient::LoadLegacyStateCallback)
        callback {
  const auto contents =
      [self.commonOps loadContentsFromFileWithName:"ledger_state.json"];
  if (contents.length() > 0) {
    std::move(callback).Run(hns_rewards::mojom::Result::OK, contents);
  } else {
    std::move(callback).Run(hns_rewards::mojom::Result::NO_LEGACY_STATE,
                            contents);
  }
  [self startNotificationTimers];
}

- (void)loadPublisherState:
    (hns_rewards::mojom::RewardsEngineClient::LoadPublisherStateCallback)
        callback {
  const auto contents =
      [self.commonOps loadContentsFromFileWithName:"publisher_state.json"];
  if (contents.length() > 0) {
    std::move(callback).Run(hns_rewards::mojom::Result::OK, contents);
  } else {
    std::move(callback).Run(hns_rewards::mojom::Result::NO_PUBLISHER_STATE,
                            contents);
  }
}

#pragma mark - Network

- (NSString*)customUserAgent {
  return self.commonOps.customUserAgent;
}

- (void)setCustomUserAgent:(NSString*)customUserAgent {
  self.commonOps.customUserAgent = [customUserAgent
      stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

- (void)loadUrl:(hns_rewards::mojom::UrlRequestPtr)request
       callback:(hns_rewards::mojom::RewardsEngineClient::LoadURLCallback)
                    callback {
  std::map<hns_rewards::mojom::UrlMethod, std::string> methodMap{
      {hns_rewards::mojom::UrlMethod::GET, "GET"},
      {hns_rewards::mojom::UrlMethod::POST, "POST"},
      {hns_rewards::mojom::UrlMethod::PUT, "PUT"},
      {hns_rewards::mojom::UrlMethod::DEL, "DELETE"}};

  if (!request) {
    request = hns_rewards::mojom::UrlRequest::New();
  }

  const auto copiedURL = base::SysUTF8ToNSString(request->url);

  auto cb = std::make_shared<decltype(callback)>(std::move(callback));
  return [self.commonOps
      loadURLRequest:request->url
             headers:request->headers
             content:request->content
        content_type:request->content_type
              method:methodMap[request->method]
            callback:^(
                const std::string& errorDescription, int statusCode,
                const std::string& response,
                const base::flat_map<std::string, std::string>& headers) {
              auto url_response = hns_rewards::mojom::UrlResponse::New();
              url_response->url = base::SysNSStringToUTF8(copiedURL);
              url_response->error = errorDescription;
              url_response->status_code = statusCode;
              url_response->body = response;
              url_response->headers = headers;

              if (cb) {
                std::move(*cb).Run(std::move(url_response));
              }
            }];
}

- (void)
    fetchFavIcon:(const std::string&)url
      faviconKey:(const std::string&)faviconKey
        callback:
            (hns_rewards::mojom::RewardsEngineClient::FetchFavIconCallback)
                callback {
  std::move(callback).Run(NO, std::string());
}

#pragma mark - Logging

- (void)log:(const std::string&)file
            line:(int32_t)line
    verboseLevel:(int32_t)verboseLevel
         message:(const std::string&)message {
  const int vlog_level =
      logging::GetVlogLevelHelper(file.c_str(), file.length());
  if (verboseLevel <= vlog_level) {
    logging::LogMessage(file.c_str(), line, -verboseLevel).stream() << message;
  }
}

#pragma mark - Publisher Database

- (void)publisherListNormalized:
    (std::vector<hns_rewards::mojom::PublisherInfoPtr>)list {
  // Not used on iOS
}

- (void)onPanelPublisherInfo:(hns_rewards::mojom::Result)result
               publisherInfo:
                   (hns_rewards::mojom::PublisherInfoPtr)publisher_info
                    windowId:(uint64_t)windowId {
  if (publisher_info.get() == nullptr ||
      result != hns_rewards::mojom::Result::OK) {
    return;
  }
  auto info =
      [[HnsRewardsPublisherInfo alloc] initWithPublisherInfo:*publisher_info];
  for (RewardsObserver* observer in [self.observers copy]) {
    if (observer.fetchedPanelPublisher) {
      observer.fetchedPanelPublisher(info, windowId);
    }
  }
}

- (void)onPublisherRegistryUpdated {
  // Not used on iOS
}

- (void)onPublisherUpdated:(const std::string&)publisherId {
  // Not used on iOS
}

- (void)showNotification:(const std::string&)type
                    args:(std::vector<std::string>)args
                callback:(hns_rewards::mojom::RewardsEngineClient::
                              ShowNotificationCallback)callback {
  // Not used on iOS
}

- (void)clientInfo:
    (hns_rewards::mojom::RewardsEngineClient::GetClientInfoCallback)callback {
  auto info = hns_rewards::mojom::ClientInfo::New();
  info->os = hns_rewards::mojom::OperatingSystem::UNDEFINED;
  info->platform = hns_rewards::mojom::Platform::IOS;
  std::move(callback).Run(std::move(info));
}

- (void)unblindedTokensReady {
  [self fetchBalance:nil];
}

- (void)reconcileStampReset {
  // Not used on iOS
}

- (void)runDbTransaction:(hns_rewards::mojom::DBTransactionPtr)transaction
                callback:(hns_rewards::mojom::RewardsEngineClient::
                              RunDBTransactionCallback)callback {
  __weak HnsRewardsAPI* weakSelf = self;
  DCHECK(rewardsDatabase);
  rewardsDatabase
      .AsyncCall(&hns_rewards::internal::RewardsDatabase::RunTransaction)
      .WithArgs(std::move(transaction))
      .Then(base::BindOnce(
          ^(hns_rewards::internal::RunDBTransactionCallback completion,
            hns_rewards::mojom::DBCommandResponsePtr response) {
            if (weakSelf) {
              std::move(completion).Run(std::move(response));
            }
          },
          std::move(callback)));
}

- (void)walletDisconnected:(const std::string&)wallet_type {
  // Not used on iOS
}

- (void)deleteLog:
    (hns_rewards::mojom::RewardsEngineClient::DeleteLogCallback)callback {
  std::move(callback).Run(hns_rewards::mojom::Result::OK);
}

- (void)
    encryptString:(const std::string&)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::EncryptStringCallback)
                 callback {
  std::string encrypted_value;
  if (!OSCrypt::EncryptString(value, &encrypted_value)) {
    std::move(callback).Run(absl::nullopt);
    return;
  }
  std::move(callback).Run(absl::make_optional(encrypted_value));
}

- (void)
    decryptString:(const std::string&)value
         callback:
             (hns_rewards::mojom::RewardsEngineClient::DecryptStringCallback)
                 callback {
  std::string decrypted_value;
  if (!OSCrypt::DecryptString(value, &decrypted_value)) {
    std::move(callback).Run(absl::nullopt);
    return;
  }
  std::move(callback).Run(absl::make_optional(decrypted_value));
}

- (void)externalWalletConnected {
  // Not used on iOS
}

- (void)externalWalletLoggedOut {
  // Not used on iOS
}

- (void)externalWalletReconnected {
  // Not used on iOS
}

- (void)externalWalletDisconnected {
  // Not used on iOS
}

@end
