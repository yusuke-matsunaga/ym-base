#ifndef JSONPARSER_H
#define JSONPARSER_H

/// @file JsonParser.h
/// @brief JsonParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"
#include "JsonScanner.h"


BEGIN_NAMESPACE_YM_JSON

class JsonObj;
class Region;

//////////////////////////////////////////////////////////////////////
/// @class JsonParser JsonParser.h "JsonParser.h"
/// @brief json のパーサークラス
//////////////////////////////////////////////////////////////////////
class JsonParser
{
public:

  /// @brief コンストラクタ
  JsonParser(
    istream& s ///< [in] 入力ストリーム
  );

  /// @brief デストラクタ
  ~JsonParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込む．
  JsonValue
  read();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  JsonObj*
  read_value();

  /// @brief オブジェクトを読み込む．
  JsonObj*
  read_object();

  /// @brief 配列を読み込む．
  JsonObj*
  read_array();

  /// @brief エラーを出力する．
  void
  error(
    const string& msg ///< [in] メッセージ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スキャナー
  JsonScanner mScanner;

};

END_NAMESPACE_YM_JSON

#endif // JSONPARSER_H
