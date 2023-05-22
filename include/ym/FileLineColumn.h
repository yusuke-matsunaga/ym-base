#ifndef FILELINECOLUMN_H
#define FILELINECOLUMN_H

/// @file FileLineColumn.h
/// @brief FileLineColumn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
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
  FileLineColumn() : mLineColumn{0U} { }

  /// @brief 行番号とコラム番号をしていしたコンストラクタ
  FileLineColumn(int line,   ///< [in] 行番号
		 int column) ///< [in] コラム番号
  {
    ASSERT_COND( line >= 0 && line < 0x100000 );
    ASSERT_COND( column >= 0 && column < 0x1000 );

    mLineColumn = (static_cast<std::uint32_t>(line) << 12) |
      (static_cast<std::uint32_t>(column) & 0xFFFU);
  }

  /// @brief デストラクタ
  ~FileLineColumn() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号の取得
  /// @return 行番号
  int
  line() const { return static_cast<int>(mLineColumn >> 12); }

  /// @brief コラム位置の取得
  /// @return コラム位置
  int
  column() const { return static_cast<int>(mLineColumn & 0xFFFU); }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容
  // 上位 20 ビットが行番号
  // 下位 12 ビットがコラム位置
  std::uint32_t mLineColumn;

};

END_NAMESPACE_YM

#endif // FILELINECOLUMN_H
