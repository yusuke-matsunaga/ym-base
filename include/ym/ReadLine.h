#ifndef READLINE_H
#define READLINE_H

/// @file ReadLine.h
/// @brief ReadLine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ReadLine ReadLine.h "ReadLine.h"
/// @brief readline() の C++ ラッパクラス
///
/// static method のみを持つ．
//////////////////////////////////////////////////////////////////////
class ReadLine
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラインエディット機能付きで1行読み込む．
  /// @retval true 正常に読み込んだ．
  /// @retval false EOF が読み込まれた．
  static
  bool
  get_line(
    const std::string& prompt, ///< [in] プロンプト文字列
    std::string& linebuf       ///< [out] 読み込んだ文字列を格納する変数
  );

};

END_NAMESPACE_YM

#endif // READLINE_H
