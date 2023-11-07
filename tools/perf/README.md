### How to update wpr

* Get write access to `hns-telemetry` GCS bucket (ask @devops-team);
* `cd src/`;
* Remove old downloaded wprs: `rm -rf ./hns/tools/perf/page_sets/data/*.wprgo`;
* Record new wprs: `vpython3 tools/perf/record_wpr <benchmark_name> --browser=system  --story-filter <story-filter>`;
* Upload the archives to GCS: `ls ./hns/tools/perf/page_sets/data/*.wprgo | xargs upload_to_google_storage.py --bucket=hns-telemetry`.
  `*.sha1` files will be generated;
* `cd hns`;
* Review and commit new `.sha1` files (not `.wprgo`) plus new entries in
  `./hns/tools/perf/page_sets_data/*.json`.
