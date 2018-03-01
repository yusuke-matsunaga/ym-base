#ifndef YMUTILS_MULTISETGENBASE_H
#define YMUTILS_MULTISETGENBASE_H

/// @file ym/MultiSetGenBase.h
/// @brief MultiSetGenBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
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
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetGenBase(const vector<int>& num_array,
		  int k);

  /// @brief デストラクタ
  ~MultiSetGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の種類の数を得る．
  int
  group_num() const;

  /// @brief 各要素の重複度を得る．
  /// @param[in] grp ( 0 <= grp < group_num() )
  int
  n(int grp) const;

  /// @brief 選択する要素数を返す．
  int
  k() const;

  /// @brief 初期化する．
  void
  init();

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置
  int
  operator()(int pos) const;

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする関数
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MultiSetGenBase& src);

  /// @brief 要素の参照の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素への参照
  int&
  elem(int pos);


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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要素の種類の数を得る．
inline
int
MultiSetGenBase::group_num() const
{
  return static_cast<int>(mNumArray.size());
}

// @brief 各要素の重複度を得る．
// @param[in] grp ( 0 <= grp < group_num() )
inline
int
MultiSetGenBase::n(int grp) const
{
  ASSERT_COND( grp < group_num() );

  return mNumArray[grp];
}

// @brief 選択する要素数を返す．
inline
int
MultiSetGenBase::k() const
{
  return mK;
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置
inline
int
MultiSetGenBase::operator()(int pos) const
{
  return mElem[pos];
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
inline
bool
MultiSetGenBase::is_end() const
{
  return mElem[0] == group_num();
}

// @brief 要素の参照の取得
// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
// @return pos 番目の要素への参照
inline
int&
MultiSetGenBase::elem(int pos)
{
  return mElem[pos];
}

END_NAMESPACE_YM

#endif // YMUTILS_MULTISETGENBASE_H
