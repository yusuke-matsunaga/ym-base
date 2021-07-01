#ifndef YMUTILS_MULTISETGENBASE_H
#define YMUTILS_MULTISETGENBASE_H

/// @file ym/MultiSetGenBase.h
/// @brief MultiSetGenBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetGenBase MultiSetGenBase.h "ym/MultiSetGenBase.h"
/// @brief MultiSetCombiGen と MultiSetPermGen の親クラス
//////////////////////////////////////////////////////////////////////
class MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  MultiSetGenBase(const vector<int>& num_array, ///< [in] 各要素の重複度を納めた配列
		  int k)                        ///< [in] 選び出す要素数
    : mNumArray{num_array},
      mK{k},
      mElem(mK)
  {
  }

  /// @brief コンストラクタ
  MultiSetGenBase(initializer_list<int>& num_array, ///< [in] 各要素の重複度を納めた初期化リスト
		  int k)                            ///< [in] 選び出す要素数
    : mNumArray{num_array},
      mK{k},
      mElem(mK)
  {
  }

  /// @brief デストラクタ
  ~MultiSetGenBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の種類の数を得る．
  int
  group_num() const { return static_cast<int>(mNumArray.size()); }

  /// @brief 各要素の重複度を得る．
  int
  n(int grp) const ///< [in] グループ番号 ( 0 <= grp < group_num() )
  {
    ASSERT_COND( grp < group_num() );

    return mNumArray[grp];
  }

  /// @brief 選択する要素数を返す．
  int
  k() const
  {
    return mK;
  }

  /// @brief 初期化する．
  void
  init();

  /// @brief 要素の取得
  int
  operator()(int pos) const ///< [in] 取り出す要素の位置 ( 0 <= pos <, k() )
  {
    return mElem[pos];
  }

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const
  {
    return mElem[0] == group_num();
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする関数
  void
  copy(const MultiSetGenBase& src); ///< [in] コピー元のオブジェクト

  /// @brief 要素の参照の取得
  /// @return pos 番目の要素への参照
  int&
  elem(int pos) ///< [in] 取り出す要素の位置 ( 0 <= pos < k() )
  {
    return mElem[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の重複度
  vector<int> mNumArray;

  // 選択する要素数
  int mK;

  // 現在の要素
  vector<int> mElem;

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTISETGENBASE_H
