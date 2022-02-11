
/// @file CodecEngine.cc
/// @brief CodecEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CodecEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CodecEngine
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のコンストラクタ
CodecEngine::CodecEngine(
  istream* is,
  SizeType buff_size
) : mInStream{is},
    mBuffSize{buff_size},
    mBuff{new ymuint8[mBuffSize]}
{
  read_fill();
}

// @brief 圧縮用のコンストラクタ
CodecEngine::CodecEngine(
  ostream* os,
  SizeType buff_size
) : mOutStream{os},
    mBuffSize{buff_size},
    mBuff{new ymuint8[mBuffSize]}
{
}

// @brief デストラクタ
CodecEngine::~CodecEngine()
{
  if ( write_mode() ) {
    write_flush();
  }
  delete [] mBuff;
}

// @brief 生のブロックデータを読み出す．
SizeType
CodecEngine::raw_read(
  ymuint8* buff, ///< [in] データバッファ
  SizeType size  ///< [in] 読み出すデータサイズ
)
{
  ASSERT_COND( read_mode() );

  // 実際に読み出したサイズ
  SizeType count = 0;
  while ( size > 0 && read_size() > 0 ) {
    // バッファから読み出せるサイズを求める．
    SizeType size1 = std::min(size, read_size());
    if ( buff != nullptr ) {
      // buff に転送する．
      bcopy(reinterpret_cast<const void*>(read_ptr()),
	    reinterpret_cast<void*>(buff),
	    size1);
      buff += size1;
    }
    // 諸元を更新する．
    count += size1;
    size -= size1;
    read_seek(size1);
  }

  return count;
}

// @brief 生のデータを書き込む．
void
CodecEngine::raw_write(
  const ymuint8* buff, ///< [in] データバッファ
  SizeType size        ///< [in] データサイズ
)
{
  ASSERT_COND( write_mode() );

  while ( size > 0 ) {
    // バッファに書き込めるサイズ
    SizeType size1 = std::min(size, write_size());
    // バッファに転送する．
    bcopy(reinterpret_cast<const void*>(buff),
	  reinterpret_cast<void*>(write_ptr()),
	  size1);
    // 諸元を更新する．
    buff += size1;
    size -= size1;
    write_seek(size1);
  }
}

END_NAMESPACE_YM
