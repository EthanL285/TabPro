# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\TabPro_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TabPro_autogen.dir\\ParseCache.txt"
  "TabPro_autogen"
  )
endif()
