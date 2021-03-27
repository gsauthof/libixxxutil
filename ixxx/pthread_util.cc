// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

// SPDX-License-Identifier: BSD-2-Clause

#include "pthread_util.hh"

#include <ixxx/pthread.hh>

namespace ixxx {
    namespace util {



        Pthread_Attr::Pthread_Attr()
        {
            ixxx::posix::pthread_attr_init(&a);
        }
        Pthread_Attr::~Pthread_Attr()
        {
            ixxx::posix::pthread_attr_destroy(&a);
        }


    }
}
