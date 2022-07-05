
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
  misc/RandSamplerTest.cc
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

add_executable ( ReadLineTest
  misc/ReadLineTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( ReadLineTest
  PRIVATE "-g"
  )

target_link_libraries ( ReadLineTest
  ${YM_LIB_DEPENDS}
  )


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
  ${CMAKE_CURRENT_SOURCE_DIR}/../c++-srcs/itvl
  )

ym_add_gtest ( base_OptionParser_test
  textproc/OptionParserTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

ym_add_gtest ( base_ShString_test
  textproc/ShStringTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )
