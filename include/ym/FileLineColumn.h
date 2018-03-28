#ifndef FILELINECOLUMN_H
#define FILELINECOLUMN_H

/// @file FileLineColumn.h
/// @brief FileLineColumn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileLineColumn FileLineColumn.h "ym/FileLineColumn.h"
/// @brief 行番号とコラム番号を表すクラス
///
/// 省メモリ化のために32ビット整数で表す．
//////////////////////////////////////////////////////////////////////
class FileLineColumn
{
public:

  /// @brief コンストラクタ
  ///
  /// 内容は未定
  FileLineColumn();

  /// @brief 行番号とコラム番号をしていしたコンストラクタ
  FileLineColumn(int line,
		 int column);

  /// @brief デストラクタ
  ~FileLineColumn();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号の取得
  /// @return 行番号
  int
  line() const;

  /// @brief コラム位置の取得
  /// @return コラム位置
  int
  column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容
  // 上位 20 ビットが行番号
  // 下位 12 ビットがコラム位置
  ymuint32 mLineColumn;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
//
// 内容は未定
inline
FileLineColumn::FileLineColumn() :
  mLineColumn(0U)
{
}

// @brief 行番号とコラム番号をしていしたコンストラクタ
inline
FileLineColumn::FileLineColumn(int line,
			       int column)
{
  ASSERT_COND( line >= 0 && line < 0x100000 );
  ASSERT_COND( column >= 0 && column < 0x1000 );

  mLineColumn = (static_cast<ymuint32>(line) << 12) |
    (static_cast<ymuint32>(column) & 0xFFFU);
}

// @brief デストラクタ
inline
FileLineColumn::~FileLineColumn()
{
}

// @brief 行番号の取得
// @return 行番号
inline
int
FileLineColumn::line() const
{
  return static_cast<int>(mLineColumn >> 12);
}

// @brief コラム位置の取得
// @return コラム位置
inline
int
FileLineColumn::column() const
{
  return static_cast<int>(mLineColumn & 0xFFFU);
}

END_NAMESPACE_YM

#endif // FILELINECOLUMN_H
