
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  RandSamplerSimpleTest
# ===================================================================

set ( RandSamplerSimpleTest_SOURCES
  RandSamplerSimpleTest.cc
  )

add_executable ( RandSamplerSimpleTest
  RandSamplerSimpleTest.cc
  $<TARGET_OBJECTS:ym_base_d>
  )

target_compile_options ( RandSamplerSimpleTest
  PRIVATE "-g"
  )

target_link_libraries ( RandSamplerSimpleTest
  pthread
  ${YM_LIB_DEPENDS}
  ${GTEST_BOTH_LIBRARIES}
  )
