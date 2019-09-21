#ifndef YM_MULTIGENBASE_H
#define YM_MULTIGENBASE_H

/// @file ym/MultiGenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
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

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiGenBase(const vector<pair<int, int>>& nk_array);

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiGenBase(initializer_list<pair<int, int>>& nk_array);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiGenBase(const MultiGenBase& src);

  /// @brief デストラクタ
  ~MultiGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数の取得
  /// @return グループ数
  int
  group_num() const;

  /// @brief 全要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの全要素数
  int
  n(int grp) const;

  /// @brief 選択する要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの選択する要素数
  int
  k(int grp) const;

  /// @brief 初期化
  void
  init();

  /// @brief 要素の取得
  /// @param[in] grp グループ番号
  /// @param[in] pos 要素の位置
  /// @return grp 番目のグループの pos 番目の要素を取り出す．
  int
  operator()(int grp,
	     int pos) const;

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MultiGenBase& src);

  /// @brief 要素配列の初期化
  /// @param[in] grp グループ番号
  /// @note grp 番目のグループの要素配列を初期化する．
  void
  init_group(int grp);

  /// @brief 要素配列の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの要素配列
  Array<int>
  elem(int grp);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(int grp) const;


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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// グループ数を得る．
inline
int
MultiGenBase::group_num() const
{
  return mGroupNum;
}

// grp 番目のグループの全要素数を得る．
inline
int
MultiGenBase::n(int grp) const
{
  ASSERT_COND( grp >= 0 && grp < mGroupNum );

  return mNArray[grp];
}

// grp 番目のグループの選択する要素数を得る．
inline
int
MultiGenBase::k(int grp) const
{
  ASSERT_COND( grp >= 0 && grp < mGroupNum );

  return mKArray[grp];
}

// grp 番目のグループの pos 番目の要素を取り出す．
inline
int
MultiGenBase::operator()(int grp,
			 int pos) const
{
  ASSERT_COND( grp >= 0 && grp < mGroupNum );
  ASSERT_COND( pos >= 0 && pos < k(grp) );

  return mElemArray[mOffsetArray[grp] + pos];
}

// 末尾の時に true を返す．
inline
bool
MultiGenBase::is_end() const
{
  return is_end_sub(0);
}

// grp 番目のグループの要素配列を得る．
inline
Array<int>
MultiGenBase::elem(int g)
{
  int offset = mOffsetArray[g];
  return Array<int>(mElemArray, offset, offset + mKArray[g]);
}

// grp 番目のグループが終了状態の時 true を返す．
inline
bool
MultiGenBase::is_end_sub(int grp) const
{
  return operator()(grp, 0) == n(grp);
}

END_NAMESPACE_YM

#endif // YM_MULTIGENBASE_H
