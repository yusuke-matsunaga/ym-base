
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
# オプション
# ===================================================================

# bzip2 ライブラリを使う．
option ( USE_BZIP2 "use bzip2" ON )

# LZMA ライブラリを使う．
option ( USE_LZMA "use lzma" ON )

# gzip ライブラリを使う．
option ( USE_ZLIB "use zlib(gzip)" ON )

# popt ライブラリを使う．
option ( USE_POPT "use popt" OFF)


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
