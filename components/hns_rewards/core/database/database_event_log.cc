/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <utility>
#include <vector>

#include "base/functional/callback_helpers.h"
#include "base/strings/stringprintf.h"
#include "base/uuid.h"
#include "hns/components/hns_rewards/core/common/time_util.h"
#include "hns/components/hns_rewards/core/database/database_event_log.h"
#include "hns/components/hns_rewards/core/database/database_util.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl.h"

namespace hns_rewards::internal {
namespace database {

namespace {

const char kTableName[] = "event_log";

}  // namespace

DatabaseEventLog::DatabaseEventLog(RewardsEngineImpl& engine)
    : DatabaseTable(engine) {}

DatabaseEventLog::~DatabaseEventLog() = default;

void DatabaseEventLog::Insert(const std::string& key,
                              const std::string& value) {
  if (key.empty()) {
    BLOG(0, "Key is empty");
    return;
  }

  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "INSERT INTO %s (event_log_id, key, value, created_at) "
      "VALUES (?, ?, ?, ?)",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::RUN;
  command->command = query;

  BindString(command.get(), 0,
             base::Uuid::GenerateRandomV4().AsLowercaseString());
  BindString(command.get(), 1, key);
  BindString(command.get(), 2, value);
  BindInt64(command.get(), 3, util::GetCurrentTimeStamp());

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(std::move(transaction),
                                      base::DoNothing());
}

void DatabaseEventLog::InsertRecords(
    const std::map<std::string, std::string>& records,
    LegacyResultCallback callback) {
  if (records.empty()) {
    BLOG(0, "No records");
    callback(mojom::Result::NOT_FOUND);
    return;
  }

  auto transaction = mojom::DBTransaction::New();
  auto time = util::GetCurrentTimeStamp();
  const std::string base_query = base::StringPrintf(
      "INSERT INTO %s (event_log_id, key, value, created_at) VALUES ",
      kTableName);

  std::string query = base_query;
  for (const auto& record : records) {
    query += base::StringPrintf(
        R"(('%s','%s','%s',%u),)",
        base::Uuid::GenerateRandomV4().AsLowercaseString().c_str(),
        record.first.c_str(), record.second.c_str(),
        static_cast<uint32_t>(time));
  }

  query.pop_back();
  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::EXECUTE;
  command->command = query;

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&OnResultCallback, std::move(callback)));
}

void DatabaseEventLog::GetLastRecords(GetEventLogsCallback callback) {
  auto transaction = mojom::DBTransaction::New();

  const std::string query = base::StringPrintf(
      "SELECT event_log_id, key, value, created_at "
      "FROM %s ORDER BY created_at DESC, ROWID DESC LIMIT 2000",
      kTableName);

  auto command = mojom::DBCommand::New();
  command->type = mojom::DBCommand::Type::READ;
  command->command = query;

  command->record_bindings = {
      mojom::DBCommand::RecordBindingType::STRING_TYPE,
      mojom::DBCommand::RecordBindingType::STRING_TYPE,
      mojom::DBCommand::RecordBindingType::STRING_TYPE,
      mojom::DBCommand::RecordBindingType::INT64_TYPE,
  };

  transaction->commands.push_back(std::move(command));

  engine_->client()->RunDBTransaction(
      std::move(transaction),
      base::BindOnce(&DatabaseEventLog::OnGetAllRecords, base::Unretained(this),
                     std::move(callback)));
}

void DatabaseEventLog::OnGetAllRecords(GetEventLogsCallback callback,
                                       mojom::DBCommandResponsePtr response) {
  if (!response ||
      response->status != mojom::DBCommandResponse::Status::RESPONSE_OK) {
    BLOG(0, "Response is wrong");
    callback({});
    return;
  }

  std::vector<mojom::EventLogPtr> list;
  for (auto const& record : response->result->get_records()) {
    auto info = mojom::EventLog::New();
    auto* record_pointer = record.get();

    info->event_log_id = GetStringColumn(record_pointer, 0);
    info->key = GetStringColumn(record_pointer, 1);
    info->value = GetStringColumn(record_pointer, 2);
    info->created_at = GetInt64Column(record_pointer, 3);

    list.push_back(std::move(info));
  }

  callback(std::move(list));
}

}  // namespace database
}  // namespace hns_rewards::internal
