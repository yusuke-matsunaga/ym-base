#ifndef JSONSCANNER_H
#define JSONSCANNER_H

/// @file JsonScanner.h
/// @brief JsonScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"
#include "Loc.h"
#include "Region.h"


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
class JsonScanner
{
public:

  /// @brief コンストラクタ
  JsonScanner(
    istream& s ///< [in] 入力ストリーム
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
  cur_string()
  {
    return mCurString;
  }

  /// @brief 直前の read_token() で読み出した字句の整数を返す．
  int
  cur_int()
  {
    return atoi(mCurString.c_str());
  }

  /// @brief 直前の read_token() で読み出した字句の浮動小数点数を返す．
  double
  cur_float()
  {
    return atof(mCurString.c_str());
  }

  /// @brief 直前の read_token() で読み出した字句の位置を返す．
  Region
  cur_loc()
  {
    return mCurLoc;
  }


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

  /// @brief peek() の下請け関数
  void
  update();

  /// @brief 次の文字を読み出す．
  ///
  /// ファイル位置の情報等は変わらない
  int
  peek()
  {
    if ( mNeedUpdate ) {
      update();
    }
    return mNextChar;
  }

  /// @brief 直前の peek() を確定させる．
  void
  accept();

  /// @brief 一文字読み出す．
  ///
  /// 実際には peek(); acept() と等価
  int
  get()
  {
    (void) peek();
    accept();
    return mNextChar;
  }

  /// @brief ファイルの末尾の時にtrue を返す．
  bool
  is_eof() const { return mNextChar == EOF; }

  /// @brief 現在の位置をトークンの最初の位置にセットする．
  void
  set_first_loc()
  {
    mFirstLine = mCurLine;
    mFirstColumn = mCurColumn;
  }

  /// @brief 改行を読み込んだ時に起動する関数
  ///
  /// デフォルトではなにもしない．
  void
  check_line(
    int line ///< [in] 行番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

  // 現在の行番号
  int mCurLine;

  // 現在のコラム位置
  int mCurColumn;

  // トークンの最初の行番号
  int mFirstLine;

  // トークンの最初のコラム位置
  int mFirstColumn;

  // peek() した文字
  int mNextChar;

  // peek() した文字の行番号
  int mNextLine;

  // peek() した文字のコラム位置
  int mNextColumn;

  // 新しい文字を読み込む必要がある時 true となるフラグ
  bool mNeedUpdate;

  // 文字列バッファ
  string mCurString;

  // 直前に読み出した字句の位置
  Region mCurLoc;

  // 読み戻したトークン
  JsonToken mUngetToken{JsonToken::None};

};

END_NAMESPACE_YM_JSON

#endif // JSONSCANNER_H
