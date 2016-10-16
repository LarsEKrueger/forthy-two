

set(SOURCE
  runtime.cpp
  parser.cpp
  tester.cpp
  )

set(HEADERS
  runtime.hpp
  parser.hpp
  tester.hpp
  )

add_library(forth STATIC ${SOURCE} ${HEADERS})
target_include_directories(forth PUBLIC ..)
