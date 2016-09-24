
add_executable(forthytwo forthytwo_main.cpp)
target_link_libraries(forthytwo forth)
install(TARGETS forthytwo DESTINATION bin/Debug CONFIGURATIONS Debug)
install(TARGETS forthytwo DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
