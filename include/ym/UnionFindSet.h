#ifndef YM_UNIONFINDSET_H
#define YM_UNIONFINDSET_H

/// @file ym/UnionFindSet.h
/// @brief UnionFindSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

// クラスの前方参照定義
// ユーザは知る必要はない．
class UFCell;

//////////////////////////////////////////////////////////////////////
/// @class UnionFindSet UnionFindSet.h "ym/UnionFindSet.h"
/// @ingroup YmUtils
/// @brief Merge/Find set を実装したクラス
///
/// データ構造とアルゴリズムの教科書でおなじみの Merge/Find-set
/// お互いに素な集合のマージと検索のみを行なう抽象データ型
//////////////////////////////////////////////////////////////////////
class UnionFindSet
{
public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲外を表す値
  static
  const int kBadID = -1;

public:

  /// @brief コンストラクタ
  UnionFindSet(
    SizeType n ///< [in] 確保したい要素の数．
  );

  /// @brief デストラクタ
  ~UnionFindSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mNum;
  }

  /// @brief 代表元の検索 (Find)
  /// @retval 要素 x の属する集合の代表元
  /// @retval kBadID 要素 x が存在していない場合
  int
  find(
    int x ///< [in] 要素番号 ( 0 <= x < num() )
  );

  /// @brief 2つの集合の併合 (Merge)
  /// @retval 新たな代表元を返す．
  /// @retval kBadID x か y が存在していなかった
  ///
  /// 2つの代表元 x, y の表す集合を併合する．
  /// 実は x, y が代表元でない場合，
  /// 内部で find(x), find(y)を呼ぶので処理は行えるが，
  /// 代表元が分かっている場合にはそれを使ったほうが処理は速い．
  int
  merge(
    int x, ///< [in] マージ対象の要素番号1 ( 0 <= x < num() )
    int y  ///< [in] マージ対象の要素番号2 ( 0 <= y < num() )
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 番号 x の要素セルを取ってくる．
  /// そのような要素がない場合にはnullptrを返す．
  UFCell*
  _get(
    int x
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列の要素数
  SizeType mNum;

  // 要素の配列
  UFCell* mCellArray;

};

END_NAMESPACE_YM

#endif // YM_UNIONFINDSET_H
