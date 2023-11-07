/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_IPFS_BLOB_CONTEXT_GETTER_FACTORY_H_
#define HNS_COMPONENTS_IPFS_BLOB_CONTEXT_GETTER_FACTORY_H_

#include <memory>

#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"

namespace storage {
class BlobStorageContext;
}  // namespace storage

namespace ipfs {

using BlobContextGetter =
    base::RepeatingCallback<base::WeakPtr<storage::BlobStorageContext>()>;

// Retrieves a blob storage context on IO thread.
class BlobContextGetterFactory {
 public:
  virtual base::WeakPtr<storage::BlobStorageContext>
  RetrieveStorageContext() = 0;

  BlobContextGetterFactory(const BlobContextGetterFactory&) = delete;
  BlobContextGetterFactory& operator=(const BlobContextGetterFactory&) = delete;
  virtual ~BlobContextGetterFactory() = default;

 protected:
  BlobContextGetterFactory() = default;
};

using BlobContextGetterFactoryPtr = std::unique_ptr<BlobContextGetterFactory>;

}  // namespace ipfs

#endif  // HNS_COMPONENTS_IPFS_BLOB_CONTEXT_GETTER_FACTORY_H_
