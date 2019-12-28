#ifndef YM_INPUTFILEOBJ_H
#define YM_INPUTFILEOBJ_H

/// @file ym/InputFileObj.h
/// @brief InputFileObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2018, 2019 Yusuke Matsunaga
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
  ~NewlineHandler() {};


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 改行コードが読み込まれるときに呼ばれるコールバック関数
  /// @param[in] lineno 行番号
  virtual
  void
  newline(int lineno) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


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
  /// @param[in] s 入力ストリーム
  /// @param[in] file_info ファイル情報
  InputFileObj(istream& s,
	       const FileInfo& file_info);

  /// @brief デストラクタ
  ~InputFileObj() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル情報関連の操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オープン中のファイル情報を得る．
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  ///
  /// プリプロセッサのプラグマなどで用いることを想定している．
  /// 通常は使わないこと．
  void
  set_file_info(const FileInfo& file_info);

  /// @brief 改行ハンドラを登録する．
  /// @param[in] handler 登録するハンドラ
  ///
  /// ハンドラの所有権は InputFileObj に移る．
  void
  reg_handler(unique_ptr<NewlineHandler>&& handler);


public:
  //////////////////////////////////////////////////////////////////////
  // データの入力関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一文字読み出す．
  ///
  /// 実際には peek(); acept() と等価
  int
  get();

  /// @brief 次の文字を読み出す．
  ///
  /// ファイル位置の情報等は変わらない
  int
  peek();

  /// @brief 直前の peek() を確定させる．
  void
  accept();

  /// @brief 現在の位置を返す．
  FileLoc
  cur_loc() const;


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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief オープン中のファイル情報を得る．
inline
const FileInfo&
InputFileObj::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
inline
void
InputFileObj::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief 改行ハンドラを登録する．
// @param[in] handler 登録するハンドラ
//
// ハンドラの所有権は InputFileObj に移る．
inline
void
InputFileObj::reg_handler(unique_ptr<NewlineHandler>&& handler)
{
  mHandlerList.push_back(move(handler));
}

// @brief 次の文字を読み出す．
// @note ファイル位置の情報等は変わらない
inline
int
InputFileObj::peek()
{
  if ( mNeedUpdate ) {
    update();
  }
  return mNextChar;
}

// 一文字読み出す．
inline
int
InputFileObj::get()
{
  int c = peek();
  accept();
  return c;
}

// @brief 直前の set_first_loc() から現在の位置までを返す．
inline
FileLoc
InputFileObj::cur_loc() const
{
  return FileLoc(file_info(), mCurLine, mCurColumn);
}

END_NAMESPACE_YM

#endif // YM_INPUTFILEOBJ_H
