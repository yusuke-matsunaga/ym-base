# ym-common に内蔵の gtest を使うためのマクロ
macro ( ym_use_gtest )
  add_subdirectory (ym-common/gtest-1.7.0)
  set (GTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/ym-common/gtest-1.7.0/include")
  set (GTEST_BOTH_LIBRARIES
    "${PROJECT_BINARY_DIR}/ym-common/gtest-1.7.0/libgtest.a"
    "${PROJECT_BINARY_DIR}/ym-common/gtest-1.7.0/libgtest_main.a")
endmacro ()
