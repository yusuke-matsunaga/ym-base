﻿#ifndef STRPOOL_H
#define STRPOOL_H

/// @file StrPool.h
/// @brief StrPool のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrPool StrPool.h "StrPool.h"
/// @ingroup ym
/// @brief 文字列を共有するためのプール
///
/// メンバ関数 reg() で文字列を登録する．
/// 文字列へのポインタが返ってくるが，これはこのオブジェクト内で共有化
/// されている．
///
/// このクラスではいったん登録した文字列を削除する方法はない．
//////////////////////////////////////////////////////////////////////
class StrPool
{
public:

  /// @brief コンストラクタ
  StrPool();

  /// @brief デストラクタ
  /// @note このオブジェクトが管理しているすべてのページが解放される．
  ~StrPool();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を登録する．
  /// @param[in] str 入力となる文字列
  /// @return カノニカライズされた文字列を返す．
  const char*
  reg(const char* str);

  /// @brief 確保した文字列領域の総量を得る．
  /// @return 確保した文字列領域の総量を得る．
  ///
  /// デバッグ/解析用 -- 通常は使わない．
  SizeType
  accum_alloc_size() const;

  /// @brief メモリを全部開放する．
  ///
  /// 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
  void
  destroy();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ハッシュ関数
  struct Hash
  {
    Hash() = default;

    ~Hash() = default;

    SizeType
    operator()(const char* s) const;

  };

  // 等価比較関数
  struct Eq
  {
    Eq() = default;

    ~Eq() = default;

    bool
    operator()(const char* s1,
	       const char* s2) const;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列のハッシュ表
  unordered_set<const char*, Hash, Eq> mStrHash;

  // 文字列用に確保されたメモリサイズの総和
  SizeType mTotalAllocSize;

  // 文字列のリスト
  vector<const char*> mBlockList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// ハッシュ関数
inline
SizeType
StrPool::Hash::operator()(const char* s) const
{
  SizeType h = 0;
  char c;
  while ( (c = *s ++) ) {
    h = h * 33 + static_cast<SizeType>(c);
  }
  return h;
}

// 等価比較関数
inline
bool
StrPool::Eq::operator()(const char* s1,
			const char* s2) const
{
  return strcmp(s1, s2) == 0;
}

// コンストラクタ
inline
StrPool::StrPool() :
  mTotalAllocSize{0}
{
}

// デストラクタ
inline
StrPool::~StrPool()
{
  destroy();
}

// 文字列を登録する．
inline
const char*
StrPool::reg(const char* str)
{
  // まず str と同一の文字列が登録されていないか調べる．
  auto p = mStrHash.find(str);
  if ( p != mStrHash.end() ) {
    return *p;
  }

  // なければ新しい文字列を登録する．
  SizeType l = strlen(str) + 1;
  char* s = new char[l];
  memcpy(s, str, l);
  mStrHash.emplace(s);
  mTotalAllocSize += l;
  mBlockList.push_back(s);

  return s;
}

// 確保した文字列領域の総量を得る．
inline
SizeType
StrPool::accum_alloc_size() const
{
  return mTotalAllocSize;
}

// @brief メモリを全部開放する．
//
// 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
inline
void
StrPool::destroy()
{
  for ( auto p: mBlockList ) {
    delete [] p;
  }
  mBlockList.clear();
  mStrHash.clear();
}

END_NAMESPACE_YM

#endif // STRPOOL_H
