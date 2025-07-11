
/// @file ItvlTree_test.cc
/// @brief ItvlTree_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <random>
#include "ItvlTree.h"


BEGIN_NAMESPACE_YM

TEST(ItvlTree, simple_1)
{
  ItvlTree it;

  int N = 10;
  std::vector<int> id_list(N);
  for ( int i = 0; i < N; ++ i ) {
    auto id = id_list[i] = it.get_min();
    EXPECT_EQ( i, id );
    it.add(id);
  }
  for ( auto id: id_list ) {
    it.del(id);
  }
  auto id = it.get_min();
  EXPECT_EQ( 0, id );
}

TEST(ItvlTree, simple_2)
{
  ItvlTree it;

  int N = 10;
  std::vector<int> id_list(N);
  for ( int i = 0; i < N; ++ i ) {
    auto id = id_list[i] = it.get_min();
    EXPECT_EQ( i, id );
    it.add(id);
  }
  for ( int i = 1; i < N; i += 2 ) {
    auto id = id_list[i];
    it.del(id);
  }
  for ( int i = 1; i < N; i += 2 ) {
    auto id = it.get_min();
    EXPECT_EQ( i, id );
    it.add(id);
  }
}


TEST(ItvlTree, random_1)
{
  ItvlTree it;

  const int N = 10000;
  std::vector<int> id_list;
  id_list.reserve(N);

  std::mt19937_64 randgen;
  std::uniform_int_distribution<int> bd(0, 1);
  const int C = 100000;
  for ( int c = 0; c < C; ++ c ) {
    bool add = false;
    if ( id_list.size() < 10 ) {
      // 要素数が少ないときは常に追加
      add = true;
    }
    else {
      // あとはランダムに追加/削除を選ぶ
      add = bd(randgen);
    }
    if ( add ) {
      auto id = it.get_min();
      // id が本当に最小値か調べる．
      {
	int last_id = -1;
	for ( auto id1: id_list ) {
	  if ( id < id1 ) {
	    break;
	  }
	  last_id = id1;
	}
	EXPECT_EQ ( last_id + 1, id );
      }
      it.add(id);
      id_list.push_back(id);
      sort(id_list.begin(), id_list.end());
    }
    else {
      std::uniform_int_distribution<int> ud(0, id_list.size() - 1);
      int idx = ud(randgen);
      auto id = id_list[idx];
      it.del(id);
      id_list.erase(id_list.begin() + idx);
    }
  }
}

END_NAMESPACE_YM
