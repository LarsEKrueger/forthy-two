function(DEFINE_TEST baseName)
add_test(
  NAME examples_${baseName}
  COMMAND forthytwo --test ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.t42 ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.42
)
endfunction()

DEFINE_TEST(bottles)
DEFINE_TEST(euler1)
