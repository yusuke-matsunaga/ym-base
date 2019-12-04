﻿
/// @file StrPool.cc
/// @brief StrPool と ShString の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ShString.h"
#include "StrPool.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// ShString
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

StrPool thePool;

END_NONAMESPACE

// 共有文字列を作ってセットする．
void
ShString::set(const char* str)
{
  mPtr = thePool.reg(str);
}

// @brief ShString 関連でアロケートされたメモリサイズ
SizeType
ShString::allocated_size()
{
  return thePool.accum_alloc_size();
}

// @brief ShString 関連でアロケートされたメモリをすべて開放する．
// @note 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
void
ShString::free_all_memory()
{
  thePool.destroy();
}

// ShString 用ストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const ShString& str)
{
  // ちょっと見にくいけど ShString から const char* への変換演算子
  const char* p = str.operator const char*();
  if ( p ) {
    return s << p;
  }
  else {
    return s << "";
  }
}

END_NAMESPACE_YM
