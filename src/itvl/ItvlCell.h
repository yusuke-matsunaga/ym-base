#ifndef ITVLCELL_H
#define ITVLCELL_H

/// @file ItvlCell.h
/// @brief ItvlCell のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// インターバルを表す要素クラス
//////////////////////////////////////////////////////////////////////
class ItvlCell
{
  friend class ItvlMgrImpl;

private:

  /// @brief 開始点と終了点を指定したコンストラクタ
  /// @param[in] start 開始点
  /// @param[in] end 終了点
  ItvlCell(int start,
	   int end);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 開始点を得る．
  int
  start() const;

  /// @brief 終了点を得る．
  int
  end() const;

  /// @brief 平衡度を得る．
  int
  balance() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 開始点
  int mStart;

  // 終了点
  int mEnd;

  // 平衡度
  ymint8 mBalance;

  // 左の子供
  ItvlCell* mLchd;

  // 右の子供
  ItvlCell* mRchd;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 開始点と終了点を指定したコンストラクタ
// @param[in] start 開始点
// @param[in] end 終了点
inline
ItvlCell::ItvlCell(int start,
		   int end) :
  mStart(start),
  mEnd(end),
  mLchd(nullptr),
  mRchd(nullptr)
{
}

// @brief 開始点を得る．
inline
int
ItvlCell::start() const
{
  return mStart;
}

// @brief 終了点を得る．
inline
int
ItvlCell::end() const
{
  return mEnd;
}

// @brief 平衡度を得る．
inline
int
ItvlCell::balance() const
{
  return static_cast<int>(mBalance);
}

END_NAMESPACE_YM

#endif // ITVLCELL_H
