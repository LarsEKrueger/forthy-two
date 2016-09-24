find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_executable(test_runtime test_runtime_main.cpp)
target_link_libraries(test_runtime
  forth
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
add_test(forth_runtime test_runtime)
