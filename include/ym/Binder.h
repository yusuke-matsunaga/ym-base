#ifndef YM_BINDER_H
#define YM_BINDER_H

/// @file ym/Binder.h
/// @brief Binder 関連クラスのヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BinderGroup 汎用バインダ
/// @ingroup ym
///
/// オブジェクトの動作を他のオブジェクトに関連づけるメカニズム
///
/// よくあるデザインパターンの一種で，クラス A のオブジェクトのある動作
/// にクラス B のオブジェクトのある動作を関連づけたいが，
/// クラス A のコード中に直接 クラス B のメンバ関数の呼び出しを書いて
/// しまうと依存関係が生じてしまうのでやりたくない，という時に用いる．
/// 具体的にはクラス A 側に BindMgr (の派生クラス) を用意しておいて
/// クラス A から呼び出してもらいたいクラスはバインダーをこの BindMgr
/// に登録する．クラス A の BindMgr はイベントを起動するタイミングで
/// 登録されているバインダーに対してイベントの通知を行う．
///
/// ここではイベントのみを伝える EventBinder とn個(nは 1 ... 6)の
/// データを伝えるテンプレートクラス T<n>Binder を用意している．

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BindMgr;

class EventBindMgr;

template <typename T>
class T1BindMgr;

template <typename T1,
	  typename T2>
class T2BindMgr;

template <typename T1,
	  typename T2,
	  typename T3>
class T3BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4>
class T4BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5BindMgr;

template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6BindMgr;


//////////////////////////////////////////////////////////////////////
/// @class Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 他のオブジェクトの動作に関連づけるための基底クラス
/// @sa BindMgr
//////////////////////////////////////////////////////////////////////
class Binder
{
  friend class BindMgr;

public:

  /// @brief コンストラクタ
  ///
  /// この時点では特定の BindMgr には結び付いていない．
  Binder() = default;

  /// @brief デストラクタ
  ///
  /// BindMgr にバインドしていたらバインドを解除する．
  virtual
  ~Binder();

  /// @brief BindMgr の取得
  /// @retval バインドしている BindMgr : バインドしている場合
  /// @retval nullptr : バインドしていない場合
  BindMgr*
  mgr() const { return mMgr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // このオブジェクトを持っているクラス
  BindMgr* mMgr{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief Binder を起動するクラスの基底クラス
/// @sa Binder
//////////////////////////////////////////////////////////////////////
class BindMgr
{
  friend class Binder;

public:

  /// @brief コンストラクタ
  BindMgr() = default;

  /// @brief デストラクタ
  ///
  /// - バインド中のすべての Binder オブジェクトとのバインドを解除する．
  /// - Binder オブジェクトそのものは削除しない．
  virtual
  ~BindMgr();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief binder の登録
  void
  _reg_binder(
    Binder* binder ///< [in] 登録する Binder
  );

  /// @brief binder の登録の解除
  void
  _unreg_binder(
    Binder* binder ///< [in] 登録を解除する Binder
  );

  /// @brief 全ての binder の登録の解除
  void
  _unreg_all_binders();


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief バインドされた binder のリスト
  std::vector<Binder*> mList;

};


//////////////////////////////////////////////////////////////////////
/// @class EventBinder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数なしのイベントを伝播するためのクラス
///
/// 引数がある場合には下の TBinder を使うこと.<br>
/// 実際に使うには event_proc() を上書きした継承クラスを用意する
/// 必要がある.
/// @sa EventBindMgr
/// @sa T1Binder, T2Binder, T3Binder, T4Binder, T5Binder, T6Binder
//////////////////////////////////////////////////////////////////////
class EventBinder :
  public Binder
{
  friend class EventBindMgr;

private:

  /// @brief イベント処理関数
  ///
  /// 継承クラスが実装しなければならない．
  virtual
  void
  event_proc() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EventBindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief EventBinder の継承クラスを起動するクラス
///
/// reg_binder() でバインダーオブジェクトを登録して
/// prop_event() でイベントを伝播する．
/// @sa EventBinder
//////////////////////////////////////////////////////////////////////
class EventBindMgr :
  private BindMgr
{
public:

  /// @brief コンストラクタ
  EventBindMgr();

  /// @brief デストラクタ
  ~EventBindMgr();


public:

  /// @brief EventBinder を登録する．
  void
  reg_binder(
    EventBinder* binder ///< [in] 登録する EventBinder
  );

  /// @brief EventBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    EventBinder* binder ///< [in] 登録を削除する EventBinder
  );

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders();

  /// @brief ここに登録されたすべての binder にイベントを伝える．
  void
  prop_event();

};


//////////////////////////////////////////////////////////////////////
/// @class T1Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を1つ持つ Binder
/// @sa T1BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1>
class T1Binder :
  public Binder
{
  friend class T1BindMgr<T1>;
private:

  /// @brief イベント処理関数
  virtual
  void
  event_proc(
    T1 obj ///< [in] ブロードキャストする情報を表すオブジェクト
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T1BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T1Binder を起動するクラス
/// @sa T1Binder
//////////////////////////////////////////////////////////////////////
template<typename T1>
class T1BindMgr :
  private BindMgr
{
public:

  using TBinder = T1Binder<T1>;

public:

  /// @brief コンストラクタ
  T1BindMgr() = default;

  /// @brief デストラクタ
  ~T1BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に T1 のオブジェクトを伝える．
  void
  prop_event(
    T1 data ///< [in] ブロードキャストする内容
  )
  {
    for ( auto p: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(p);
      binder->event_proc(data);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T2Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を2つ持つ Binder
/// @sa T2BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2>
class T2Binder :
  public Binder
{
  friend class T2BindMgr<T1, T2>;
private:

  /// @brief イベント処理関数
  /// @param[in] obj1, obj2 ブロードキャストする情報を表すオブジェクト
  virtual
  void
  event_proc(
    T1 obj1, ///< [in] ブロードキャストするデータ1
    T2 obj2 ///< [in] ブロードキャストするデータ2
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T2BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T2Binder を起動するクラス
/// @sa T2Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2>
class T2BindMgr :
  private BindMgr
{
public:

  using TBinder = T2Binder<T1, T2>;

public:

  /// @brief コンストラクタ
  T2BindMgr() = default;

  /// @brief デストラクタ
  ~T2BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に (T1, T2) を伝える．
  void
  prop_event(
    T1 data1, ///< [in] ブロードキャストするデータ1
    T2 data2  ///< [in] ブロードキャストするデータ2
  )
  {
    for ( auto tmp: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(tmp);
      binder->event_proc(data1, data2);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T3Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を3つ持つ Binder
/// @sa T3BindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3>
class T3Binder :
  public Binder
{
  friend class T3BindMgr<T1, T2, T3>;
private:

  /// @brief イベント処理関数
  virtual
  void
  event_proc(
    T1 obj1, ///< [in] ブロードキャストするデータ1
    T2 obj2, ///< [in] ブロードキャストするデータ2
    T3 obj3  ///< [in] ブロードキャストするデータ3
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T3BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T3Binder を起動するクラス
/// @sa T3Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3>
class T3BindMgr :
  private BindMgr
{
public:

  using TBinder = T3Binder<T1, T2, T3>;

public:

  /// @brief コンストラクタ
  T3BindMgr() = default;

  /// @brief デストラクタ
  ~T3BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinderの登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に (T1, T2, T3) を伝える．
  void
  prop_event(
    T1 data1, ///< [in] ブロードキャストするデータ1
    T2 data2, ///< [in] ブロードキャストするデータ2
    T3 data3  ///< [in] ブロードキャストするデータ3
  )
  {
    for ( auto tmp: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(tmp);
      binder->event_proc(data1, data2, data3);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T4Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を4つ持つ Binder
/// @sa TBindMgr
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
class T4Binder :
  public Binder
{
  friend class T4BindMgr<T1, T2, T3, T4>;
private:

  /// @brief イベント処理関数
  virtual
  void
  event_proc(
    T1 obj1, ///< [in] ブロードキャストするデータ1
    T2 obj2, ///< [in] ブロードキャストするデータ2
    T3 obj3, ///< [in] ブロードキャストするデータ3
    T4 obj4  ///< [in] ブロードキャストするデータ4
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T4BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T4Binder を起動するクラス
/// @sa T4Binder
//////////////////////////////////////////////////////////////////////
template<typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
class T4BindMgr :
  private BindMgr
{
public:

  using TBinder = T4Binder<T1, T2, T3, T4>;

public:

  /// @brief コンストラクタ
  T4BindMgr() = default;

  /// @brief デストラクタ
  ~T4BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4) を伝える．
  void
  prop_event(
    T1 data1, ///< [in] ブロードキャストするデータ1
    T2 data2, ///< [in] ブロードキャストするデータ2
    T3 data3, ///< [in] ブロードキャストするデータ3
    T4 data4  ///< [in] ブロードキャストするデータ4
  )
  {
    for ( auto tmp: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(tmp);
      binder->event_proc(data1, data2, data3, data4);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T5Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を5つ持つ Binder
/// @sa T5BindMgr
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5Binder :
  public Binder
{
  friend class T5BindMgr<T1, T2, T3, T4, T5>;
private:

  /// @brief イベント処理関数
  virtual
  void
  event_proc(
    T1 obj1, ///< [in] ブロードキャストするデータ1
    T2 obj2, ///< [in] ブロードキャストするデータ2
    T3 obj3, ///< [in] ブロードキャストするデータ3
    T4 obj4, ///< [in] ブロードキャストするデータ4
    T5 obj5  ///< [in] ブロードキャストするデータ5
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T5BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T5Binder を起動するクラス
/// @sa T5Binder
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class T5BindMgr :
  private BindMgr
{
public:

  using TBinder = T5Binder<T1, T2, T3, T4, T5>;

public:

  /// @brief コンストラクタ
  T5BindMgr() = default;

  /// @brief デストラクタ
  ~T5BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4, T5) を伝える．
  void
  prop_event(
    T1 data1, ///< [in] ブロードキャストするデータ1
    T2 data2, ///< [in] ブロードキャストするデータ2
    T3 data3, ///< [in] ブロードキャストするデータ3
    T4 data4, ///< [in] ブロードキャストするデータ4
    T5 data5  ///< [in] ブロードキャストするデータ5
  )
  {
    for ( auto tmp: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(tmp);
      binder->event_proc(data1, data2, data3, data4, data5);
    }
  }
};


//////////////////////////////////////////////////////////////////////
/// @class T6Binder Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief 引数を6つ持つ Binder
/// @sa T6BindMgr
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6Binder :
  public Binder
{
  friend class T6BindMgr<T1, T2, T3, T4, T5, T6>;
private:

  /// @brief イベント処理関数
  virtual
  void
  event_proc(
    T1 obj1, ///< [in] ブロードキャストするデータ1
    T2 obj2, ///< [in] ブロードキャストするデータ2
    T3 obj3, ///< [in] ブロードキャストするデータ3
    T4 obj4, ///< [in] ブロードキャストするデータ4
    T5 obj5, ///< [in] ブロードキャストするデータ5
    T6 obj6  ///< [in] ブロードキャストするデータ6
  ) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class T6BindMgr Binder.h "ym/Binder.h"
/// @ingroup BinderGroup
/// @brief T6Binder を起動するクラス
/// @sa T6Binder
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class T6BindMgr :
  private BindMgr
{
public:

  using TBinder = T6Binder<T1, T2, T3, T4, T5, T6>;

public:

  /// @brief コンストラクタ
  T6BindMgr() = default;

  /// @brief デストラクタ
  ~T6BindMgr() = default;


public:

  /// @brief TBinder を登録する．
  void
  reg_binder(
    TBinder* binder ///< [in] 登録する対象
  )
  {
    _reg_binder(binder);
  }

  /// @brief TBinder の登録を削除する．
  ///
  /// binder が登録されていない場合には何もしない．
  void
  unreg_binder(
    TBinder* binder ///< [in] 登録を削除する対象
  )
  {
    _unreg_binder(binder);
  }

  /// @brief 全ての binder の登録の解除
  void
  unreg_all_binders() { _unreg_all_binders(); }

  /// @brief ここに登録されたすべての binder に (T1, T2, T3, T4, T5, T6) を伝える．
  void
  prop_event(
    T1 data1, ///< [in] ブロードキャストするデータ1
    T2 data2, ///< [in] ブロードキャストするデータ2
    T3 data3, ///< [in] ブロードキャストするデータ3
    T4 data4, ///< [in] ブロードキャストするデータ4
    T5 data5, ///< [in] ブロードキャストするデータ5
    T6 data6  ///< [in] ブロードキャストするデータ6
  )
  {
    for ( auto tmp: mList ) {
      // 本当はよくない static_cast だが reg_binder() で登録できるのは
      // TBinder だけなので大丈夫なはず
      auto binder = static_cast<TBinder*>(tmp);
      binder->event_proc(data1, data2, data3, data4, data5, data6);
    }
  }
};

END_NAMESPACE_YM

#endif // YM_BINDER_H
