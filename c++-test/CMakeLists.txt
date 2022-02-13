
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
  ${CMAKE_CURRENT_SOURCE_DIR}/../c++-srcs/itvl
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


# ===================================================================
# BinCodec 関係のテスト
# ===================================================================

ym_add_gtest ( base_BinCodec_test
  io/BinCodecTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

if ( 0 )
# ===================================================================
# ZFile 関係のテスト
# ===================================================================

set ( TESTDATA_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/testdata
  )

ym_add_gtest ( base_compress_test
  io/compress_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_include_directories ( base_compress_test
  PRIVATE
  ../c++-srcs/io
  )

target_compile_definitions ( base_compress_test
  PRIVATE
  "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

target_link_libraries ( base_compress_test
  )


# ===================================================================
# XzFile 関係のテスト
# ===================================================================
if ( LIBLZMA_FOUND )
  ym_add_gtest ( base_xz_test
    io/xz_test.cc
    $<TARGET_OBJECTS:ym_base_obj_d>
    )

  target_include_directories ( base_xz_test
    PRIVATE
    ../c++-srcs/io
    )

  target_compile_definitions ( base_xz_test
    PRIVATE
    "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
    )

  target_link_libraries ( base_xz_test
    ${LIBLZMA_LIBRARIES}
    )

endif ( LIBLZMA_FOUND )


# ===================================================================
# GzFile 関係のテスト
# ===================================================================
if ( ZLIB_FOUND )
  ym_add_gtest ( base_gzip_test
    io/gzip_test.cc
    $<TARGET_OBJECTS:ym_base_obj_d>
    )

  target_include_directories ( base_gzip_test
    PRIVATE
    ../c++-srcs/io
    )

  target_compile_definitions ( base_gzip_test
    PRIVATE
    "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
    )

  target_link_libraries ( base_gzip_test
    ${ZLIB_LIBRARIES}
    )

endif ( ZLIB_FOUND )


# ===================================================================
# BzFile 関係のテスト
# ===================================================================
if ( BZIP2_FOUND )
  ym_add_gtest ( base_bzip2_test
    io/bzip2_test.cc
    $<TARGET_OBJECTS:ym_base_obj_d>
    )

  target_include_directories ( base_bzip2_test
    PRIVATE
    ../c++-srcs/io
    )

  target_compile_definitions ( base_bzip2_test
    PRIVATE
    "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
    )

  target_link_libraries ( base_bzip2_test
    ${BZIP2_LIBRARIES}
    )

endif ( BZIP2_FOUND )


# ===================================================================
# CodecGen 関係のテスト
# ===================================================================
ym_add_gtest ( base_codecgen_test
  io/codecgen_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )


# ===================================================================
# basic_codec_streambuf 関係のテスト
# ===================================================================
ym_add_gtest ( base_streambuf_test
  io/streambuf_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  )
endif()