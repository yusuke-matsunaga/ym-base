
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest ( base_CombiGen_test
  gen/CombiGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_PermGen_test
  gen/PermGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_MultiCombiGen_test
  gen/MultiCombiGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_MultiPermGen_test
  gen/MultiPermGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_Array_test
  misc/ArrayTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_Range_test
  misc/RangeTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_OptionParser_test
  textproc/OptionParserTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )
