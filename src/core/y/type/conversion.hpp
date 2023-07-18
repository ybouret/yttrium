
//! \file
#ifndef Y_Type_Conversion_Included
#define Y_Type_Conversion_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    
    //! class conversion prototype
    template <class T,class U>
    class Conversion {
    private:
        typedef uint8_t SmallType;                      //!< small type alias
        class           LargeType { uint32_t unused; }; //!< inner large type

        static  SmallType Test( U ); //!< returned type for U
        static  LargeType Test(...); //!< returned type for other U
        static  T         MakeT();   //!< a prototype for returning T

    public:
        Conversion(); //!< or some compiler will complain about all private...
        enum
        {
            Exists   = ( sizeof( Test(MakeT()) ) == sizeof(SmallType) ), //!< type conversion is possible
            SameType = false                                             //!< with different types!
        };
    };

    //! specialized self conversion
    template <class T >
    class Conversion<T,T> {
    public:
        enum {
            Exists   = true, //!< obvious
            SameType = true  //!< obvious
        };
    };

    //! true if U is derived from T, may be T
#define Y_Is_SuperSubClass(T,U) \
(Conversion<const U *,const T *>::Exists && (!Conversion<const T *, const void *>::SameType) )

    //! true is U is derived from T, and not T
#define Y_Is_SuperSubClass_Strict(T,U) \
( Y_Is_SuperSubClass(T,U) && (!Conversion<const T *,const U *>::SameType) )

}


#endif
