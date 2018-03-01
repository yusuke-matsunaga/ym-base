#ifndef YM_RANDPERMGEN_H
#define YM_RANDPERMGEN_H

/// @file ym/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

class RandGen;

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

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief ランダムな順列を生成する．
  /// @param[in] randgen 乱数発生器
  void
  generate(RandGen& randgen);

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

END_NAMESPACE_YM

#endif // YM_RANDPERMGEN_H
