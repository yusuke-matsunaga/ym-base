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