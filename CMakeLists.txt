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

ym_init_readline ()


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

if ( Python3_FOUND )
  add_subdirectory ( py_ymbase )
endif ( Python3_FOUND )
add_subdirectory ( tests )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( binder_SOURCES
  c++-srcs/binder/Binder.cc
  )

set ( binio_SOURCES
  c++-srcs/binio/BinEnc.cc
  )

set ( file_info_SOURCES
  c++-srcs/file_info/File.cc
  c++-srcs/file_info/FileInfo.cc
  c++-srcs/file_info/FileInfoMgr.cc
  )

set ( gen_SOURCES
  c++-srcs/gen/GenBase.cc
  c++-srcs/gen/MultiGenBase.cc
  c++-srcs/gen/MultiSetGenBase.cc
  )

set ( itvl_SOURCES
  c++-srcs/itvl/ItvlMgr.cc
  c++-srcs/itvl/ItvlTree.cc
  c++-srcs/itvl/NameMgr.cc
  )

set ( misc_SOURCES
  c++-srcs/misc/ReadLine.cc
  c++-srcs/misc/UnionFindSet.cc
  )

set ( msg_SOURCES
  c++-srcs/msg/MsgHandler.cc
  c++-srcs/msg/MsgMgr.cc
  c++-srcs/msg/MsgMgrImpl.cc
  )

if ( POPT_FOUND )
  set ( popt_SOURCES
    c++-srcs/popt/PoptMainApp.cc
    )
else ( )
  set ( popt_SOURCES )
endif ( )

set ( textproc_SOURCES
  c++-srcs/textproc/OptionParser.cc
  c++-srcs/textproc/Scanner.cc
  c++-srcs/textproc/ShString.cc
  c++-srcs/textproc/StrBuff.cc
  )

set ( ym_base_SOURCES
  ${binder_SOURCES}
  ${binio_SOURCES}
  ${file_info_SOURCES}
  ${gen_SOURCES}
  ${itvl_SOURCES}
  ${misc_SOURCES}
  ${msg_SOURCES}
  ${popt_SOURCES}
  ${textproc_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_base
  ${ym_base_SOURCES}
  )
