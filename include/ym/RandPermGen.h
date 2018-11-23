#ifndef YM_RANDPERMGEN_H
#define YM_RANDPERMGEN_H

/// @file ym/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/Range.h"
#include <random>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RandPermGen RandGen.h "RandGen.h"
/// @brief ランダムな順列を作るクラス
//////////////////////////////////////////////////////////////////////
class RandPermGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 要素数
  RandPermGen(int n);

  /// @brief デストラクタ
  ~RandPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief ランダムな順列を生成する．
  /// @param[in] randgen 乱数発生器
  template<class URNG>
  void
  generate(URNG& randgen);

  /// @brief 順列の要素を取り出す．
  /// @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
  int
  elem(int pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

  // 現在の順列
  int* mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] n 要素数
inline
RandPermGen::RandPermGen(int n) :
  mNum{n},
  mArray{new int[n]}
{
}

// @brief デストラクタ
inline
RandPermGen::~RandPermGen()
{
  delete [] mArray;
}

// @brief 要素数を返す．
inline
int
RandPermGen::num() const
{
  return mNum;
}

// @brief ランダムな順列を生成する．
template<class URNG>
inline
void
RandPermGen::generate(URNG& randgen)
{
  vector<int> src_array(mNum);
  for ( int i: Range(mNum) ) {
    src_array[i] = i;
  }
  int n = mNum - 1;
  for ( int i: Range(mNum) ) {
    std::uniform_int_distribution<int> rd(0, n);
    int r = rd(randgen);
    mArray[i] = src_array[r];
    for ( int j: Range(r, n) ) {
      src_array[j] = src_array[j + 1];
    }
    -- n;
  }
}

// @brief 順列の要素を取り出す．
// @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
inline
int
RandPermGen::elem(int pos) const
{
  return mArray[pos];
}


END_NAMESPACE_YM

#endif // YM_RANDPERMGEN_H
