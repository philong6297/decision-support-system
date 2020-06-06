#
# My desired compile options for each compiler
# |output_variable|:
#   store the list of compiler flags
#

function (longlp_desired_compile_options output_variable)
  set(GNU_compile_options_
      -std=c++17
      -Wall -Wextra -Wpedantic
      -Walloc-zero
      -Walloca -Wno-c++11-compat -Wno-c++14-compat
      -Wcast-align -Wcast-qual -Wconditionally-supported
      -Wconversion -Wctor-dtor-privacy -Wdate-time
      -Wdelete-non-virtual-dtor -Wdisabled-optimization -Wdouble-promotion
      -Wduplicated-branches -Wduplicated-cond -Weffc++
      -Wfloat-conversion -Wfloat-equal -Wformat-nonliteral
      -Wformat-security -Wformat-signedness -Wformat-y2k
      -Winit-self -Winline -Winvalid-pch
      -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
      -Wmultichar -Wmultiple-inheritance -Wnoexcept
      -Wnoexcept-type -Wnon-virtual-dtor -Wnull-dereference
      -Wold-style-cast -Woverloaded-virtual -Wpacked
      -Wpadded -Wredundant-decls -Wregister
      -Wreorder -Wrestrict -Wshadow
      -Wshadow=compatible-local -Wshadow=local -Wsign-conversion
      -Wsign-promo -Wstack-protector -Wstrict-null-sentinel
      -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=noreturn
      -Wsuggest-attribute=pure -Wsuggest-final-methods -Wsuggest-final-types
      -Wsuggest-override -Wswitch-default -Wswitch-enum
      -Wsynth -Wtrampolines -Wundef
      -Wunsafe-loop-optimizations -Wunused-macros -Wuseless-cast
      -Wvector-operation-performance -Wvirtual-inheritance -Wwrite-strings
      -Wzero-as-null-pointer-constant -Wno-deprecated -Wnon-template-friend
      -Wno-pmf-conversions)
  set(Clang_compile_options_
      -std=c++17
      -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic
      -Wno-c++11-compat -Wno-c++11-compat-pedantic -Wno-c++14-compat
      -Wno-c++14-compat-pedantic)
  set(MSVC_compile_options_
      /W4 /WX /std:c++17
      /permissive-)
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      list(APPEND Clang_compile_options_ -O0 -mno-omit-leaf-frame-pointer)
      list(APPEND GNU_compile_options_ -O0 -fno-omit-frame-pointer)
    endif ()
  endif ()
  set(${output_variable} ${${CMAKE_CXX_COMPILER_ID}_compile_options_} PARENT_SCOPE)
endfunction ()
