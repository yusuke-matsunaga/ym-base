﻿#ifndef YMUTILS_GENBASE_H
#define YMUTILS_GENBASE_H

/// @file ym/GenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup GeneratorGroup 組み合わせ生成器と順列生成器
/// @ingroup YmUtils
///
/// - n 個の中から k 個の組み合わせを選びだす CombiGen
/// - n 個の中から k 個の順列を選びだす PermGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の組合わせを選びだす
///   MultiCombiGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の順列を選びだす
///   MultiPermGen
///
/// 使いかたは全てのクラスで共通で，コンストラクタでオブジェクトを
/// 生成した後，++ 演算子で末尾に達するまで(!is_end())，進めれば良い．
/// 値を取り出すには () 演算子を用いる．

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenBase GenBase.h "ym/GenBase.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen と PermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class GenBase
{
public:

  /// @brief コンストラクタ
  GenBase(
    SizeType n, ///< [in] 全要素数
    SizeType k  ///< [in] 選び出す要素数
  ) : mN{n},
      mElemList(k)
  {
    init();
  }

  /// @brief コピーコンストラクタ
  GenBase(
    const GenBase& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~GenBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全要素数を得る．
  /// @return 全要素数
  SizeType
  n() const { return mN; }

  /// @brief 選択する要素数を得る．
  /// @return 選択する要素数
  SizeType
  k() const { return mElemList.size(); }

  /// @brief 初期化する．
  void
  init();

  /// @brief 要素の取得
  /// @return pos 番目の要素
  int
  operator()(
    SizeType pos ///< [in] 取り出す要素の位置 (最初の位置は 0)
  ) const
  {
    _check_pos(pos);
    return mElemList[pos];
  }

  /// @brief 要素リストの取得
  std::vector<int>
  elem_list() const { return mElemList; }

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const
  {
    if ( n() == 0 ) {
      return true;
    }
    // 末尾の時には範囲外の値(= n())を持っている．
    return operator()(0) == n();
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の参照の取得
  /// @return pos 番目の要素への参照
  int&
  elem(
    SizeType pos ///< [in] 取り出す要素の位置 (最初の位置は 0)
  )
  {
    _check_pos(pos);
    return mElemList[pos];
  }

  /// @brief pos が適正かチェックする．
  void
  _check_pos(
    SizeType pos
  ) const
  {
    if ( pos >= mElemList.size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全要素数
  SizeType mN;

  // 現在の要素を持つ配列
  // サイズは mK;
  std::vector<int> mElemList;

};

END_NAMESPACE_YM

#endif // YMUTILS_GENBASE_H
