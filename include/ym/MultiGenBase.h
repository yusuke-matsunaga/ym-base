﻿#ifndef YM_MULTIGENBASE_H
#define YM_MULTIGENBASE_H

/// @file ym/MultiGenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/Array.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiGenBase MultiGenBase.h "ym/MultiGenBase.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen/MultiPermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenBase
{
public:

  /// @brief コンストラクタ
  MultiGenBase(
    const vector<pair<int, int>>& nk_array ///< [in] 要素数 n と選択する要素数 k のベクタ
  );

  /// @brief コンストラクタ
  MultiGenBase(
    initializer_list<pair<int, int>>& nk_array ///< [in] 要素数 n と選択する要素数 k の初期化リスト
  );

  /// @brief コピーコンストラクタ
  MultiGenBase(
    const MultiGenBase& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~MultiGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数の取得
  /// @return グループ数
  int
  group_num() const { return mGroupNum; }

  /// @brief 全要素数の取得
  /// @return grp 番目のグループの全要素数
  int
  n(
    int grp ///< [in] グループ番号 ( 0 <= grp < group_num() )
  ) const
  {
    ASSERT_COND( grp >= 0 && grp < mGroupNum );
    return mNArray[grp];
  }

  /// @brief 選択する要素数の取得
  /// @return grp 番目のグループの選択する要素数
  int
  k(
    int grp ///< [in] グループ番号 ( 0 <= grp < group_num() )
  ) const
  {
    ASSERT_COND( grp >= 0 && grp < mGroupNum );
    return mKArray[grp];
  }

  /// @brief 初期化
  void
  init();

  /// @brief 要素の取得
  /// @return grp 番目のグループの pos 番目の要素を取り出す．
  int
  operator()(
    int grp,       ///< [in] グループ番号 ( 0 <= grp < group_num() )
    int pos        ///< [in] 要素の位置 ( 0 <= pos < k(grp) )
  ) const
  {
    ASSERT_COND( grp >= 0 && grp < mGroupNum );
    ASSERT_COND( pos >= 0 && pos < k(grp) );
    return mElemArray[mOffsetArray[grp] + pos];
  }

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const { return is_end_sub(0); }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  void
  copy(
    const MultiGenBase& src ///< [in] コピー元のオブジェクト
  );

  /// @brief 要素配列の初期化
  ///
  /// grp 番目のグループの要素配列を初期化する．
  void
  init_group(
    int grp ///< [in] グループ番号
  );

  /// @brief 要素配列の取得
  /// @return grp 番目のグループの要素配列
  Array<int>
  elem(
    int grp ///< [in] グループ番号
  )
  {
    int offset = mOffsetArray[grp];
    return Array<int>(mElemArray, offset, offset + mKArray[grp]);
  }

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(
    int grp ///< [in] グループ番号
  ) const
  {
    return operator()(grp, 0) == n(grp);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループ数
  int mGroupNum;

  // 各グループごとの要素数の配列
  // サイズは mGroupNum
  int* mNArray;

  // 各グループごとの選択数の配列
  // サイズは mGroupNum
  int* mKArray;

  // 各グループごとのmElemArray上のオフセットの配列
  // = mOffsetArray[i] = sum_j^{i - 1} mKArray[j]
  // サイズは mGroupNum
  int* mOffsetArray;

  // 現在の要素(二重の配列を一次元の配列で表すので少しめんどくさい)
  // サイズは sum_i mKArray[i]
  int* mElemArray;

};

END_NAMESPACE_YM

#endif // YM_MULTIGENBASE_H
