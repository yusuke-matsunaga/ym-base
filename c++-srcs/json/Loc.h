#ifndef LOC_H
#define LOC_H

/// @file Loc.h
/// @brief Locのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"


BEGIN_NAMESPACE_YM_JSON

//////////////////////////////////////////////////////////////////////
/// @class Loc Loc.h "ym/Loc.h"
/// @ingroup ym
/// @brief 位置を表すクラス
///
/// - 行番号
/// - コラム位置
/// の情報を持つ
/// @sa LocLoc
//////////////////////////////////////////////////////////////////////
class Loc
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 無効なデータを持つ
  Loc() = default;

  /// @brief 内容を指定するコンストラクタ
  Loc(
    int line,  ///< [in] 行番号
    int column ///< [in] コラム番号
  )
  {
    if ( line < 1 || line >= 0x100000 ) {
      throw std::invalid_argument{"Loc(line, column): line is out of range"};
    }
    if ( column < 1 || column >= 0x1000 ) {
      throw std::invalid_argument{"Loc(line, column): column is out of range"};
    }

    mLineColumn = (static_cast<std::uint32_t>(line) << 12) |
      (static_cast<std::uint32_t>(column) & 0xFFFU);
  }

  /// @brief デストラクタ
  ~Loc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持つ時 true を返す．
  bool
  is_valid() const
  {
    return mLineColumn != 0;
  }

  /// @brief 行番号の取得
  /// @return 行番号
  int
  line() const { return static_cast<int>(mLineColumn >> 12); }

  /// @brief コラム位置の取得
  /// @return コラム位置
  int
  column() const { return static_cast<int>(mLineColumn & 0xFFFU); }

  /// @brief 等価比較演算子
  /// @retval true 等しい
  /// @retval false 等しくない
  bool
  operator==(
    const Loc& right ///< [in] 右のオペランド
  )
  {
    return mLineColumn == right.mLineColumn;
  }

  /// @brief 非等価比較演算子
  /// @retval true 等しくない．
  /// @retval false 等しい．
  bool
  operator!=(
    const Loc& right ///< [in] 右のオペランド
  )
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号とコラム位置をパックしたもの
  // 上位 20 ビットが行番号
  // 下位 12 ビットがコラム位置
  std::uint32_t mLineColumn{0};

};


//////////////////////////////////////////////////////////////////////
/// @name Loc に関連する非メンバ関数
/// @{

/// @relates Loc
/// @brief Loc を表示するための関数
/// @return s をそのまま返す
ostream&
operator<<(
  ostream& s,    ///< [in] 出力ストリーム
  const Loc& loc ///< [in] 位置の情報
);

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_JSON

#endif // YM_LOC_H
