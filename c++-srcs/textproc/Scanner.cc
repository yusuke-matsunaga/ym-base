
/// @file Scanner.cc
/// @brief Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Scanner.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
Scanner::Scanner(
  std::istream& s,
  const FileInfo& file_info
) : mS{s},
    mFileInfo{file_info},
    mCurLine{1},
    mCurColumn{1},
    mFirstLine{1},
    mFirstColumn{1},
    mNextLine{1},
    mNextColumn{1},
    mNeedUpdate{true}
{
}

// @brief peek() の下請け関数
void
Scanner::update()
{
  int c = mS.peek();
  mS.ignore();

  // Windows(DOS)/Mac/UNIX の間で改行コードの扱いが異なるのでここで
  // 強制的に '\n' に書き換えてしまう．
  // Windows : '\r', '\n'
  // Mac     : '\r'
  // UNIX    : '\n'
  // なので '\r' を '\n' に書き換えてしまう．
  // ただし次に本当の '\n' が来たときには無視するために
  // mCR を true にしておく．
  if ( c == '\r' ) {
    c = mS.peek();
    if ( c != '\n' ) {
      // MAC 形式 ('\r' のみ)
      c = '\n';
    }
    else {
      // Windows 形式 ('\r', '\n')
      mS.ignore();
    }
  }
  mNeedUpdate = false;
  mNextChar = c;
}

// @brief 直前の peek() を確定させる．
void
Scanner::accept()
{
  ASSERT_COND( mNeedUpdate == false );

  mNeedUpdate = true;
  mCurLine = mNextLine;
  mCurColumn = mNextColumn;
  // mNextLine と mNextColumn を先に設定しておく
  if ( mNextChar == '\n' ) {
    check_line(mCurLine);
    ++ mNextLine;
    mNextColumn = 0;
  }
  ++ mNextColumn;
}

// @brief 現在の位置をトークンの最初の位置にセットする．
void
Scanner::set_first_loc()
{
  mFirstLine = mCurLine;
  mFirstColumn = mCurColumn;
}

// @brief 改行を読み込んだ時に起動する関数
// @param[in] line 行番号
// @note デフォルトではなにもしない．
void
Scanner::check_line(int line)
{
}

END_NAMESPACE_YM
