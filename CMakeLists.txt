cmake_minimum_required(VERSION 3.15)
project(decision_support_system LANGUAGES CXX)

set(LONGLP_PROJECT_DIR "${PROJECT_SOURCE_DIR}")
set(LONGLP_PROJECT_SRC_DIR "${LONGLP_PROJECT_DIR}/src")
set(LONGLP_PROJECT_EXTERNAL_DIR "${LONGLP_PROJECT_DIR}/external")
set(LONGLP_PROJECT_DATA_DIR "${LONGLP_PROJECT_DIR}/data")
set(LONGLP_PROJECT_OUTPUT_DIR "${PROJECT_BINARY_DIR}")

add_subdirectory(tools/cmake)
add_subdirectory(external)
add_subdirectory(data)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(
  Qt5 5.13
  COMPONENTS Gui Widgets Core Gui Sql
  REQUIRED
)

longlp_make_exec(
  TARGET_NAME ${PROJECT_NAME}
  USE_DEFAULT
  SOURCES
  #[[]] PRIVATE
  #[[]]   src/main.cpp
  #[[]]   src/main_window.hpp src/main_window.cpp
  #[[]]   src/decision_table_window.cpp src/decision_table_window.hpp
  #[[]]   src/defs.hpp
  #[[]]   src/database_service.cpp src/database_service.hpp
  #[[]]   src/widget_creator.cpp src/widget_creator.hpp
  #[[]]   src/solver.cpp src/solver.hpp
  ##[[]]   $<TARGET_OBJECTS:nlohmann_json>
  LINK_LIBRARIES
  #[[]] PRIVATE Qt5::Widgets Qt5::Core Qt5::Sql Qt5::Gui
  INCLUDE_DIRECTORIES
  #[[]] SYSTEM PRIVATE external/json/single_include
  DEPENDENCIES
  #[[]] copy_data
)
