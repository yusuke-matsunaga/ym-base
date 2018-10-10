#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

/// @file OptionParser.h
/// @brief OptionParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class OptionParser OptionParser.h "OptionParser.h"
/// @brief オプション文字列用パーサー
///
///   <オプションキーワード>[:<オプション値>][, <オプションキーワード>[:<オプション値>]]
/// の形式の文字列を受け取り，キーワードと値のpairのリスト(vector<pair<string, string>>)返す．
/// デフォルトでは要素の区切りは ',' オプションの区切りは ':' だが，
/// 設定することができる．
///
/// * 空白は取り除かれる．
/// * '\' を前につけることで区切り文字をエスケープすることが出来る．
//////////////////////////////////////////////////////////////////////
class OptionParser
{
public:

  /// @brief コンストラクタ
  /// @param[in] delim 区切り文字
  /// @param[in] opt_delim オプション用区切り文字
  OptionParser(char delim = ',',
	       char opt_delim = ':');

  /// @brief デストラクタ
  ~OptionParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @grief デリミタを設定する．
  /// @param[in] delim 区切り文字
  /// @param[in] opt_delim オプション用区切り文字
  void
  set_delim(char delim,
	    char opt_delim);

  /// @brief パースする．
  /// @param[in] input 入力文字列
  /// @return パース結果のハッシュ表への参照
  const vector<pair<string, string>>&
  parse(const string& input);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 区切り文字
  char mDelim;

  // オプション区切り文字
  char mOptDelim;

  // パース結果を保持しておくリスト
  vector<pair<string, string>> mOptList;

};

//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////


END_NAMESPACE_YM

#endif // OPTIONPARSER_H
