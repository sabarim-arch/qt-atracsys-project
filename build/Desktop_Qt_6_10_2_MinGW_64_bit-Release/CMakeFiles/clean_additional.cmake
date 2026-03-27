# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "AtracsysSessionApp_autogen"
  "CMakeFiles\\AtracsysSessionApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AtracsysSessionApp_autogen.dir\\ParseCache.txt"
  )
endif()
