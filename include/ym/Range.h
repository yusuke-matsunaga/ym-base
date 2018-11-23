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
template<int step = 1>
class RangeIterator
{
public:

  /// @brief コンストラクタ
  /// @param[in] pos 現在の値
  RangeIterator(int pos);

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

};


//////////////////////////////////////////////////////////////////////
/// @class Range Range.h "Range.h"
/// @brief c++11 の for 文で用いる範囲を生成するクラス
///
/// Python の range() とほぼおなじ機能
//////////////////////////////////////////////////////////////////////
template<int step = 1>
class Range_
{
public:
  typedef RangeIterator<step> iterator;

  /// @brief コンストラクタ
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  ///
  /// @code
  /// for ( int i: Range<step>(start, end) ) {
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
  Range_(int start,
	 int end);

  /// @brief 開始位置と刻み幅を省略したコンストラクタ
  /// @param[in] end 終了位置
  ///
  /// start = 0 とする．
  explicit
  Range_(int end);

  /// @brief デストラクタ
  ~Range_();


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

};

/// @brief Range_<1> の別名
using Range = Range_<1>;


//////////////////////////////////////////////////////////////////////
// RangeIterator のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pos 現在の値
// @param[in] step 刻み幅
template<int step>
inline
RangeIterator<step>::RangeIterator(int pos) :
  mCurPos(pos)
{
}

// @brief デストラクタ
template<int step>
inline
RangeIterator<step>::~RangeIterator()
{
}

// @brief dereference 演算子
template<int step>
inline
int
RangeIterator<step>::operator*() const
{
  return mCurPos;
}

// @brief increment 演算子
template<int step>
inline
const RangeIterator<step>&
RangeIterator<step>::operator++()
{
  mCurPos += step;

  return *this;
}

// @brief 等価比較演算子
template<int step>
inline
bool
RangeIterator<step>::operator==(const RangeIterator<step>& right) const
{
  return mCurPos == right.mCurPos;
}

// @brief 非等価比較演算子
template<int step>
inline
bool
RangeIterator<step>::operator!=(const RangeIterator<step>& right) const
{
  return !operator==(right);
}


//////////////////////////////////////////////////////////////////////
// Range_ のインライン関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
/// @param[in] start 開始位置
/// @param[in] end 終了位置
/// @param[in] step 刻み幅
template<int step>
inline
Range_<step>::Range_(int start,
		     int end) :
  mStart(start),
  mEnd(end)
{
  // sanity check
  if ( mStart < mEnd ) {
    ASSERT_COND( step > 0 );
  }
  else if ( mStart > mEnd ) {
    ASSERT_COND( step < 0 );
  }

  // 終了位置の補正を行う．
  if ( ((mEnd - mStart) % step) != 0 ) {
    mEnd = (((mEnd - mStart) / step) + 1) * step + mStart;
  }
}

// @brief 開始位置と刻み幅を省略したコンストラクタ
// @param[in] end 終了位置
//
// start = 0 とする．
template<int step>
inline
Range_<step>::Range_(int end) :
  mStart(0),
  mEnd(end)
{
}

// @brief デストラクタ
template<int step>
inline
Range_<step>::~Range_()
{
}

// @brief 開始位置の反復子を返す．
template<int step>
inline
RangeIterator<step>
Range_<step>::begin() const
{
  return iterator(mStart);
}

// @brief 終了位置の反復子を返す．
template<int step>
inline
RangeIterator<step>
Range_<step>::end() const
{
  return iterator(mEnd);
}

END_NAMESPACE_YM

#endif // RANGE_H
