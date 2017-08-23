#
# @file CXX_RandCombiGen.pxd
# @brief RandCombiGen 用のpxdファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016, 2017 Yusuke Matsunaga
# All rights reserved.


from CXX_RandGen cimport RandGen


cdef extern from "ym/RandCombiGen.h" namespace "nsYm" :

    # RandCombiGen クラスの cython バージョン
    cdef cppclass RandCombiGen :
        RandCombiGen(unsigned int n, unsigned int k)
        unsigned int num()
        unsigned int combi_num()
        void generate(RandGen& rg)
        unsigned int elem(unsigned int pos)
