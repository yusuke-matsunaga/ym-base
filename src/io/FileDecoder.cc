
/// @file FileDecoder.cc
/// @brief FileDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "CodecTypeConf.h"
#include "FileDecoder.h"
#include "ThruDecoder.h"
#include "ZDecoder.h"
#include "ym/MsgMgr.h"

#if defined(ZLIB_FOUND)
#include "GzDecoder.h"
#endif

#if defined(BZIP2_FOUND)
#include "BzDecoder.h"
#endif

#if defined(LIBLZMA_FOUND)
#include "LzmaDecoder.h"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileDecoder
//////////////////////////////////////////////////////////////////////

// @brief FileDecoder のインスタンスを作る関数
// @param[in] type FileDecoder の型
// @return 作成したインスタンスを返す．
// @note type が不正な値の場合は nullptr を返す．
FileDecoder*
FileDecoder::new_decoder(CodecType type)
{
  FileDecoder* decoder = nullptr;

  if ( type == CodecType::Through ) {
    decoder = new ThruDecoder();
  }
  else if ( type == CodecType::Z ) {
    decoder = new ZDecoder();
  }
  else if ( type == CodecType::Gzip ) {
#if defined(ZLIB_FOUND)
    decoder = new GzDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "FileDecoder",
		    "gzip format is not supported on this system");
#endif
  }
  else if ( type == CodecType::Bzip2 ) {
#if defined(BZIP2_FOUND)
    decoder = new BzDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "FileDecoder",
		    "bzip2 format is not supported on this system");
#endif
  }
  else if ( type == CodecType::Lzma ) {
#if defined(LIBLZMA_FOUND)
    decoder = new LzmaDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "FileCoder",
		    "lzma(xz) format is not supported on this system");
#endif
  }


  return decoder;
}

// @brief thru decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_thru_decoder()
{
  return new ThruDecoder();
}

// @brief compress decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_z_decoder()
{
  return new ZDecoder();
}

// @brief gzip decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_gzip_decoder()
{
#if defined(ZLIB_FOUND)
  return new GzDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "FileDecoder",
		  "gzip format is not supported on this system");
  return nullptr;
#endif
}

// @brief bzip2 coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_bzip2_decoder()
{
#if defined(BZIP2_FOUND)
  return new BzDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "FileDecoder",
		  "bzip2 format is not supported on this system");
  return nullptr;
#endif
}

// @brief xz coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_xz_decoder()
{
#if defined(LIBLZMA_FOUND)
  return new LzmaDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "FileDecoder",
		  "xz format is not supported on this system");
  return nullptr;
#endif
}

END_NAMESPACE_YM
