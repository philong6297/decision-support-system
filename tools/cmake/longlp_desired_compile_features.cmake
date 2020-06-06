#
# My desired compile features for each compiler
# |output_variable| will stored the list of compile features
#
# include
function (longlp_desired_compile_features output_variable)
  set (GNU_compile_features_ cxx_std_17)
  set (Clang_compile_features_ cxx_std_17)
  set (MSVC_compile_features_ cxx_std_17)
  set (${output_variable} ${${CMAKE_CXX_COMPILER_ID}_compile_features_} PARENT_SCOPE)
endfunction ()
