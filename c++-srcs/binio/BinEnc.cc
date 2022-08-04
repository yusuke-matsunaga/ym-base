
/// @file BinEnc.cc
/// @brief BinEnc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM

// @brief 可変長の整数の書き込み
void
BinEnc::write_vint(
  SizeType val
)
{
  if ( val <= 127 ) {
    write_8(static_cast<ymuint8>(val));
  }
  else {
    SizeType val1 = (val & 127) | 128;
    write_8(static_cast<ymuint8>(val1));
    write_vint((val >> 7));
  }
}

// @brief 可変長の整数の読み出し
SizeType
BinDec::read_vint()
{
  SizeType val = 0;
  for ( SizeType weight = 1; ; weight *= 128 ) {
    SizeType c = read_8();
    val += (c & 127) * weight;
    if ( (c & 128) == 0 ) {
      break;
    }
  }
  return val;
}

END_NAMESPACE_YM
