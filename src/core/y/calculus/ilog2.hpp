
//! \file
#ifndef Y_Calculus_ILOG2_INCLUDED
#define Y_Calculus_ILOG2_INCLUDED 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //! integer log2
    template <size_t> struct iLog2;

    //! sizeof=1
    template <> struct iLog2<1>    { enum { Value = 0  }; };
    //! sizeof=2
    template <> struct iLog2<2>    { enum { Value = 1  }; };
    //! sizeof=4
    template <> struct iLog2<4>    { enum { Value = 2  }; };
    //! sizeof=8
    template <> struct iLog2<8>    { enum { Value = 3  }; };
    //! sizeof=16
    template <> struct iLog2<16>   { enum { Value = 4  }; };
    //! sizeof=32
    template <> struct iLog2<32>   { enum { Value = 5  }; };
    //! sizeof=64
    template <> struct iLog2<64>   { enum { Value = 6  }; };
    //! sizeof=128
    template <> struct iLog2<128>  { enum { Value = 7  }; };
    //! sizeof=256
    template <> struct iLog2<256>  { enum { Value = 8  }; };
    //! sizeof=512
    template <> struct iLog2<512>  { enum { Value = 9  }; };
    //! sizeof=1024
    template <> struct iLog2<1024> { enum { Value = 10 }; };
    //! sizeof=2048
    template <> struct iLog2<2048> { enum { Value = 11 }; };
    //! sizeof=4096
    template <> struct iLog2<4096> { enum { Value = 12 }; };
    //! sizeof=8192
    template <> struct iLog2<8192> { enum { Value = 13 }; };
    //! sizeof=16384
    template <> struct iLog2<16384> { enum { Value = 14 }; };
    //! sizeof=32768
    template <> struct iLog2<32768> { enum { Value = 15 }; };
    //! sizeof=65536
    template <> struct iLog2<65536> { enum { Value = 16 }; };


    //! proxy to get ilog2 using the type size
    template <typename T>
    struct iLog2Of { enum { Value = iLog2<sizeof(T)>::Value }; };


}
#endif
