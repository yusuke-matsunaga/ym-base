﻿
/// @file Binder.cc
/// @brief Binder 関連クラスの実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Binder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 他のオブジェクトの動作に関連づけるための基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// この時点では特定の BindMgr には結び付いていない．
Binder::Binder()
{
  mMgr = nullptr;
}

// デストラクタ
// 同時にバインドも削除される．
Binder::~Binder()
{
  if ( mgr() ) {
    mgr()->_unreg_binder(this);
  }
}

// バインドしている BindMgr の取得
//
// バインドしている BindMgrを返す．
// バインドしていない場合には nullptr を返す．
BindMgr*
Binder::mgr() const
{
  return mMgr;
}


//////////////////////////////////////////////////////////////////////
// Binder を起動するクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BindMgr::BindMgr()
{
}

// デストラクタ
// ここに登録されているすべての binder のバインドは削除される．
// binder のオブジェクトは削除されない．
BindMgr::~BindMgr()
{
  _unreg_all_binders();
}

// Binder を登録する．
// @param[in] binder 登録する binder
void
BindMgr::_reg_binder(Binder* binder)
{
  BindMgr* old_src = binder->mgr();
  if ( old_src ) {
    // すでにバインドしている場合には古い bind を削除する．
    old_src->_unreg_binder(binder);
  }
  binder->mMgr = this;
  mList.push_back(binder);
  binder->mItForList = -- mList.end();
}

// @brief binder の登録を削除する．
// @param[in] binder 登録を削除する対象
// binder が登録されていない場合には何もしない．
void
BindMgr::_unreg_binder(Binder* binder)
{
  if ( binder->mgr() == this ) {
    mList.erase(binder->mItForList);
    binder->mMgr = nullptr;
  }
}

// @brief 全ての binder の登録の解除
void
BindMgr::_unreg_all_binders()
{
  for (list<Binder*>::iterator p = mList.begin();
       p != mList.end(); ++ p) {
    Binder* binder = *p;
    binder->mMgr = nullptr;
  }
  mList.clear();
}


//////////////////////////////////////////////////////////////////////
// EventBinder の継承クラスを起動するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EventBindMgr::EventBindMgr()
{
}

// @brief デストラクタ
EventBindMgr::~EventBindMgr()
{
}

// @brief EventBinder を登録する．
//
// @param[in] binder 登録する EventBinder
void
EventBindMgr::reg_binder(EventBinder* binder)
{
  _reg_binder(binder);
}

// @brief EventBinder の登録を削除する．
//
// @param[in] binder 登録を削除する EventBinder
//
// binder が登録されていない場合には何もしない．
void
EventBindMgr::unreg_binder(EventBinder* binder)
{
  _unreg_binder(binder);
}

// @brief 全ての binder の登録の解除
void
EventBindMgr::unreg_all_binders()
{
  _unreg_all_binders();
}

// @brief ここに登録されたすべての binder にイベントを伝える．
void
EventBindMgr::prop_event()
{
  for (list<Binder*>::iterator p = mList.begin(); p != mList.end(); ++ p) {
    // 本当はよくない static_cast だが reg_binder() で登録できるのは
    // EventBinder だけなので大丈夫なはず
    EventBinder* binder = static_cast<EventBinder*>(*p);
    binder->event_proc();
  }
}

END_NAMESPACE_YM
