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
  "${PROJECT_BINARY_DIR}/CodecTypeConf.h"
  )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( alloc_SOURCES
  c++-src/alloc/Alloc.cc
  c++-src/alloc/FragAlloc.cc
  c++-src/alloc/SimpleAlloc.cc
  c++-src/alloc/UnitAlloc.cc
  )

set ( binder_SOURCES
  c++-src/binder/Binder.cc
  )

set ( file_info_SOURCES
  c++-src/file_info/File.cc
  c++-src/file_info/FileInfo.cc
  c++-src/file_info/FileInfoMgr.cc
  )

set ( gen_SOURCES
  c++-src/gen/CombiGen.cc
  c++-src/gen/GenBase.cc
  c++-src/gen/MultiCombiGen.cc
  c++-src/gen/MultiGenBase.cc
  c++-src/gen/MultiPermGen.cc
  c++-src/gen/MultiSetCombiGen.cc
  c++-src/gen/MultiSetGenBase.cc
  c++-src/gen/MultiSetPermGen.cc
  c++-src/gen/PermGen.cc
  )

set ( io_SOURCES
  c++-src/io/CodecType.cc
  c++-src/io/FileBuff.cc
  c++-src/io/FileBuff.h
  c++-src/io/FileCoder.cc
  c++-src/io/FileDecoder.cc
  c++-src/io/FileIDO.cc
  c++-src/io/FileODO.cc
  c++-src/io/IDO.cc
  c++-src/io/ODO.cc
  c++-src/io/StreamIDO.cc
  c++-src/io/StringIDO.cc
  c++-src/io/StrListIDO.cc
  c++-src/io/ThruCoder.cc
  c++-src/io/ThruDecoder.cc
  c++-src/io/ZCoder.cc
  c++-src/io/ZDecoder.cc
  )

if ( ZLIB_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-src/io/GzCoder.cc
    c++-src/io/GzDecoder.cc
    c++-src/io/zstream.cc
    )
endif ( ZLIB_FOUND )

if ( BZIP2_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-src/io/BzCoder.cc
    c++-src/io/BzDecoder.cc
    )
endif ( BZIP2_FOUND )

if ( LIBLZMA_FOUND )
  set (io_SOURCES ${io_SOURCES}
    c++-src/io/LzmaCoder.cc
    c++-src/io/LzmaDecoder.cc
    )
endif ( LIBLZMA_FOUND )

set ( itvl_SOURCES
  c++-src/itvl/ItvlMgr.cc
  c++-src/itvl/ItvlMgrImpl.cc
  c++-src/itvl/NameMgr.cc
  )

set ( misc_SOURCES
  c++-src/misc/MFSet.cc
  )

set ( msg_SOURCES
  c++-src/msg/MsgHandler.cc
  c++-src/msg/MsgMgr.cc
  c++-src/msg/MsgMgrImpl.cc
  )

if ( POPT_FOUND )
  set ( popt_SOURCES
    c++-src/popt/PoptMainApp.cc
    )
else ( )
  set ( popt_SOURCES )
endif ( )

set ( random_SOURCES
  c++-src/random/RandGen.cc
  c++-src/random/RandSampler.cc
  c++-src/random/RandSamplerSimple.cc
  )

set ( textproc_SOURCES
  c++-src/textproc/OptionParser.cc
  c++-src/textproc/Scanner.cc
  c++-src/textproc/StrBuff.cc
  c++-src/textproc/StrPool.cc
  )

set ( time_SOURCES
  c++-src/time/StopWatch.cc
  )

set ( ym_base_SOURCES
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

ym_add_object_library ( ym_base
  ${ym_base_SOURCES}
  )
