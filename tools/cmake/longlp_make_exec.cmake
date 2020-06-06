include (longlp_message.cmake)

#
#   Make executable target
#   @TARGET_NAME: string, required, name of target
#   @ALIAS_NAME: string, optional, name of alias
#   @SOURCES: list, required, shortcut of target_sources
#   @DEPENDENCIES: list, optional, shortcut of add_dependencies
#   @COMPILE_OPTIONS: list, optional, shortcut of target_compile_options
#   @COMPILE_FEATURES: list, optional, shortcut of target_compule_features
#   @COMPILE_DEFINITIONS: list, optional, shortcut of target_compule_definitions
#   @INCLUDE_DIRECTORIES: list, optional, shortcut of target_include_directories
#   @LINK_LIBRARIES: list, optional, shortcut of target_link_libraries
#   @LINK_OPTIONS: list, optional, shortcut of target_link_options
#   @LINK_DIRECTORIES: list, optional, shortcut of target_link_directories
#
function (longlp_make_exec)
  set (options_ USE_DEFAULT)
  set (one_value_keys_ TARGET_NAME ALIAS_NAME)
  set (multi_value_keys_
       SOURCES DEPENDENCIES
       COMPILE_OPTIONS COMPILE_FEATURES COMPILE_DEFINITIONS
       INCLUDE_DIRECTORIES
       LINK_LIBRARIES LINK_OPTIONS LINK_DIRECTORIES
       )
  cmake_parse_arguments (
    EXEC
    "${options_}"
    "${one_value_keys_}"
    "${multi_value_keys_}"
    ${ARGV})
  if ((DEFINED EXEC_TARGET_NAME) AND (DEFINED EXEC_SOURCES))
    set (target_ ${EXEC_TARGET_NAME})
    add_executable (${target_}) # define exec
    target_sources (${target_} ${EXEC_SOURCES}) # add sources to target
    
    # create alias name
    if (DEFINED EXEC_ALIAS_NAME)
      add_executable (${EXEC_ALIAS_NAME} ALIAS ${target_})
    endif ()
    
    # add dependencies
    if (DEFINED EXEC_DEPENDENCIES)
      add_dependencies (${target_} ${EXEC_DEPENDENCIES})
    endif ()
    
    # config compile options
    if (DEFINED EXEC_COMPILE_OPTIONS)
      target_compile_options (${target_} ${EXEC_COMPILE_OPTIONS})
    elseif (DEFINED EXEC_USE_DEFAULT)
      longlp_desired_compile_options (default_options_)
      target_compile_options (${target_} PRIVATE ${default_options_})
    endif ()
    
    # config compile features
    if (DEFINED EXEC_COMPILE_FEATURES)
      target_compile_features (${target_} ${EXEC_COMPILE_FEATURES})
    elseif (DEFINED EXEC_USE_DEFAULT)
      longlp_desired_compile_features (default_options_)
      target_compile_features (${target_} PRIVATE ${default_options_})
    endif ()
    
    # compile definitions
    if (DEFINED EXEC_COMPILE_DEFINITIONS)
      target_compile_definitions (${target_} ${EXEC_COMPILE_DEFINITIONS})
    endif ()
    
    # include directories
    if (DEFINED EXEC_INCLUDE_DIRECTORIES)
      target_include_directories (${target_} ${EXEC_INCLUDE_DIRECTORIES})
    endif ()
    
    # link libraries
    if (DEFINED EXEC_LINK_LIBRARIES)
      target_link_libraries (${target_} ${EXEC_LINK_LIBRARIES})
    endif ()
    
    # link options
    if (DEFINED EXEC_LINK_OPTIONS)
      target_link_options (${target_} ${EXEC_LINK_OPTIONS})
    endif ()
    
    # link directories
    if (DEFINED EXEC_LINK_DIRECTORIES)
      target_link_directories (${target_} ${EXEC_LINK_DIRECTORIES})
    endif ()
  
  else ()
    longlp_message (FATAL_ERROR "TARGET_NAME and SOURCES must be defined")
  endif ()
endfunction ()
