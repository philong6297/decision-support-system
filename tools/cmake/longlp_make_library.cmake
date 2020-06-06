include(longlp_message.cmake)
include(longlp_desired_compile_options.cmake)
include(longlp_desired_compile_features.cmake)
#
#   Make library target
#   @TARGET_NAME: string, required, name of target
#   @ALIAS_NAME: string, optional, name of alias
#   @LIBRARY_TYPE: list, required, specify which type of the target as follow:
#       Normal Libraries: [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#       Imported Libraries: <SHARED|STATIC|MODULE|OBJECT|UNKNOWN> IMPORTED [GLOBAL]
#       Object Libraries: OBJECT
#       Interface Libraries: INTERFACE [IMPORTED [GLOBAL]]
#   @USE_DEFAULT: option, if specified, target use default compile features and options
#   @DEPENDENCIES: list, optional, shortcut of add_dependencies
#   @COMPILE_OPTIONS: list, optional, shortcut of target_compile_options
#   @COMPILE_FEATURES: list, optional, shortcut of target_compile_features
#   @COMPILE_DEFINITIONS: list, optional, shortcut of target_compile_definitions
#   @INCLUDE_DIRECTORIES: list, optional, shortcut of target_include_directories
#   @LINK_LIBRARIES: list, optional, shortcut of target_link_libraries
#   @LINK_OPTIONS: list, optional, shortcut of target_link_options
#   @LINK_DIRECTORIES: list, optional, shortcut of target_link_directories
#
function (longlp_make_library)
  set(options_ USE_DEFAULT)
  set(one_value_keys_ TARGET_NAME ALIAS_NAME)
  set(multi_value_keys_
      LIBRARY_TYPE
      SOURCES DEPENDENCIES
      COMPILE_OPTIONS COMPILE_FEATURES COMPILE_DEFINITIONS
      INCLUDE_DIRECTORIES
      LINK_LIBRARIES LINK_OPTIONS LINK_DIRECTORIES
      )
  cmake_parse_arguments(
    LIB
    "${options_}"
    "${one_value_keys_}"
    "${multi_value_keys_}"
    ${ARGV})
  if ((DEFINED LIB_TARGET_NAME) AND (DEFINED LIB_SOURCES) AND (DEFINED LIB_LIBRARY_TYPE))
    set(target_ ${LIB_TARGET_NAME})
    add_library(${target_} ${LIB_LIBRARY_TYPE}) # define lib
    target_sources(${target_} ${LIB_SOURCES}) # add sources to target

    # create alias name
    if (DEFINED LIB_ALIAS_NAME)
      add_library(${LIB_ALIAS_NAME} ALIAS ${target_})
    endif ()

    # add dependencies
    if (DEFINED LIB_DEPENDENCIES)
      add_dependencies(${target_} ${LIB_DEPENDENCIES})
    endif ()

    # config compile options
    if (DEFINED LIB_COMPILE_OPTIONS)
      target_compile_options(${target_} ${LIB_COMPILE_OPTIONS})
    elseif (DEFINED LIB_USE_DEFAULT)
      longlp_desired_compile_options(default_options_)
      target_compile_options(${target_} PRIVATE ${default_options_})
    endif ()

    # config compile features
    if (DEFINED LIB_COMPILE_FEATURES)
      target_compile_features(${target_} ${LIB_COMPILE_FEATURES})
    elseif (DEFINED LIB_USE_DEFAULT)
      longlp_desired_compile_features(default_options_)
      target_compile_features(${target_} PRIVATE ${default_options_})
    endif ()

    # compile definitions
    if (DEFINED LIB_COMPILE_DEFINITIONS)
      target_compile_definitions(${target_} ${LIB_COMPILE_DEFINITIONS})
    endif ()

    # include directories
    if (DEFINED LIB_INCLUDE_DIRECTORIES)
      target_include_directories(${target_} ${LIB_INCLUDE_DIRECTORIES})
    endif ()

    # link libraries
    if (DEFINED LIB_LINK_LIBRARIES)
      target_link_libraries(${target_} ${LIB_LINK_LIBRARIES})
    endif ()

    # link options
    if (DEFINED LIB_LINK_OPTIONS)
      target_link_options(${target_} ${LIB_LINK_OPTIONS})
    endif ()

    # link directories
    if (DEFINED LIB_LINK_DIRECTORIES)
      target_link_directories(${target_} ${LIB_LINK_DIRECTORIES})
    endif ()

  else ()
    longlp_message(FATAL_ERROR "TARGET_NAME, LIBRARY_TYPE and SOURCES must be defined")
  endif ()
endfunction ()
