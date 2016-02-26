# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================

# 名前空間の定義
set ( YM_NAMESPACE "nsYm" )


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

ym_gen_header ()


# ===================================================================
# インクルードパスの設定
# ===================================================================

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

#add_subdirectory ( cython-src )


# ===================================================================
# コンフィグレーション依存のファイルの生成
# ===================================================================
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/etc/CodecTypeConf.h.in"
  "${PROJECT_BINARY_DIR}/CodecTypeConf.h"
  )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( alloc_SOURCES
  src/alloc/Alloc.cc
  src/alloc/FragAlloc.cc
  src/alloc/SimpleAlloc.cc
  src/alloc/UnitAlloc.cc
  )

set ( binder_SOURCES
  src/binder/Binder.cc
  )

set ( file_info_SOURCES
  src/file_info/File.cc
  src/file_info/FileInfo.cc
  src/file_info/FileInfoMgr.cc
  )

set ( gen_SOURCES
  src/gen/CombiGen.cc
  src/gen/GenBase.cc
  src/gen/MultiCombiGen.cc
  src/gen/MultiGenBase.cc
  src/gen/MultiPermGen.cc
  src/gen/MultiSetCombiGen.cc
  src/gen/MultiSetGenBase.cc
  src/gen/MultiSetPermGen.cc
  src/gen/PermGen.cc
  )

set ( io_SOURCES
  src/io/CodecType.cc
  src/io/FileBuff.cc
  src/io/FileBuff.h
  src/io/FileCoder.cc
  src/io/FileDecoder.cc
  src/io/FileIDO.cc
  src/io/FileODO.cc
  src/io/IDO.cc
  src/io/ODO.cc
  src/io/StreamIDO.cc
  src/io/StringIDO.cc
  src/io/StrListIDO.cc
  src/io/ThruCoder.cc
  src/io/ThruDecoder.cc
  src/io/ZCoder.cc
  src/io/ZDecoder.cc
  )

if ( ZLIB_FOUND )
  set (io_SOURCES ${io_SOURCES}
    src/io/GzCoder.cc
    src/io/GzDecoder.cc
    src/io/zstream.cc
    )
endif ( ZLIB_FOUND )

if ( BZIP2_FOUND )
  set (io_SOURCES ${io_SOURCES}
    src/io/BzCoder.cc
    src/io/BzDecoder.cc
    )
endif ( BZIP2_FOUND )

if ( LIBLZMA_FOUND )
  set (io_SOURCES ${io_SOURCES}
    src/io/LzmaCoder.cc
    src/io/LzmaDecoder.cc
    )
endif ( LIBLZMA_FOUND )

set ( itvl_SOURCES
  src/itvl/ItvlMgr.cc
  src/itvl/ItvlMgrImpl.cc
  src/itvl/NameMgr.cc
  )

set ( misc_SOURCES
  src/misc/MFSet.cc
  )

set ( msg_SOURCES
  src/msg/MsgHandler.cc
  src/msg/MsgMgr.cc
  src/msg/MsgMgrImpl.cc
  )

if ( POPT_FOUND )
  set ( popt_SOURCES
    src/popt/PoptMainApp.cc
    )
else ( )
  set ( popt_SOURCES )
endif ( )

set ( random_SOURCES
  src/random/RandGen.cc
  src/random/RandSampler.cc
  )

set ( textproc_SOURCES
  src/textproc/Scanner.cc
  src/textproc/StrBuff.cc
  src/textproc/StrPool.cc
  )

set ( time_SOURCES
  src/time/StopWatch.cc
  )

set ( ym_common_SOURCES
  ${alloc_SOURCES}
  ${binder_SOURCES}
  ${file_info_SOURCES}
  ${gen_SOURCES}
  ${io_SOURCES}
  ${itvl_SOURCES}
  ${misc_SOURCES}
  ${msg_SOURCES}
  ${popt_SOURCES}
  ${random_SOURCES}
  ${textproc_SOURCES}
  ${time_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_common
  ${ym_common_SOURCES}
  )
