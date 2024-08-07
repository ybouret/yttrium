//! \file

#ifndef Y_XBitRev_Included
#define Y_XBitRev_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
#include <cstring>

#include "y/fft/xbitrev/xbr4.hpp"
#include "y/fft/xbitrev/xbr8.hpp"
#include "y/fft/xbitrev/xbr16.hpp"
#include "y/fft/xbitrev/xbr32.hpp"
#include "y/fft/xbitrev/xbr64.hpp"
#include "y/fft/xbitrev/xbr128.hpp"
#include "y/fft/xbitrev/xbr256.hpp"
#include "y/fft/xbitrev/xbr512.hpp"
#include "y/fft/xbitrev/xbr1024.hpp"
#include "y/fft/xbitrev/xbr2048.hpp"
#include "y/fft/xbitrev/xbr4096.hpp"
#include "y/fft/xbitrev/xbr8192.hpp"
#include "y/fft/xbitrev/xbr16384.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! inline code for precomputed n on data[]
    //
    //
    //__________________________________________________________________________
#define Y_XBITREV_CODE() \
/**/ static const size_t Required = 2 * sizeof(T);     \
/**/ void               *tmp[ Y_WORDS_GEQ(Required) ]; \
/**/  for(size_t j=1,i=1;i<n;i+=2)                     \
/**/  {                                                \
/**/      if(j>i)                                      \
/**/      {                                            \
/**/          T * const lhs = &data[i];                \
/**/          T * const rhs = &data[j];                \
/**/          memcpy(tmp,lhs,Required);                \
/**/          memcpy(lhs,rhs,Required);                \
/**/          memcpy(rhs,tmp,Required);                \
/**/      }                                            \
/**/      size_t m=size;                               \
/**/      while (m >= 2 && j > m)                      \
/**/      {                                            \
/**/          j -= m;                                  \
/**/          m >>= 1;                                 \
/**/      }                                            \
/**/      j += m;                                      \
/**/  }                                                \
/**/  return n

    //__________________________________________________________________________
    //
    //
    //
    //! inline code for precomputed n on data1[] and data2[]
    //
    //
    //__________________________________________________________________________
#define Y_XBITREV2_CODE()                               \
/**/  static const size_t Required = 2 * sizeof(T);     \
/**/  void               *tmp[ Y_WORDS_GEQ(Required) ]; \
/**/  for(size_t j=1,i=1;i<n;i+=2)                      \
/**/  {                                                 \
/**/      if(j>i)                                       \
/**/      {                                             \
/**/          {                                         \
/**/              T * const lhs = &data1[i];            \
/**/              T * const rhs = &data1[j];            \
/**/              memcpy(tmp,lhs,Required);             \
/**/              memcpy(lhs,rhs,Required);             \
/**/              memcpy(rhs,tmp,Required);             \
/**/          }                                         \
/**/          {                                         \
/**/              T * const lhs = &data2[i];            \
/**/              T * const rhs = &data2[j];            \
/**/              memcpy(tmp,lhs,Required);             \
/**/              memcpy(lhs,rhs,Required);             \
/**/              memcpy(rhs,tmp,Required);             \
/**/          }                                         \
/**/      }                                             \
/**/      size_t m=size;                                \
/**/      while (m >= 2 && j > m)                       \
/**/      {                                             \
/**/          j -= m;                                   \
/**/          m >>= 1;                                  \
/**/      }                                             \
/**/      j += m;                                       \
/**/  }                                                 \
/**/  return n

    //__________________________________________________________________________
    //
    //
    //! helper to call inline bit reversal
    //
    //__________________________________________________________________________
#define Y_XBITREV(SIZE) case SIZE: Call<T,sizeof(XBR##SIZE)/sizeof( XBR##SIZE  [0])>(data,(const Info *)&XBR##SIZE[0]); return n;


    //__________________________________________________________________________
    //
    //
    //! helper to call inline bit reversal
    //
    //__________________________________________________________________________
#define Y_XBITREV2(SIZE) case SIZE: Call<T,sizeof(XBR##SIZE)/sizeof( XBR##SIZE  [0])>(data1,data2,(const Info *)&XBR##SIZE[0]); return n;



    //__________________________________________________________________________
    //
    //
    //
    //! Bit Reversal Algorithms
    //
    //
    //__________________________________________________________________________
    struct XBitRev
    {

        //______________________________________________________________________
        //
        //! legacy bit reversal of data[1..size*2]
        /**
         \param data [1..size*2], (re,im)[size]
         \param size a power of two
         */
        //______________________________________________________________________
        template <typename T> static inline
        size_t Run_(T data[], const size_t size) noexcept
        {
            assert(IsPowerOfTwo(size)); assert(0!=data);
            const size_t n = (size<<1);
            Y_XBITREV_CODE();
        }


        //______________________________________________________________________
        //
        //!  bit reversal of data[1..size*2] with precomputed tables
        /**
         \param data [1..size*2], (re,im)[size]
         \param size a power of two
         */
        //______________________________________________________________________
        template <typename T> static inline
        size_t Run(T data[], const size_t size) noexcept
        {
            assert(0!=data);
            assert(IsPowerOfTwo(size));

            const size_t n = (size<<1);
            switch(size)
            {
                case 0:
                case 2: return n;
                    //----------------------------------------------------------
                    Y_XBITREV(4);
                    Y_XBITREV(8);
                    Y_XBITREV(16);
                    Y_XBITREV(32);
                    Y_XBITREV(64);
                    Y_XBITREV(128);
                    Y_XBITREV(256);
                    Y_XBITREV(512);
                    Y_XBITREV(1024);
                    Y_XBITREV(2048);
                    Y_XBITREV(4096);
                    Y_XBITREV(8192);
                    Y_XBITREV(16384);
                    //----------------------------------------------------------
                default: { Y_XBITREV_CODE(); }
            }

            return n;
        }

        //______________________________________________________________________
        //
        //! legacy bit reversal of data1 and data2
        /**
         \param data1 [1..size*2], (re,im)[size]
         \param data2 [1..size*2], (re,im)[size]
         \param size a power of two
         */
        //______________________________________________________________________
        template <typename T> static inline
        size_t Run_(T data1[], T data2[], const size_t size) noexcept
        {
            assert(0!=data1); assert(0!=data2); assert(IsPowerOfTwo(size));
            const size_t n = (size<<1);
            Y_XBITREV2_CODE();
        }
        //______________________________________________________________________
        //
        //! bit reversal of data1 and data2 with prcomputed tables
        /**
         \param data1 [1..size*2], (re,im)[size]
         \param data2 [1..size*2], (re,im)[size]
         \param size a power of two
         */
        //______________________________________________________________________
        template <typename T> static inline
        size_t Run(T data1[], T data2[], const size_t size) noexcept
        {
            assert(0!=data1);
            assert(0!=data2);
            assert(IsPowerOfTwo(size));

            const size_t n = (size<<1);
            switch(size)
            {
                case 0:
                case 2:
                    return n;
                    //----------------------------------------------------------
                    Y_XBITREV2(4);
                    Y_XBITREV2(8);
                    Y_XBITREV2(16);
                    Y_XBITREV2(32);
                    Y_XBITREV2(64);
                    Y_XBITREV2(128);
                    Y_XBITREV2(256);
                    Y_XBITREV2(512);
                    Y_XBITREV2(1024);
                    Y_XBITREV2(2048);
                    Y_XBITREV2(4096);
                    Y_XBITREV2(8192);
                    Y_XBITREV2(16384);
                    //----------------------------------------------------------
                default: { Y_XBITREV2_CODE(); }
            }
            return n;
        }

    private:
        struct Info { uint16_t i; uint16_t j; };

        template <typename T,const unsigned N>
        static inline void Call(T data[], const Info * const xbr) noexcept
        {
            static const size_t Required = 2 * sizeof(T);
            void               *tmp[ Y_WORDS_GEQ(Required) ];
            for(unsigned k=0;k<N;++k)
            {
                const Info &_ = xbr[k];
                T * const lhs = &data[_.i];
                T * const rhs = &data[_.j];
                memcpy(tmp,lhs,Required);
                memcpy(lhs,rhs,Required);
                memcpy(rhs,tmp,Required);
            }
        }

        template <typename T,const unsigned N>
        static inline void Call2(T data1[],  T data2[], const Info * const xbr) noexcept
        {
            static const size_t Required = 2 * sizeof(T);
            void               *tmp[ Y_WORDS_GEQ(Required) ];
            for(unsigned k=0;k<N;++k)
            {
                const Info    &_ = xbr[k];
                const unsigned i = _.i;
                const unsigned j = _.j;
                {
                    T * const lhs = &data1[i];
                    T * const rhs = &data1[j];
                    memcpy(tmp,lhs,Required);
                    memcpy(lhs,rhs,Required);
                    memcpy(rhs,tmp,Required);
                }
                {
                    T * const lhs = &data2[i];
                    T * const rhs = &data2[j];
                    memcpy(tmp,lhs,Required);
                    memcpy(lhs,rhs,Required);
                    memcpy(rhs,tmp,Required);
                }
            }
        }


    };
}

#endif

