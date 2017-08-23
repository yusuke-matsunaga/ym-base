#
# @file CXX_RandPermGen.pxd
# @brief RandPermGen 用のpxdファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016, 2017 Yusuke Matsunaga
# All rights reserved.


from CXX_RandGen cimport RandGen


cdef extern from "ym/RandPermGen.h" namespace "nsYm" :

    # RandPermGen クラスの cython バージョン
    cdef cppclass RandPermGen :
        RandPermGen(unsigned int n)
        unsigned int num()
        void generate(RandGen& rg)
        unsigned int elem(unsigned int pos)
