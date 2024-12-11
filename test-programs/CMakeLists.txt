
# ===================================================================
# インクルードパスの設定
# ===================================================================


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
  ${RandSamplerTest_SOURCES}
  $<TARGET_OBJECTS:ym_base_obj>
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
  )

add_executable ( RandSamplerTest_d
  ${RandSamplerTest_SOURCES}
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( RandSamplerTest_d
  PRIVATE "-g"
  )

target_link_libraries ( RandSamplerTest_d
  pthread
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  ReadLineTest
# ===================================================================

add_executable ( ReadLineTest
  ReadLineTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( ReadLineTest
  PRIVATE "-g"
  )

target_link_libraries ( ReadLineTest
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  IntervalTimer_test
# ===================================================================

add_executable ( IntervalTimer_test
  IntervalTimer_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( IntervalTimer_test
  PRIVATE "-g"
  )

target_link_libraries ( IntervalTimer_test
  ${YM_LIB_DEPENDS}
  )
