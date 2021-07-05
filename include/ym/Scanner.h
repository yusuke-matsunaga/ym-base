#ifndef YM_SCANNER_H
#define YM_SCANNER_H

/// @file ym/Scanner.h
/// @brief Scanner のヘッダファイル
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
/// @class Scanner Scanner.h "ym/Scanner.h"
/// @brief 位置情報のトラッキング付きの入力データ読み出し器
///
/// 入力データはコンストラクタで指定する．
/// 途中で切り替えることはできない．
/// このクラスで行える処理は
/// - 一文字読み出し     (get)
/// - 一文字の先読み     (peek)
/// - 先読みした文字の確定 (accept)
/// これ以外にトークンの開始位置を set_first_loc() で記録して
/// cur_loc() で現在の位置までの領域を求める．
///
/// istream の代りにこのクラスを使う利点は以下の通り
/// - エラー出力を行なう際に問題となった箇所の位置を示すことができる．
/// - UNIX/MacOS/Windows による改行コードの違いを自動的に吸収する．
//////////////////////////////////////////////////////////////////////
class Scanner
{
public:

  /// @brief コンストラクタ
  Scanner(
    istream& s,               ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );

  /// @brief デストラクタ
  virtual
  ~Scanner() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力データ関連の操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オープン中のファイル情報を得る．
  const FileInfo&
  file_info() const { return mFileInfo; }

  /// @brief 現在のファイル情報を書き換える．
  ///
  /// プリプロセッサのプラグマなどで用いることを想定している．
  /// 通常は使わないこと．
  void
  set_file_info(
    const FileInfo& file_info ///< [in] 新しいファイル情報
  )
  {
    mFileInfo = file_info;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 値を読み出す関数
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
    (void) peek();
    accept();
    return mNextChar;
  }

  /// @brief ファイルの末尾の時にtrue を返す．
  bool
  is_eof() const { return mNextChar == EOF; }

  /// @brief 現在の位置をトークンの最初の位置にセットする．
  void
  set_first_loc();

  /// @brief 現在の位置を返す．
  FileLoc
  cur_pos() const
  {
    return FileLoc(file_info(), mCurLine, mCurColumn);
  }

  /// @brief 直前の set_first_loc() から現在の位置までを返す．
  FileRegion
  cur_region() const
  {
    return FileRegion(file_info(), mFirstLine, mFirstColumn, mCurLine, mCurColumn);
  }

  /// @brief 改行を読み込んだ時に起動する関数
  ///
  /// デフォルトではなにもしない．
  virtual
  void
  check_line(
    int line ///< [in] 行番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief peek() の下請け関数
  void
  update();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

  // ファイル情報
  FileInfo mFileInfo;

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

};

END_NAMESPACE_YM

#endif // YM_FILESCANNER_H
