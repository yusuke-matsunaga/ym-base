#ifndef SPLIT_H
#define SPLIT_H

/// @file split.h
/// @brief split のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief 文字列をデリミタで区切る
/// @return 区切られた文字列のリストを返す．
inline
vector<string>
split(
  const string& src_str,     ///< [in] 元の文字列
  const string& delim = " "  ///< [in] デリミタ
)
{
  vector<string> ans;
  string::size_type start = 0;
  for ( ; ; ) {
    // start 以降で delim 中の文字とマッチする位置を探す．
    auto p = src_str.find_first_of(delim, start);
    auto n = p - start;
    if ( n > 0 ) {
      auto tmp = src_str.substr(start, n);
      ans.push_back(tmp);
    }
    if ( p == string::npos ) {
      // 終端に達した．
      break;
    }
    start = p + 1;
  }
  return ans;
}

END_NAMESPACE_YM

#endif // SPLIT_H
