/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <map>
#include <memory>
#include <utility>

#include "base/strings/stringprintf.h"
#include "hns/components/hns_rewards/core/common/time_util.h"
#include "hns/components/hns_rewards/core/database/database_contribution_queue.h"
#include "hns/components/hns_rewards/core/database/database_util.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

using std::placeholders::_1;

namespace hns_rewards::internal {
namespace database {

namespace {

const char kTableName[] = "contribution_queue";

}  // namespace

DatabaseContributionQueue::DatabaseContributionQueue(RewardsEngineImpl& engine)
    : DatabaseTable(engine), publishers_(engine) {}

DatabaseContributionQueue::~DatabaseContributionQueue() = default;

void DatabaseContributionQueue::InsertOrUpdate(mojom::ContributionQueuePtr info,
                                               LegacyResultCallback callback) {
  if (!info) {
    BLOG(0, "Queue is null");
    callback(mojom::Result::FAILED);
    return;
  }

  if (info->id.empty()) {
    BLOG(0, "Queue id is empty");
    callback(mojom::Result::FAILED);
    return;
  }

  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "INSERT OR REPLACE INTO %s (contribution_queue_id, type, amount, "
      "partial) "
      "VALUES (?, ?, ?, ?)",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::RUN;
  command->command = query;

  BindString(command.get(), 0, info->id);
  BindInt(command.get(), 1, static_cast<int>(info->type));
  BindDouble(command.get(), 2, info->amount);
  BindBool(command.get(), 3, info->partial);

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&DatabaseContributionQueue::OnInsertOrUpdate,
                     base::Unretained(this), std::move(callback),
                     std::move(info)));
}

void DatabaseContributionQueue::OnInsertOrUpdate(
    LegacyResultCallback callback,
    mojom::ContributionQueuePtr queue,
    mojom::DBCommandResponsePtr response) {
  CHECK(queue);

  if (!response ||
      response->status != mojom::DBCommandResponse::Status::RESPONSE_OK) {
    BLOG(0, "Response is not ok");
    callback(mojom::Result::FAILED);
    return;
  }

  publishers_.InsertOrUpdate(queue->id, std::move(queue->publishers),
                             std::move(callback));
}

void DatabaseContributionQueue::GetFirstRecord(
    GetFirstContributionQueueCallback callback) {
  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "SELECT contribution_queue_id, type, amount, partial "
      "FROM %s WHERE completed_at = 0 "
      "ORDER BY created_at ASC LIMIT 1",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::READ;
  command->command = query;

  command->record_bindings = {mojom::DBCommand::RecordBindingType::STRING_TYPE,
                              mojom::DBCommand::RecordBindingType::INT_TYPE,
                              mojom::DBCommand::RecordBindingType::DOUBLE_TYPE,
                              mojom::DBCommand::RecordBindingType::INT_TYPE};

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&DatabaseContributionQueue::OnGetFirstRecord,
                     base::Unretained(this), std::move(callback)));
}

void DatabaseContributionQueue::OnGetFirstRecord(
    GetFirstContributionQueueCallback callback,
    mojom::DBCommandResponsePtr response) {
  if (!response ||
      response->status != mojom::DBCommandResponse::Status::RESPONSE_OK) {
    BLOG(0, "Response is wrong");
    callback(nullptr);
    return;
  }

  if (response->result->get_records().size() != 1) {
    callback(nullptr);
    return;
  }

  auto* record = response->result->get_records()[0].get();

  auto info = mojom::ContributionQueue::New();
  info->id = GetStringColumn(record, 0);
  info->type = static_cast<mojom::RewardsType>(GetIntColumn(record, 1));
  info->amount = GetDoubleColumn(record, 2);
  info->partial = static_cast<bool>(GetIntColumn(record, 3));

  auto shared_info =
      std::make_shared<mojom::ContributionQueuePtr>(info->Clone());

  auto publishers_callback =
      std::bind(&DatabaseContributionQueue::OnGetPublishers, this, _1,
                shared_info, callback);

  publishers_.GetRecordsByQueueId(info->id, publishers_callback);
}

void DatabaseContributionQueue::OnGetPublishers(
    std::vector<mojom::ContributionQueuePublisherPtr> list,
    std::shared_ptr<mojom::ContributionQueuePtr> shared_queue,
    GetFirstContributionQueueCallback callback) {
  if (!shared_queue) {
    BLOG(0, "Queue is null");
    callback(nullptr);
    return;
  }

  (*shared_queue)->publishers = std::move(list);
  callback(std::move(*shared_queue));
}

void DatabaseContributionQueue::MarkRecordAsComplete(
    const std::string& id,
    LegacyResultCallback callback) {
  if (id.empty()) {
    BLOG(1, "Id is empty");
    callback(mojom::Result::FAILED);
    return;
  }

  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "UPDATE %s SET completed_at = ? WHERE contribution_queue_id = ?",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::RUN;
  command->command = query;

  BindInt64(command.get(), 0, util::GetCurrentTimeStamp());
  BindString(command.get(), 1, id);

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&OnResultCallback, std::move(callback)));
}

}  // namespace database
}  // namespace hns_rewards::internal
