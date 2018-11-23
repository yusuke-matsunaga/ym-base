﻿
/// @file StrBuff.cc
/// @brief StrBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM

// 文字列を追加する．
void
StrBuff::put_str(const char* str)
{
  if ( str ) {
    SizeType len = strlen(str) + mEnd;
    if ( len >= mSize ) {
      SizeType new_size = mSize << 1;
      while ( len >= new_size ) {
	new_size <<= 1;
      }
      expand(new_size);
    }
    copy(str, mBuffer + mEnd);
    mEnd = len;
  }
}

// @brief 整数を文字列に変換して追加
void
StrBuff::put_digit(int d)
{
  if ( d >= 10 ) {
    put_digit(d / 10);
    d = d % 10;
  }
  put_char(static_cast<char>('0' + d));
}

// c が最初に現れる位置を返す．
SizeType
StrBuff::find_first_of(char c) const
{
  for ( SizeType i = 0; i < mEnd; ++ i ) {
    if ( mBuffer[i] == c ) return i;
  }
  return npos;
}

// first から last までの部分文字列を切り出す．
StrBuff
StrBuff::substr(SizeType first,
		SizeType last) const
{
  if ( last == npos ) {
    last = mEnd;
  }
  SizeType len = mEnd - first;
  StrBuff ans(len + 1);
  const char* src = mBuffer + first;
  char* dst = ans.mBuffer;
  char* dst_end = dst + len;
  for ( ; dst != dst_end; ++ dst, ++ src ) {
    *dst = *src;
  }
  *dst = '\0';
  ans.mEnd = len;
  return ans;
}

// バッファサイズを拡張する．
void
StrBuff::expand(SizeType new_size)
{
  mSize = new_size;
  char* old = mBuffer;
  mBuffer = new char[new_size];
  copy(old, mBuffer);
  delete [] old;
}

END_NAMESPACE_YM
