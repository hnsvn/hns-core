/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <map>
#include <utility>

#include "base/strings/stringprintf.h"
#include "hns/components/hns_rewards/core/common/time_util.h"
#include "hns/components/hns_rewards/core/database/database_contribution_queue_publishers.h"
#include "hns/components/hns_rewards/core/database/database_util.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace database {

namespace {

const char kTableName[] = "contribution_queue_publishers";

}  // namespace

DatabaseContributionQueuePublishers::DatabaseContributionQueuePublishers(
    RewardsEngineImpl& engine)
    : DatabaseTable(engine) {}

DatabaseContributionQueuePublishers::~DatabaseContributionQueuePublishers() =
    default;

void DatabaseContributionQueuePublishers::InsertOrUpdate(
    const std::string& id,
    std::vector<mojom::ContributionQueuePublisherPtr> list,
    LegacyResultCallback callback) {
  if (id.empty() || list.empty()) {
    BLOG(1, "Empty data");
    callback(mojom::Result::FAILED);
    return;
  }

  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "INSERT OR REPLACE INTO %s "
      "(contribution_queue_id, publisher_key, amount_percent) VALUES (?, ?, ?)",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::RUN;
  command->command = query;

  for (const auto& publisher : list) {
    BindString(command.get(), 0, id);
    BindString(command.get(), 1, publisher->publisher_key);
    BindDouble(command.get(), 2, publisher->amount_percent);

    transaction->commands.push_back(command->Clone());
  }

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&OnResultCallback, std::move(callback)));
}

void DatabaseContributionQueuePublishers::GetRecordsByQueueId(
    const std::string& queue_id,
    ContributionQueuePublishersListCallback callback) {
  if (queue_id.empty()) {
    BLOG(1, "Queue id is empty");
    callback({});
    return;
  }

  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "SELECT publisher_key, amount_percent "
      "FROM %s WHERE contribution_queue_id = ?",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::READ;
  command->command = query;

  BindString(command.get(), 0, queue_id);

  command->record_bindings = {mojom::DBCommand::RecordBindingType::STRING_TYPE,
                              mojom::DBCommand::RecordBindingType::DOUBLE_TYPE};

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(
          &DatabaseContributionQueuePublishers::OnGetRecordsByQueueId,
          base::Unretained(this), std::move(callback)));
}

void DatabaseContributionQueuePublishers::OnGetRecordsByQueueId(
    ContributionQueuePublishersListCallback callback,
    mojom::DBCommandResponsePtr response) {
  if (!response ||
      response->status != mojom::DBCommandResponse::Status::RESPONSE_OK) {
    BLOG(0, "Response is wrong");
    callback({});
    return;
  }

  std::vector<mojom::ContributionQueuePublisherPtr> list;
  for (auto const& record : response->result->get_records()) {
    auto info = mojom::ContributionQueuePublisher::New();
    auto* record_pointer = record.get();

    info->publisher_key = GetStringColumn(record_pointer, 0);
    info->amount_percent = GetDoubleColumn(record_pointer, 1);

    list.push_back(std::move(info));
  }

  callback(std::move(list));
}

}  // namespace database
}  // namespace hns_rewards::internal