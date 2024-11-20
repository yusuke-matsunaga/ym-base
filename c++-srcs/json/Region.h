#ifndef REGION_H
#define REGION_H

/// @file Region.h
/// @brief Region のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"
#include "Loc.h"


BEGIN_NAMESPACE_YM_JSON

//////////////////////////////////////////////////////////////////////
/// @class Region Region.h "Region.h"
/// @brief ファイル上の領域を表すクラス
/// @sa Loc
///
/// 具体的には開始位置と終了位置を持つ．
//////////////////////////////////////////////////////////////////////
class Region
{
public:

  /// @brief 空のデフォルトコンストラクタ
  ///
  /// 結果は invalid な情報を持つオブジェクトとなる．
  Region() = default;

  /// @brief 内容を指定したコンストラクタ
  Region(
    int start_line,   ///< [in] 先頭の行番号
    int start_column, ///< [in] 先頭のコラム番号
    int end_line,     ///< [in] 末尾の行番号
    int end_column    ///< [in] 末尾のコラム番号
  ) : mStartLoc{start_line, start_column},
      mEndLoc{end_line, end_column}
  {
  }

  /// @brief Loc からの変換コンストラクタ
  Region(
    const Loc& loc ///< [in] 位置
  ) : mStartLoc{loc},
      mEndLoc{loc}
  {
  }

  /// @brief 内容を指定したコンストラクタ
  Region(
    const Loc& start_loc, ///< [in] 先頭の位置
    const Loc& end_loc    ///< [in] 末尾の位置
  ) : mStartLoc{start_loc},
      mEndLoc{end_loc}
  {
  }

  /// @brief 内容を指定したコンストラクタ
  Region(
    const Region& start_region, ///< [in] 先頭の位置
    const Region& end_region    ///< [in] 末尾の位置
  ) : mStartLoc{start_region.start_loc()},
      mEndLoc{end_region.end_loc()}
  {
  }

  /// @brief デストラクタ
  ~Region() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データの妥当性のチェック
  /// @return 意味のある値を持っているとき true を返す．
  bool
  is_valid() const
  {
    return mStartLoc.is_valid() && mEndLoc.is_valid();
  }

  /// @brief 先頭のファイル位置の取得
  /// @return ファイル位置
  Loc
  start_loc() const
  {
    return mStartLoc;
  }

  /// @brief 先頭の行番号の取得
  /// @return 行番号
  int
  start_line() const
  {
    return mStartLoc.line();
  }

  /// @brief 先頭のコラム位置の取得
  /// @return コラム位置
  int
  start_column() const
  {
    return mStartLoc.column();
  }

  /// @brief 末尾のファイル位置の取得
  /// @return ファイル位置
  Loc
  end_loc() const
  {
    return mEndLoc;
  }

  /// @brief 末尾の行番号の取得
  /// @return 行番号
  int
  end_line() const
  {
    return mEndLoc.line();
  }

  /// @brief 末尾のコラム位置の取得
  /// @return コラム位置
  int
  end_column() const
  {
    return mEndLoc.column();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  Loc mStartLoc;

  // 末尾の位置
  Loc mEndLoc;

};


//////////////////////////////////////////////////////////////////////
/// @name Region の出力関数
/// @{

/// @relates Region
/// @brief Region を表示するための関数
/// @return s をそのまま返す
inline
ostream&
operator<<(
  ostream& s,          ///< [in] 出力ストリーム
  const Region& region ///< [in] ファイル領域の情報
)
{
  if ( region.is_valid() ) {
    int start_line = region.start_line();
    int start_column = region.start_column();
    int end_line = region.end_line();
    int end_column = region.end_column();
    if ( start_line == end_line ) {
      s << "line " << start_line;
      if ( start_column == end_column ) {
	s << ", column = " << start_column;
      }
      else {
	s << ", column " << start_column
	  << " - " << end_column;
      }
    }
    else {
      s << "line " << start_line
	<< ", column " << start_column
	<< " - line " << end_line
	<< ", colmun " << end_column;
    }
  }
  return s;
}

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_JSON

#endif // REGION_H
