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
include (FindBZip2)
include (FindLibLZMA)
include (FindZLIB)

include (FindPOPT)


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

include ( ym_gen_header )
ym_gen_header ()


# ===================================================================
# インクルードパスの設定
# ===================================================================

if (ZLIB_FOUND)
  include_directories(${ZLIBINCLUDE_DIRS})
endif (ZLIB_FOUND)

if (BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIRS})
endif (BZIP2_FOUND)

if (LIBLZMA_FOUND)
  include_directories(${LIBLZMA_INCLUDE_DIRS})
endif (LIBLZMA_FOUND)

if (POPT_FOUND)
  include_directories(${POPT_INCLUDE_DIR})
endif (POPT_FOUND)


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( cython-src )


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

set ( ALLOC_SOURCES
  src/alloc/Alloc.cc
  src/alloc/FragAlloc.cc
  src/alloc/SimpleAlloc.cc
  src/alloc/UnitAlloc.cc
  )

set ( BINDER_SOURCES
  src/binder/Binder.cc
  )

set ( FILE_INFO_SOURCES
  src/file_info/FileInfo.cc
  src/file_info/FileInfoMgr.cc
  )

set ( IO_SOURCES
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

set ( MSG_SOURCES
  src/msg/MsgHandler.cc
  src/msg/MsgMgr.cc
  src/msg/MsgMgrImpl.cc
  )

if ( POPT_FOUND )
  set ( POPT_SOURCES
    src/popt/PoptMainApp.cc
    )
else ( POPT_FOUND )
  set ( POPT_SOURCES )
endif ( POPT_FOUND )

set ( RANDOM_SOURCES
  src/random/RandGen.cc
  src/random/RandSampler.cc
  )

set ( TEXTPROC_SOURCES
  src/textproc/Scanner.cc
  src/textproc/StrBuff.cc
  src/textproc/StrPool.cc
  )

set ( TIME_SOURCES
  src/time/StopWatch.cc
  )


set (YM_COMMON_SOURCES
  ${ALLOC_SOURCES}
  ${BINDER_SOURCES}
  ${IO_SOURCES}
  ${MSG_SOURCES}
  ${POPT_SOURCES}
  ${RANDOM_SOURCES}
  ${TEXTPROC_SOURCES}
  ${TIME_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
ym_add_library (ym_common
  SHARED
  ${YM_COMMON_SOURCES}
  DESTINATION lib
  )
