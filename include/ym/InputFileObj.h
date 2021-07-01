#ifndef YM_INPUTFILEOBJ_H
#define YM_INPUTFILEOBJ_H

/// @file ym/InputFileObj.h
/// @brief InputFileObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/FileInfo.h"
#include "ym/FileLoc.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class NewlineHandler InputFileObj.h "ym/InputFileObj.h"
/// @brief 改行コードが読み込まれる度に起動されるハンドラクラス
/// @sa InputFileObj
//////////////////////////////////////////////////////////////////////
class NewlineHandler
{
public:

  /// @brief デストラクタ
  virtual
  ~NewlineHandler() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 改行コードが読み込まれるときに呼ばれるコールバック関数
  virtual
  void
  newline(int lineno) ///< [in] 行番号
  = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class InputFileObj InputFileObj.h "ym/InputFileObj.h"
/// @brief 位置情報のトラッキング付きの入力データファイル
///
/// 入力データはコンストラクタで指定する．
/// 途中で切り替えることはできない．
/// このクラスで行える処理は
/// - 一文字読み出し     (get)
/// - 一文字の先読み     (peek)
/// - 先読みした文字の確定 (accept)
/// これ以外にトークンの開始位置を set_first_loc() で記録して
/// cur_loc() で現在の位置までの領域を求める．
//////////////////////////////////////////////////////////////////////
class InputFileObj
{
public:

  /// @brief コンストラクタ
  InputFileObj(istream& s,                 ///< [in] 入力ストリーム
	       const FileInfo& file_info); ///< [in] ファイル情報

  /// @brief デストラクタ
  ~InputFileObj() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル情報関連の操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オープン中のファイル情報を得る．
  const FileInfo&
  file_info() const { return mFileInfo; }

  /// @brief 現在のファイル情報を書き換える．
  ///
  /// プリプロセッサのプラグマなどで用いることを想定している．
  /// 通常は使わないこと．
  void
  set_file_info(const FileInfo& file_info) ///< [in] 新しいファイル情報
  {
    mFileInfo = file_info;
  }

  /// @brief 改行ハンドラを登録する．
  ///
  /// ハンドラの所有権は InputFileObj に移る．
  void
  reg_handler(unique_ptr<NewlineHandler>&& handler) ///< [in] 登録するハンドラ
  {
    mHandlerList.push_back(move(handler));
  }


public:
  //////////////////////////////////////////////////////////////////////
  // データの入力関係の関数
  //////////////////////////////////////////////////////////////////////

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
    int c = peek();
    accept();
    return c;
  }

  /// @brief ファイルの末尾の時にtrue を返す．
  bool
  is_eof() const { return mNextChar == EOF; }

  /// @brief 現在の位置を返す．
  FileLoc
  cur_loc() const { return FileLoc(file_info(), mCurLine, mCurColumn); }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief peek() の下請け関数
  void
  update();

  /// @brief 改行ハンドラを呼び出す．
  void
  emit();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

  // ファイル情報
  FileInfo mFileInfo{};

  // 現在の行番号
  int mCurLine{1};

  // 現在のコラム位置
  int mCurColumn{1};

  // peek() した文字
  int mNextChar{-1};

  // peek() した文字の行番号
  int mNextLine{1};

  // peek() した文字のコラム位置
  int mNextColumn{1};

  // 新しい文字を読み込む必要がある時 true となるフラグ
  bool mNeedUpdate{true};

  // ハンドラのリスト
  vector<unique_ptr<NewlineHandler>> mHandlerList;

};

END_NAMESPACE_YM

#endif // YM_INPUTFILEOBJ_H
