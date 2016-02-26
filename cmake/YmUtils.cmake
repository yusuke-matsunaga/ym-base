# いくつかのマクロ，関数を集めたファイル

# ym_init: ym-common の初期化を行うマクロ
#
# 入力: "USE_BZIP2" bzip2 を使うとき指定する．
#       "USE_LZMA"  lzma を使うとき指定する．
#       "USE_ZLIB"  gzip を使うとき指定する．
#       "USE_POPT"  popt を使うとき指定する．
#
# 出力: YM_UNIX     unix 系のプラットフォームの時セットされる．
#       YM_APPLE    mac 系のプラットフォームの時セットされる．
#       YM_WIN32    windows 系のプラットフォームの時セットされる．
#       BZIP2_FOUND bzip2 が使用可能の時セットされる．
#       LZMA_FOUND  lzma が使用可能の時セットされる．
#       ZLIB_FOUND  gzip が使用可能の時セットされる．
#       POPT_FOUND  popt が使用可能の時セットされる．
macro ( ym_init )

  # bzip2 ライブラリを使う．
  set ( USE_BZIP2 OFF )

  # LZMA ライブラリを使う．
  set ( USE_LZMA OFF )

  # gzip ライブラリを使う．
  set ( USE_ZLIB OFF )

  # popt ライブラリを使う．
  set ( USE_POPT OFF )

  foreach ( ym_pos RANGE 0 ${ARGC} )
    if ( ${ym_pos} EQUAL ${ARGC} )
      break()
    endif ()
    list ( GET ARGV ${ym_pos} ym_argv )
    if ( ${ym_argv} STREQUAL "USE_BZIP2" )
      set ( USE_BZIP2 ON )
    elseif ( ${ym_argv} STREQUAL "USE_LZMA" )
      set ( USE_LZMA ON )
    elseif ( ${ym_argv} STREQUAL "USE_ZLIB" )
      set ( USE_ZLIB ON )
    elseif ( ${ym_argv} STREQUAL "USE_POPT" )
      set ( USE_POPT ON )
    else ()
      # エラー
      message ( FATAL_ERROR
	"illegal argument in ym_init()" )
    endif ()
  endforeach ()

  # ===================================================================
  # システムの検査
  # ===================================================================

  if ( UNIX )
    set ( YM_UNIX 1 )
  endif ( UNIX )
  if ( APPLE )
    set ( YM_APPLE 1 )
  endif ( APPLE )
  if ( WIN32 )
    set ( YM_WIN32 1 )
  endif ( WIN32 )


  # ===================================================================
  # パッケージの検査
  # ===================================================================

  if ( USE_BZIP2 )
    include (FindBZip2)
  endif ()

  if ( USE_LZMA )
    include (FindLibLZMA)
  endif ()

  if ( USE_ZLIB )
    include (FindZLIB)
  endif ()

  if ( USE_POPT )
    include (FindPOPT)
  endif ()


  # ===================================================================
  # リンクすべきライブラリの設定
  # ===================================================================

  set ( YM_LIB_DEPENDS )

  if ( ZLIB_FOUND )
    list (APPEND YM_LIB_DEPENDS ${ZLIB_LIBRARIES})
  endif ()

  if ( BZIP2_FOUND )
    list (APPEND YM_LIB_DEPENDS ${BZIP2_LIBRARIES})
  endif ()

  if ( LIBLZMA_FOUND )
    list (APPEND YM_LIB_DEPENDS ${LIBLZMA_LIBRARIES})
  endif ()

  if ( POPT_FOUND )
    list (APPEND YM_LIB_DEPENDS ${POPT_LIBRARY})
  endif ()

endmacro ()


# ym_use_gtest: ym-common に内蔵の gtest を使うためのマクロ
#
# 入力: なし
# 出力: GTEST_INCLUDE_DIR    gtest 用のインクルードディレクトリ
#       GTEST_BOTH_LIBRARIES gtest 用のライブラリ
macro ( ym_use_gtest )
  add_subdirectory (ym-common/gtest-1.7.0)
  set (GTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/ym-common/gtest-1.7.0/include")
  set (GTEST_BOTH_LIBRARIES
    "${PROJECT_BINARY_DIR}/ym-common/gtest-1.7.0/libgtest.a"
    "${PROJECT_BINARY_DIR}/ym-common/gtest-1.7.0/libgtest_main.a")
endmacro ()


# ヘッダファイルを生成するためのマクロ
#
# USAGE: ym_gen_header ()
function ( ym_gen_header )
  # ===================================================================
  # ヘッダファイルの検査
  # ===================================================================
  include (CheckIncludeFileCXX)
  check_include_file_cxx (stdlib.h    YM_HAVE_STDLIB_H)
  check_include_file_cxx (string.h    YM_HAVE_STRING_H)
  check_include_file_cxx (unistd.h    YM_HAVE_UNISTD_H)
  check_include_file_cxx (limits.h    YM_HAVE_LIMITS_H)
  check_include_file_cxx (float.h     YM_HAVE_FLOAT_H)
  check_include_file_cxx (math.h      YM_HAVE_MATH_H)
  check_include_file_cxx (sys/time.h  YM_HAVE_SYS_TIME_H)
  check_include_file_cxx (time.h      YM_HAVE_TIME_H)
  check_include_file_cxx (sys/stat.h  YM_HAVE_SYS_STAT_H)
  check_include_file_cxx (pwd.h       YM_HAVE_PWD_H)
  check_include_file_cxx (sys/param.h YM_HAVE_SYS_PARAM_H)

  if ( YM_HAVE_STDLIB_H )
    set ( INCLUDE_STDLIB "#include <stdlib.h>" )
  else ()
    set ( INCLUDE_STDLIB )
  endif ()

  if ( YM_HAVE_STRING_H )
    set ( INCLUDE_STRING "#include <string.h>" )
  else ()
    set ( INCLUDE_STRING )
  endif ()

  if ( YM_HAVE_UNISTD_H )
    set ( INCLUDE_UNISTD "#include <unistd.h>" )
  else ()
    set ( INCLUDE_UNISTD )
  endif ()

  if ( YM_HAVE_LIMITS_H )
    set ( INCLUDE_LIMITS "#include <limits.h>" )
  else ()
    set ( INCLUDE_LIMITS )
  endif ()

  if ( YM_HAVE_FLOAT_H )
    set ( INCLUDE_FLOAT "#include <float.h>" )
  else ()
    set ( INCLUDE_FLOAT )
  endif ()

  if ( YM_HAVE_MATH_H )
    set ( INCLUDE_MATH "#include <math.h>" )
  else ()
    set ( INCLUDE_MATH )
  endif ()


  # ===================================================================
  # ライブラリ関数の検査
  # ===================================================================
  include (CheckFunctionExists)
  check_function_exists (getrusage    YM_HAVE_GETRUSAGE)
  check_function_exists (times        YM_HAVE_TIMES)


  # ===================================================================
  # 組み込み型のサイズの検査
  # ===================================================================
  include (CheckTypeSize)
  check_type_size (short       SHORT_SIZE)
  check_type_size (int         INT_SIZE)
  check_type_size (long        LONG_SIZE)
  check_type_size ("long long" LLONG_SIZE)
  check_type_size ("void *"    VOIDP_SIZE)

  if ( SHORT_SIZE EQUAL 2 )
    set( INT16_TYPE short )
    set( UINT16_TYPE "unsigned short" )
  elseif ( INT_SIZE EQUAL 2 )
    set( INT16_TYPE int )
    set( UINT16_TYPE "unsigned int" )
  else ()
    # エラー
    message ( FATAL_ERROR
      "No valid type for 16-bit integer" )
  endif ()

  if ( SHORT_SIZE EQUAL 4 )
    set( INT32_TYPE short )
    set( UINT32_TYPE "unsigned short" )
  elseif ( INT_SIZE EQUAL 4 )
    set( INT32_TYPE int )
    set( UINT32_TYPE "unsigned int" )
  elseif ( LONG_SIZE EQUAL 4 )
    set( INT32_TYPE long )
    set( UINT32_TYPE "unsigned long" )
  else ()
    # エラー
    message ( FATAL_ERROR
      "No valid type for 32-bit integer" )
  endif ()

  if ( INT_SIZE EQUAL 8 )
    set( INT64_TYPE int )
    set( UINT64_TYPE "unsigned int" )
  elseif ( LONG_SIZE EQUAL 8 )
    set( INT64_TYPE long )
    set( UINT64_TYPE "unsigned long" )
  elseif ( LLONG_SIZE EQUAL 8 )
    set( INT64_TYPE "lon long" )
    set( UINT64_TYPE "unsigned long long" )
  else ()
    # エラー
    message ( FATAL_ERROR
      "No valid type for 64-bit integer" )
  endif ()

  if ( INT_SIZE EQUAL VOIDP_SIZE )
    set( PTRINT_TYPE "unsigned int" )
  elseif ( LONG_SIZE EQUAL VOIDP_SIZE )
    set( PTRINT_TYPE "unsigned long" )
  elseif ( LLONG_SIZE EQUAL VOIDP_SIZE )
    set( PTRINT_TYPE "unsigned long long" )
  else ()
    # エラー
    message ( FATAL_ERROR
      "No valid type for pointer size integer" )
  endif ()

  # ヘッダファイルの生成
  configure_file(
    "${PROJECT_SOURCE_DIR}/ym-common/etc/ym_config.h.in"
    "${PROJECT_BINARY_DIR}/ym_config.h"
    )

endfunction()


# release, relwithdeb, debug ターゲットライブラリの設定
#
# USAGE: ym_add_object_library ( <target-name>
#                         <source-file> [<source-file>]
#                       )
function ( ym_add_object_library )
  foreach ( pos RANGE 0 ${ARGC} )
    if ( ${pos} EQUAL ${ARGC} )
      break()
    endif ()
    list (GET ARGV ${pos} argv)
    if ( ${pos} EQUAL 0 )
      # ターゲット名の設定
      # - 最適化モード
      # - 最適化モード(アーカイブタイプ _a)
      # - プロファイルモード (_p)
      # - デバッグモード (_d)
      # の4つがある．
      set (_target_name   "${argv}")
      set (_target_name_a "${argv}_a")
      set (_target_name_p "${argv}_p")
      set (_target_name_d "${argv}_d")
    else ()
      list (APPEND _sources ${argv})
    endif ()
  endforeach ()

  # release モードのターゲット
  add_library (${_target_name}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name}
    PRIVATE "-O3" "-fPIC"
    )

  target_compile_definitions (${_target_name}
    PRIVATE "-DNDEBUG"
    )

  # release:archive モードのターゲット
  add_library (${_target_name_a}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name_a}
    PRIVATE "-O3"
    )

  target_compile_definitions (${_target_name_a}
    PRIVATE "-DNDEBUG"
    )

  # release with debug info モードのターゲット
  add_library (${_target_name_p}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name_p}
    PRIVATE "-O2" "-g" "-fPIC"
    )

  target_compile_definitions (${_target_name_p}
    PRIVATE "-DNDEBUG"
    )

  # debug モードのターゲット
  # 常に static にする．
  add_library (${_target_name_d}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name_d}
    PRIVATE "-g"
    )

  target_compile_definitions (${_target_name_d}
    PRIVATE
    )

endfunction ()
