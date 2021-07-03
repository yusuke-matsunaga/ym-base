#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

/// @file OptionParser.h
/// @brief OptionParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


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
  OptionParser(
    char delim = ',',    ///< [in] 区切り文字
    char opt_delim = ':' ///< [in] オプション用区切り文字
  ) : mDelim(delim),
      mOptDelim(opt_delim)
  {
  }

  /// @brief デストラクタ
  ~OptionParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @grief デリミタを設定する．
  void
  set_delim(
    char delim,    ///< [in] 区切り文字
    char opt_delim ///< [in] オプション用区切り文字
  );

  /// @brief パースする．
  /// @return パース結果の<キー，値>のペアのリストを返す．
  vector<pair<string, string>>
  parse(
    const string& input ///< [in] 入力文字列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 区切り文字
  char mDelim;

  // オプション区切り文字
  char mOptDelim;

};

END_NAMESPACE_YM

#endif // OPTIONPARSER_H
