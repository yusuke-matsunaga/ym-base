
/// @file InputFileObj.cc
/// @brief InputFileObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2019 Yusuke Matsunaga
/// All rights reserved.

#include "ym/InputFileObj.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
// @param[in] s 入力ストリーム
// @param[in] file_info ファイル情報
InputFileObj::InputFileObj(istream& s,
			   const FileInfo& file_info) :
  mS{s},
  mFileInfo{file_info}
{
  emit();
}

// @brief peek() の下請け関数
void
InputFileObj::update()
{
  int c = mS.peek();
  mS.ignore();
  if ( c == '\r' ) {
    c = mS.peek();
    if ( c != '\n' ) {
      // MAC 形式('\r'のみ)
      c = '\n';
    }
    else {
      // WIN 形式('\r', '\n')
      // そのまま
      mS.ignore();
    }
  }
  mNeedUpdate = false;
  mNextChar = c;
}

// @brief 直前の peek() を確定させる．
void
InputFileObj::accept()
{
  ASSERT_COND( mNeedUpdate == false );

  mNeedUpdate = true;
  mCurLine = mNextLine;
  mCurColumn = mNextColumn;
  // mNextLine と mNextColumn を先に設定しておく
  if ( mNextChar == '\n' ) {
    ++ mNextLine;
    mNextColumn = 0;
  }
  ++ mNextColumn;
  emit();
}

// @brief 改行ハンドラを呼び出す．
void
InputFileObj::emit()
{
  for ( auto& handler: mHandlerList ) {
    handler->newline(mNextLine);
  }
}

END_NAMESPACE_YM
