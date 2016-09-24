

set(SOURCE
  runtime.cpp
  )

set(HEADERS
  runtime.hpp
  )

add_library(forth STATIC ${SOURCE} ${HEADERS})
target_include_directories(forth PUBLIC ..)
install(TARGETS forth DESTINATION lib/Debug CONFIGURATIONS Debug)
install(TARGETS forth DESTINATION lib/Release CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
