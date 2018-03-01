#ifndef RANGE_H
#define RANGE_H

/// @file Range.h
/// @brief Range のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RangeIterator Range.h "Range.h"
/// @brief Range 用の反復子
//////////////////////////////////////////////////////////////////////
class RangeIterator
{
public:

  /// @brief コンストラクタ
  /// @param[in] pos 現在の値
  /// @param[in] step 刻み幅
  RangeIterator(int pos,
		int step);

  /// @brief デストラクタ
  ~RangeIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  int
  operator*() const;

  /// @brief increment 演算子
  const RangeIterator&
  operator++();

  /// @brief 等価比較演算子
  bool
  operator==(const RangeIterator& right) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const RangeIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の位置
  int mCurPos;

  // 刻み幅
  int mStep;

};


//////////////////////////////////////////////////////////////////////
/// @class Range Range.h "Range.h"
/// @brief c++11 の for 文で用いる範囲を生成するクラス
///
/// Python の range() とほぼおなじ機能
//////////////////////////////////////////////////////////////////////
class Range
{
public:
  typedef RangeIterator iterator;

  /// @brief コンストラクタ
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  /// @param[in] step 刻み幅
  ///
  /// @code
  /// for ( int i: Range(start, end, step) ) {
  ///   ...
  /// }
  /// @endcoe
  /// のように用いる．これは
  /// @code
  /// for ( int i = start; i < end; i += step ) {
  ///   ...
  /// }
  /// @endcode
  /// と同義になる．
  /// ただし step が負の時には i > end が継続条件になる．
  ///
  /// start < end かつ step < 0 の場合などは無限ループになる．
  Range(int start,
	int end,
	int step = 1);

  /// @brief 開始位置と刻み幅を省略したコンストラクタ
  /// @param[in] end 終了位置
  ///
  /// start = 0, step = 1 とする．
  explicit
  Range(int end);

  /// @brief デストラクタ
  ~Range();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 開始位置の反復子を返す．
  iterator
  begin() const;

  /// @brief 終了位置の反復子を返す．
  iterator
  end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 開始位置
  int mStart;

  // 終了位置
  int mEnd;

  // 刻み幅
  int mStep;

};


//////////////////////////////////////////////////////////////////////
// RangeIterator のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pos 現在の値
// @param[in] step 刻み幅
inline
RangeIterator::RangeIterator(int pos,
			     int step) :
  mCurPos(pos),
  mStep(step)
{
}

// @brief デストラクタ
inline
RangeIterator::~RangeIterator()
{
}

// @brief dereference 演算子
inline
int
RangeIterator::operator*() const
{
  return mCurPos;
}

// @brief increment 演算子
inline
const RangeIterator&
RangeIterator::operator++()
{
  mCurPos += mStep;

  return *this;
}

// @brief 等価比較演算子
inline
bool
RangeIterator::operator==(const RangeIterator& right) const
{
  ASSERT_COND( mStep == right.mStep );

  return mCurPos == right.mCurPos;
}

// @brief 非等価比較演算子
inline
bool
RangeIterator::operator!=(const RangeIterator& right) const
{
  return !operator==(right);
}


//////////////////////////////////////////////////////////////////////
// Range のインライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
/// @param[in] start 開始位置
/// @param[in] end 終了位置
/// @param[in] step 刻み幅
inline
Range::Range(int start,
	     int end,
	     int step) :
  mStart(start),
  mEnd(end),
  mStep(step)
{
  // sanity check
  if ( mStart < mEnd ) {
    ASSERT_COND( mStep > 0 );
  }
  else if ( mStart > mEnd ) {
    ASSERT_COND( mStep < 0 );
  }

  // 終了位置の補正を行う．
  if ( ((mEnd - mStart) % mStep) != 0 ) {
    mEnd = (((mEnd - mStart) / mStep) + 1) * mStep + mStart;
  }
}

// @brief 開始位置と刻み幅を省略したコンストラクタ
// @param[in] end 終了位置
//
// start = 0, step = 1 とする．
Range::Range(int end) :
  mStart(0),
  mEnd(end),
  mStep(1)
{
}

// @brief デストラクタ
inline
Range::~Range()
{
}

// @brief 開始位置の反復子を返す．
inline
Range::iterator
Range::begin() const
{
  return iterator(mStart, mStep);
}

// @brief 終了位置の反復子を返す．
inline
Range::iterator
Range::end() const
{
  return iterator(mEnd, mStep);
}

END_NAMESPACE_YM

#endif // RANGE_H
