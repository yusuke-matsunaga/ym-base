#ifndef YM_HASHFUNC_H
#define YM_HASHFUNC_H

/// @file ym/HashFunc.h
/// @brief HaseFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

typedef unsigned int HashType;

//////////////////////////////////////////////////////////////////////
/// @class HashFunc HashFunc.h "ym/HashFunc.h"
/// @brief ハッシュ関数を表すファンクタクラス
///
/// 下のコードを見ればわかるが Key_Type が直接 unsigned int にキャスト
/// できない場合にはデフォルト実装ではエラーとなるので
/// 個別に特殊化する必要がある．
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
struct HashFunc
{
  HashType
  operator()(const Key_Type& key)
  {
    // Key_Type が unsigned int にキャストできなければエラーになる．
    return key;
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<void*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<void*>
{
  HashType
  operator()(void* key) const
  {
    ympuint tmp = reinterpret_cast<ympuint>(key) / sizeof(void*);
    return static_cast<HashType>(tmp);
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<int> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<int>
{
  HashType
  operator()(int key) const
  {
    return static_cast<HashType>(key);
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<unsigned int> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<unsigned int>
{
  HashType
  operator()(unsigned int key) const
  {
    return static_cast<HashType>(key);
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<string> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<string>
{
  HashType
  operator()(const string& key) const
  {
    HashType h = 0;
    for ( int i = 0; i < key.size(); ++ i ) {
      h = h * 33 + static_cast<HashType>(key[i]);
    }
    return h;
  }
};


//////////////////////////////////////////////////////////////////////
// HashFunc<const char*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<const char*>
{
  HashType
  operator()(const char* key) const
  {
    HashType h = 0;
    char c;
    while ( (c = *key ++) ) {
      h = h * 33 + static_cast<HashType>(c);
    }
    return h;
  }
};

END_NAMESPACE_YM

#endif // YM_HASHFUNC_H
