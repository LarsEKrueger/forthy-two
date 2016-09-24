find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

function(DEFINE_TEST baseName)
  add_executable(test_${baseName} test_${baseName}_main.cpp)
  target_link_libraries(test_${baseName}
    forth
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
  add_test(forth_${baseName} test_${baseName})
endfunction()

DEFINE_TEST(runtime)
