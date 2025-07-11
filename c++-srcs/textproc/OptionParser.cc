
/// @file OptionParser.cc
/// @brief OptionParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/OptionParser.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス OptionParser
//////////////////////////////////////////////////////////////////////

// @grief デリミタを設定する．
void
OptionParser::set_delim(
  char delim,
  char opt_delim
)
{
  mDelim = delim;
  mOptDelim = opt_delim;
}

BEGIN_NONAMESPACE

// string::find_first_of() のバリエーション
// 直前の '\' をエスケープ文字とみなす．
std::string::size_type
find_first_of(
  const std::string& input,
  char c
)
{
  std::string::size_type p = 0;
  std::string::size_type end = input.size();
  for ( ; p < end; ++ p ) {
    char c1 = input[p];
    if ( c1 == c ) {
      return p;
    }
    if ( c1 == '\\' ) {
      if ( p + 1 == end ) {
	// 末尾が \ だった．
	return std::string::npos;
      }
      // 次の文字を読み飛ばす．
      ++ p;
    }
  }
  return std::string::npos;
}

// 前後の空白を取り除く
std::string
strip_wspace(
  const std::string& input
)
{
  std::string::size_type p1 = 0;
  std::string::size_type end = input.size();
  for ( ; p1 < end; ++ p1 ) {
    if ( !isspace(input[p1]) ) {
      break;
    }
  }
  std::string::size_type p2 = end;
  for ( ; p2 > p1; -- p2 ) {
    if ( !isspace(input[p2 - 1]) ) {
      break;
    }
  }
  return input.substr(p1, p2);
}

END_NONAMESPACE

// @brief パースする．
std::vector<std::pair<std::string, std::string>>
OptionParser::parse(
  const std::string& input
)
{
  std::vector<std::pair<std::string, std::string>> ans_list;
  // mDelim で区切る
  std::string tmp_str(input);
  for ( ; ; ) {
    std::string::size_type p = find_first_of(tmp_str, mDelim);
    std::string tmp = strip_wspace(tmp_str.substr(0, p));
    // tmp を mOptDelim で区切る．
    std::string::size_type q = find_first_of(tmp, mOptDelim);
    if ( q == std::string::npos ) {
      // mOptDelim がなかった
      ans_list.push_back(make_pair(tmp, std::string()));
    }
    else {
      std::string l_str = strip_wspace(tmp.substr(0, q));
      std::string r_str = strip_wspace(tmp.substr(q + 1, std::string::npos));
      ans_list.push_back(make_pair(l_str, r_str));
    }
    if ( p == std::string::npos ) {
      // 末尾だったので終わる．
      break;
    }
    // tmp_str を切り詰める．
    tmp_str = tmp_str.substr(p + 1, std::string::npos);
  }

  return ans_list;
}

END_NAMESPACE_YM
