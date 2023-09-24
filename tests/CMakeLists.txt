
# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

set ( TESTDATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/testdata/" )

add_subdirectory ( gtest/binio )
add_subdirectory ( gtest/gen )
add_subdirectory ( gtest/itvl )
add_subdirectory ( gtest/misc )
add_subdirectory ( gtest/textproc )
add_subdirectory ( gtest/json )
add_subdirectory ( programs )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
