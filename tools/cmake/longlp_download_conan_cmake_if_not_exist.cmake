#
# Download conan.cmake if not exist in |directory|
# |conan_cmake_file_path_value|:
#   install conan.cmake as this path
# longlp
#
include (longlp_message.cmake)
function (longlp_download_conan_cmake_if_not_exist conan_cmake_file_path_value)
  message (${conan_cmake_file_path_value})
  if (NOT EXISTS ${conan_cmake_file_path_value})
    longlp_message ("Downloading conan.cmake to ${conan_cmake_file_path_value}")
    file (DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
          ${conan_cmake_file_path_value})
  else ()
    longlp_message ("Already have conan.cmake in ${conan_cmake_file_path_value}")
  endif ()
endfunction ()
