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

include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

if ( ZLIB_FOUND )
  include_directories(${ZLIBINCLUDE_DIRS})
endif ()

if ( BZIP2_FOUND )
  include_directories(${BZIP2_INCLUDE_DIRS})
endif ()

if ( LIBLZMA_FOUND )
  include_directories(${LIBLZMA_INCLUDE_DIRS})
endif ()

if ( POPT_FOUND )
  include_directories(${POPT_INCLUDE_DIR})
endif ()


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( c++-test )
add_subdirectory ( tests )


# ===================================================================
# コンフィグレーション依存のファイルの生成
# ===================================================================
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/etc/CodecTypeConf.h.in"
  "${PROJECT_BINARY_DIR}/include/CodecTypeConf.h"
  )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( binder_SOURCES
  c++-srcs/binder/Binder.cc
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

set ( io_SOURCES
  c++-srcs/io/FileBuff.cc
  c++-srcs/io/FileBuff.h
  c++-srcs/io/ZFile.cc
  )

if ( ZLIB_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-srcs/io/GzFile.cc
    )
endif ( ZLIB_FOUND )

if ( BZIP2_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-srcs/io/BzFile.cc
    )
endif ( BZIP2_FOUND )

if ( LIBLZMA_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-srcs/io/XzFile.cc
    )
endif ( LIBLZMA_FOUND )

set ( itvl_SOURCES
  c++-srcs/itvl/ItvlMgr.cc
  c++-srcs/itvl/ItvlTree.cc
  c++-srcs/itvl/NameMgr.cc
  )

set ( misc_SOURCES
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

set ( time_SOURCES
  c++-srcs/time/StopWatch.cc
  )

set ( ym_base_SOURCES
  ${binder_SOURCES}
  ${file_info_SOURCES}
  ${gen_SOURCES}
  ${io_SOURCES}
  ${itvl_SOURCES}
  ${misc_SOURCES}
  ${msg_SOURCES}
  ${popt_SOURCES}
  ${textproc_SOURCES}
  ${time_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_base
  ${ym_base_SOURCES}
  )
