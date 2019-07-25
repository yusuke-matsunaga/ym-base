
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
#  RandSamplerTest
# ===================================================================

set ( RandSamplerTest_SOURCES
  RandSamplerTest.cc
  )

add_executable ( RandSamplerTest
  RandSamplerTest.cc
  $<TARGET_OBJECTS:ym_base_obj_a>
  )

target_compile_options ( RandSamplerTest
  PRIVATE "-O3"
  )

target_compile_definitions ( RandSamplerTest
  PRIVATE "-DNDEBUG"
  )

target_link_libraries ( RandSamplerTest
  pthread
  ${YM_LIB_DEPENDS}
  ${GTEST_BOTH_LIBRARIES}
  )

add_executable ( RandSamplerTest_d
  RandSamplerTest.cc
  $<TARGET_OBJECTS:ym_base_obj_ad>
  )

target_compile_options ( RandSamplerTest_d
  PRIVATE "-g"
  )

target_link_libraries ( RandSamplerTest_d
  pthread
  ${YM_LIB_DEPENDS}
  ${GTEST_BOTH_LIBRARIES}
  )
