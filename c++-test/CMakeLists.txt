
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

ym_add_gtest ( base_MultiSetCombiGen_test
  gen/MultiSetCombiGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_MultiSetPermGen_test
  gen/MultiSetPermGenTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_Array_test
  misc/ArrayTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_UnionFindSet_test
  misc/UnionFindSet_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_ItvlTree_test
  itvl/ItvlTree_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_include_directories( base_ItvlTree_test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../c++-src/itvl
  )

ym_add_gtest ( base_Range_test
  misc/RangeTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_OptionParser_test
  textproc/OptionParserTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_ShString_test
  textproc/ShStringTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

#ym_add_gtest ( base_InputFileObj_test
#  file/InputFileObjTest.cc
#  $<TARGET_OBJECTS:ym_base_obj_d>
#  DEFINITIONS
#  "-DDATAPATH=\"${CMAKE_CURRENT_SOURCE_DIR}/testdata/\""
#  )

ym_add_gtest ( base_BinCodec_test
  io/BinCodecTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )