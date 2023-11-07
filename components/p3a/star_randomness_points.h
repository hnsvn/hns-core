/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_P3A_STAR_RANDOMNESS_POINTS_H_
#define HNS_COMPONENTS_P3A_STAR_RANDOMNESS_POINTS_H_

#include <memory>
#include <string>

#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/ref_counted.h"
#include "hns/components/p3a/constellation/rs/cxx/src/lib.rs.h"
#include "hns/components/p3a/star_randomness_meta.h"

namespace network {
class SharedURLLoaderFactory;
class SimpleURLLoader;
}  // namespace network

namespace p3a {

struct P3AConfig;

// Handles sending requests/handling responses to/from the randomness
// server in order to receive randomness point data for STAR measurements.
class StarRandomnessPoints {
 public:
  using RandomnessDataCallback = base::RepeatingCallback<void(
      std::string metric_name,
      uint8_t epoch,
      ::rust::Box<constellation::RandomnessRequestStateWrapper>
          randomness_request_state,
      std::unique_ptr<rust::Vec<constellation::VecU8>> resp_points,
      std::unique_ptr<rust::Vec<constellation::VecU8>> resp_proofs)>;

  StarRandomnessPoints(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      RandomnessDataCallback data_callback,
      const P3AConfig* config);
  ~StarRandomnessPoints();
  StarRandomnessPoints(const StarRandomnessPoints&) = delete;
  StarRandomnessPoints& operator=(const StarRandomnessPoints&) = delete;

  void SendRandomnessRequest(
      std::string metric_name,
      StarRandomnessMeta* randomness_meta,
      uint8_t epoch,
      ::rust::Box<constellation::RandomnessRequestStateWrapper>
          randomness_request_state,
      const rust::Vec<constellation::VecU8>& rand_req_points);

 private:
  void HandleRandomnessResponse(
      std::string metric_name,
      StarRandomnessMeta* randomness_meta,
      uint8_t epoch,
      ::rust::Box<constellation::RandomnessRequestStateWrapper>
          randomness_request_state,
      std::unique_ptr<std::string> response_body);

  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  std::unique_ptr<network::SimpleURLLoader> url_loader_;

  RandomnessDataCallback data_callback_;

  const raw_ptr<const P3AConfig> config_;
};

}  // namespace p3a

#endif  // HNS_COMPONENTS_P3A_STAR_RANDOMNESS_POINTS_H_
