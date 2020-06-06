longlp_desired_compile_features(features_)
longlp_make_library(
  TARGET_NAME nlohmann_json
  ALIAS_NAME nlohmann_json::nlohmann_json
  LIBRARY_TYPE OBJECT
  COMPILE_OPTIONS PRIVATE "-w"
  COMPILE_FEATURES PRIVATE ${features_}
  SOURCES
  #[[]] PRIVATE single_include/nlohmann/json.hpp main.cpp
)
