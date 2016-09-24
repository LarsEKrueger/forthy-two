

set(SOURCE
  runtime.cpp
  parser.cpp
  )

set(HEADERS
  runtime.hpp
  parser.hpp
  )

add_library(forth STATIC ${SOURCE} ${HEADERS})
target_include_directories(forth PUBLIC ..)
install(TARGETS forth DESTINATION lib/Debug CONFIGURATIONS Debug)
install(TARGETS forth DESTINATION lib/Release CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
