// Copyright (c) 2015, Georg Sauthoff <mail@georg.so>

// SPDX-License-Identifier: BSD-2-Clause

#ifndef IXXX_PTHREAD_UTIL_HH
#define IXXX_PTHREAD_UTIL_HH

#include <pthread.h>

namespace ixxx {
    namespace util {

        class Pthread_Attr {
            public:
                Pthread_Attr();
                ~Pthread_Attr();
                Pthread_Attr(const Pthread_Attr &) =delete;
                Pthread_Attr &operator=(const Pthread_Attr &) =delete;

                pthread_attr_t *ibute() { return &a; }
            private:
                pthread_attr_t a;
        };

    }
}

#endif
