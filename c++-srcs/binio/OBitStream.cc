
/// @file OBitStream.cc
/// @brief OBitStream の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/OBitStream.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
OBitStream::OBitStream(
  ostream& s
) : mS{s}
{
  mByteOffset = 0;
  mBitOffset = 0;
  memset(reinterpret_cast<void*>(mBuff), 0, BUFF_SIZE);
}

// @brief デストラクタ
OBitStream::~OBitStream()
{
  flush();
}

// @brief ブール値(1ビット)出力
void
OBitStream::write_bool(
  bool val
)
{
  mBuff[mByteOffset] |= (1 << mBitOffset);
  ++ mBitOffset;
  if ( mBitOffset == 8 ) {
    mBitOffset = 0;
    next_byte();
  }
}

// @brief 8ビット値の出力
void
OBitStream::write_8(
  ymuint8 val
)
{
  ymuint8 val1 = (val << mBitOffset) & 0xFF;
  mBuff[mByteOffset] |= val1;
  next_byte();
  if ( mBitOffset > 0 ) {
    ymuint8 val2 = val >> (8 - mBitOffset);
    mBuff[mByteOffset] = val2;
  }
}

// @brief 16ビット値の出力
void
OBitStream::write_16(
  ymuint16 val
)
{
  ymuint8 val1 = (val << mBitOffset) & 0xFF;
  mBuff[mByteOffset] |= val1;
  next_byte();
  ymuint8 val2 = (val >> (8 - mBitOffset)) & 0xFF;
  mBuff[mByteOffset] = val2;
  next_byte();
  if ( mByteOffset > 0 ) {
    ymuint8 val3 = (val >> (16 - mBitOffset));
    mBuff[mByteOffset] = val3;
  }
}

// @brief 32ビット値の出力
void
OBitStream::write_32(
  ymuint32 val ///< [in] 値
)
{
  ymuint8 val1 = (val << mBitOffset) & 0xFF;
  mBuff[mByteOffset] |= val1;
  next_byte();
  ymuint8 val2 = (val >> (8 - mBitOffset)) & 0xFF;
  mBuff[mByteOffset] = val2;
  next_byte();
  ymuint8 val3 = (val >> (16 - mBitOffset));
  mBuff[mByteOffset] = val3;
  next_byte();

}

// @brief バッファの内容を書き出す．
void
OBitStream::flush()
{
  SizeType nb = mByteOffset;
  if ( mBitOffset > 0 ) {
    ++ nb;
  }
  if ( nb > 0 ) {
    s.write(reinterpret_cast<char*>(mBuff), nb);
  }
  mByteOffset = 0;
  mBitOffset = 0;
  memset(reinterpret_cast<void*>(mBuff), 0, BUFF_SIZE);
}

/// @brief 1バイト進める．
void
OBitStream::next_byte()
{
  ++ mByteOffset;
  if ( mByteOffset == BUFF_SIZE ) {
    flush();
  }
}

END_NAMESPACE_YM
