#ifndef JSONPARSER_H
#define JSONPARSER_H

/// @file JsonParser.h
/// @brief JsonParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Json.h"
#include "ym/FileInfo.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM

class JsonValue;
class JsonScanner;

//////////////////////////////////////////////////////////////////////
/// @class JsonParser JsonParser.h "JsonParser.h"
/// @brief json のパーサークラス
//////////////////////////////////////////////////////////////////////
class JsonParser
{
public:

  /// @brief コンストラクタ
  JsonParser();

  /// @brief デストラクタ
  ~JsonParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込む．
  JsonValue*
  read(
    istream& s,               ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  JsonValue*
  read_value();

  /// @brief オブジェクトを読み込む．
  JsonValue*
  read_object();

  /// @brief 配列を読み込む．
  JsonValue*
  read_array();

  /// @brief エラーを出力する．
  void
  error(
    const string& msg,    ///< [in] メッセージ
    const FileRegion& loc ///< [in] ファイル位置
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スキャナー
  JsonScanner* mScanner{nullptr};

};

END_NAMESPACE_YM

#endif // JSONPARSER_H
