#
# @file CXX_RandSampler.pxd
# @brief RandSampler 用のpxdファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016, 2017 Yusuke Matsunaga
# All rights reserved.


from CXX_RandGen cimport RandGen


cdef extern from "ym/RandSampler.h" namespace "nsYm" :

    # RandSampler クラスの cython バージョン
    cdef cppclass RandSampler :
        RandSampler(unsigned int num, unsigned int* weight_array)
        unsigned int num()
        unsigned int weight(unsigned int)
        unsigned int get_sample(RandGen& rg)
