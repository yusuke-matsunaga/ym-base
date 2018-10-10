
/// @file OptionParser.cc
/// @brief OptionParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/OptionParser.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス OptionParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] delim 区切り文字
// @param[in] opt_delim オプション用区切り文字
OptionParser::OptionParser(char delim,
			   char opt_delim) :
  mDelim(delim),
  mOptDelim(opt_delim)
{
}

// @brief デストラクタ
OptionParser::~OptionParser()
{
}

// @grief デリミタを設定する．
// @param[in] delim 区切り文字
// @param[in] opt_delim オプション用区切り文字
void
OptionParser::set_delim(char delim,
			char opt_delim)
{
  mDelim = delim;
  mOptDelim = opt_delim;
}

BEGIN_NONAMESPACE

// string::find_first_of() のバリエーション
// 直前の '\' をエスケープ文字とみなす．
string::size_type
find_first_of(const string& input,
	      char c)
{
  string::size_type p = 0;
  string::size_type end = input.size();
  for ( ; p < end; ++ p ) {
    char c1 = input[p];
    if ( c1 == c ) {
      return p;
    }
    if ( c1 == '\\' ) {
      if ( p + 1 == end ) {
	// 末尾が \ だった．
	return string::npos;
      }
      // 次の文字を読み飛ばす．
      ++ p;
    }
  }
  return string::npos;
}

// 前後の空白を取り除く
string
strip_wspace(const string& input)
{
  string::size_type p1 = 0;
  string::size_type end = input.size();
  for ( ; p1 < end; ++ p1 ) {
    if ( !isspace(input[p1]) ) {
      break;
    }
  }
  string::size_type p2 = end;
  for ( ; p2 > p1; -- p2 ) {
    if ( !isspace(input[p2 - 1]) ) {
      break;
    }
  }
  return input.substr(p1, p2);
}

END_NONAMESPACE

// @brief パースする．
// @param[in] input 入力文字列
// @return パース結果のリストへの参照
const vector<pair<string, string>>&
OptionParser::parse(const string& input)
{
  // mDelim で区切る
  string tmp_str(input);
  mOptList.clear();
  for ( ; ; ) {
    string::size_type p = find_first_of(tmp_str, mDelim);
    string tmp = strip_wspace(tmp_str.substr(0, p));
    // tmp を mOptDelim で区切る．
    string::size_type q = find_first_of(tmp, mOptDelim);
    if ( q == string::npos ) {
      // mOptDelim がなかった
      mOptList.push_back(make_pair(tmp, string()));
    }
    else {
      string l_str = strip_wspace(tmp.substr(0, q));
      string r_str = strip_wspace(tmp.substr(q + 1, string::npos));
      mOptList.push_back(make_pair(l_str, r_str));
    }
    if ( p == string::npos ) {
      // 末尾だったので終わる．
      break;
    }
    // tmp_str を切り詰める．
    tmp_str = tmp_str.substr(p + 1, string::npos);
  }

  return mOptList;
}

END_NAMESPACE_YM
