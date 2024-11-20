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

add_subdirectory( binio/gtest )
add_subdirectory( gen/gtest )
add_subdirectory( itvl/gtest )
add_subdirectory( json/gtest )
add_subdirectory( misc/gtest )
add_subdirectory( textproc/gtest )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( binder_SOURCES
  binder/Binder.cc
  )

set ( binio_SOURCES
  binio/BinEnc.cc
  )

set ( file_info_SOURCES
  file_info/File.cc
  file_info/FileInfo.cc
  file_info/FileInfoMgr.cc
  )

set ( gen_SOURCES
  gen/GenBase.cc
  gen/MultiGenBase.cc
  gen/MultiSetGenBase.cc
  )

set ( itvl_SOURCES
  itvl/ItvlMgr.cc
  itvl/ItvlTree.cc
  itvl/NameMgr.cc
  )

set ( json_SOURCES
  json/JsonValue.cc
  json/JsonObj.cc
  json/JsonParser.cc
  json/JsonScanner.cc
  )

set ( misc_SOURCES
  misc/ReadLine.cc
  misc/UnionFindSet.cc
  )

set ( msg_SOURCES
  msg/MsgHandler.cc
  msg/MsgMgr.cc
  msg/MsgMgrImpl.cc
  )

if ( POPT_FOUND )
  set ( popt_SOURCES
    popt/PoptMainApp.cc
    )
else ( )
  set ( popt_SOURCES )
endif ( )

set ( textproc_SOURCES
  textproc/OptionParser.cc
  textproc/Scanner.cc
  textproc/ShString.cc
  textproc/StrBuff.cc
  )

set ( ym_base_SOURCES
  ${binder_SOURCES}
  ${binio_SOURCES}
  ${file_info_SOURCES}
  ${gen_SOURCES}
  ${itvl_SOURCES}
  ${json_SOURCES}
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
