# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================

# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory( gtest )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( itvl_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ItvlMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ItvlTree.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/NameMgr.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
