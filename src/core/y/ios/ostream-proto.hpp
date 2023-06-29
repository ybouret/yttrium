//! \file

#ifndef Y_IOS_OSTREAM_PROTO_INCLUDED
#define Y_IOS_OSTREAM_PROTO_INCLUDED 1


#include <iostream>

#define Y_OSTREAM_PROTO(CLASS) friend std::ostream & operator<<(std::ostream &, const CLASS &)

#endif

