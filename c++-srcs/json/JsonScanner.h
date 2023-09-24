#ifndef JSONSCANNER_H
#define JSONSCANNER_H

/// @file JsonScanner.h
/// @brief JsonScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"
#include "ym/Scanner.h"
#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM_JSON

/// @brief json のトークン
enum class JsonToken {
  None,
  LCB,
  RCB,
  LBK,
  RBK,
  Comma,
  Colon,
  String,
  Int,
  Float,
  True,
  False,
  Null,
  End
};


//////////////////////////////////////////////////////////////////////
/// @class JsonScanner JsonScanner.h "JsonScanner.h"
/// @brief json 用の字句解析器
//////////////////////////////////////////////////////////////////////
class JsonScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  JsonScanner(
    istream& s,               ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );

  /// @brief デストラクタ
  ~JsonScanner() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つ読み出す．
  JsonToken
  read_token();

  /// @brief 直前に読み込んだトークンを戻す．
  void
  unget_token(
    JsonToken tk ///< [in] トークン
  );

  /// @brief 直前の read_token() で読み出した字句の文字列を返す．
  string
  cur_string();

  /// @brief 直前の read_token() で読み出した字句の整数を返す．
  int
  cur_int();

  /// @brief 直前の read_token() で読み出した字句の浮動小数点数を返す．
  double
  cur_float();

  /// @brief 直前の read_token() で読み出した字句のファイル上の位置を返す．
  FileRegion
  cur_loc();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() のサブルーチン
  JsonToken
  scan();

  /// @brief 'true' を読み込む．
  bool
  read_true();

  /// @brief 'false' を読み込む．
  bool
  read_false();

  /// @brief 'null' を読み込む．
  bool
  read_null();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列バッファ
  StrBuff mCurString;

  // ファイル上の位置
  FileRegion mCurLoc;

  // 読み戻したトークン
  JsonToken mUngetToken{JsonToken::None};

};

END_NAMESPACE_YM_JSON

#endif // JSONSCANNER_H
