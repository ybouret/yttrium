#include "y/type/list.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace Yttrium;


template <typename T>
static inline void showTI(const char *name)
{
    std::cerr << std::setw(20) << name << " = " << typeid(T).name() << std::endl;
}

#define SHOW_TI(TYPE) showTI< TYPE >(#TYPE)

Y_UTEST(type_list)
{
    typedef  TL1(int) L1;
    typedef  TL2(int,float) L2;
    typedef  TL3(int,float,double) L3;
    typedef  TL4(int,float,double,char) L4;
    typedef  TL5(int,float,double,char,void *) L5;
    typedef  TL6(int,float,double,char,void *,long) L6;

    std::cerr << "LengthOf" << std::endl;
    Y_USHOW(TL::LengthOf<L1>::Value);
    Y_USHOW(TL::LengthOf<L2>::Value);
    Y_USHOW(TL::LengthOf<L3>::Value);
    Y_USHOW(TL::LengthOf<L4>::Value);
    Y_USHOW(TL::LengthOf<L5>::Value);
    Y_USHOW(TL::LengthOf<L6>::Value);
    std::cerr << std::endl;

    std::cerr << "TypeAt" << std::endl;
    { typedef TL::TypeAt<L1,0>::Result L1_0; SHOW_TI(L1_0); }

    std::cerr << std::endl;
    { typedef TL::TypeAt<L2,0>::Result L2_0; SHOW_TI(L2_0); }
    { typedef TL::TypeAt<L2,1>::Result L2_1; SHOW_TI(L2_1); }

    std::cerr << std::endl;
    { typedef TL::TypeAt<L3,0>::Result L3_0; SHOW_TI(L3_0); }
    { typedef TL::TypeAt<L3,1>::Result L3_1; SHOW_TI(L3_1); }
    { typedef TL::TypeAt<L3,2>::Result L3_2; SHOW_TI(L3_2); }


    std::cerr << std::endl;
    { typedef TL::TypeAt<L4,0>::Result L4_0; SHOW_TI(L4_0); }
    { typedef TL::TypeAt<L4,1>::Result L4_1; SHOW_TI(L4_1); }
    { typedef TL::TypeAt<L4,2>::Result L4_2; SHOW_TI(L4_2); }
    { typedef TL::TypeAt<L4,3>::Result L4_3; SHOW_TI(L4_3); }

    std::cerr << std::endl;
    { typedef TL::TypeAt<L5,0>::Result L5_0; SHOW_TI(L5_0); }
    { typedef TL::TypeAt<L5,1>::Result L5_1; SHOW_TI(L5_1); }
    { typedef TL::TypeAt<L5,2>::Result L5_2; SHOW_TI(L5_2); }
    { typedef TL::TypeAt<L5,3>::Result L5_3; SHOW_TI(L5_3); }
    { typedef TL::TypeAt<L5,4>::Result L5_4; SHOW_TI(L5_4); }

    std::cerr << std::endl;
    { typedef TL::TypeAt<L6,0>::Result L6_0; SHOW_TI(L6_0); }
    { typedef TL::TypeAt<L6,1>::Result L6_1; SHOW_TI(L6_1); }
    { typedef TL::TypeAt<L6,2>::Result L6_2; SHOW_TI(L6_2); }
    { typedef TL::TypeAt<L6,3>::Result L6_3; SHOW_TI(L6_3); }
    { typedef TL::TypeAt<L6,4>::Result L6_4; SHOW_TI(L6_4); }
    { typedef TL::TypeAt<L6,5>::Result L6_5; SHOW_TI(L6_5); }
    std::cerr << std::endl;


    std::cerr << "SafeTypeAt" << std::endl;
    { typedef TL::SafeTypeAt<L1,0>::Result L1_0; SHOW_TI(L1_0); }
    { typedef TL::SafeTypeAt<L1,1>::Result L1_1; SHOW_TI(L1_1); }

    std::cerr << std::endl;
    { typedef TL::SafeTypeAt<L2,0>::Result L2_0; SHOW_TI(L2_0); }
    { typedef TL::SafeTypeAt<L2,1>::Result L2_1; SHOW_TI(L2_1); }
    { typedef TL::SafeTypeAt<L2,2>::Result L2_2; SHOW_TI(L2_2); }

    std::cerr << std::endl;
    { typedef TL::SafeTypeAt<L3,0>::Result L3_0; SHOW_TI(L3_0); }
    { typedef TL::SafeTypeAt<L3,1>::Result L3_1; SHOW_TI(L3_1); }
    { typedef TL::SafeTypeAt<L3,2>::Result L3_2; SHOW_TI(L3_2); }
    { typedef TL::SafeTypeAt<L3,3>::Result L3_3; SHOW_TI(L3_3); }


    std::cerr << std::endl;
    { typedef TL::SafeTypeAt<L4,0>::Result L4_0; SHOW_TI(L4_0); }
    { typedef TL::SafeTypeAt<L4,1>::Result L4_1; SHOW_TI(L4_1); }
    { typedef TL::SafeTypeAt<L4,2>::Result L4_2; SHOW_TI(L4_2); }
    { typedef TL::SafeTypeAt<L4,3>::Result L4_3; SHOW_TI(L4_3); }
    { typedef TL::SafeTypeAt<L4,4>::Result L4_4; SHOW_TI(L4_4); }

    std::cerr << std::endl;
    { typedef TL::SafeTypeAt<L5,0>::Result L5_0; SHOW_TI(L5_0); }
    { typedef TL::SafeTypeAt<L5,1>::Result L5_1; SHOW_TI(L5_1); }
    { typedef TL::SafeTypeAt<L5,2>::Result L5_2; SHOW_TI(L5_2); }
    { typedef TL::SafeTypeAt<L5,3>::Result L5_3; SHOW_TI(L5_3); }
    { typedef TL::SafeTypeAt<L5,4>::Result L5_4; SHOW_TI(L5_4); }
    { typedef TL::SafeTypeAt<L5,5>::Result L5_5; SHOW_TI(L5_5); }

    std::cerr << std::endl;
    { typedef TL::SafeTypeAt<L6,0>::Result L6_0; SHOW_TI(L6_0); }
    { typedef TL::SafeTypeAt<L6,1>::Result L6_1; SHOW_TI(L6_1); }
    { typedef TL::SafeTypeAt<L6,2>::Result L6_2; SHOW_TI(L6_2); }
    { typedef TL::SafeTypeAt<L6,3>::Result L6_3; SHOW_TI(L6_3); }
    { typedef TL::SafeTypeAt<L6,4>::Result L6_4; SHOW_TI(L6_4); }
    { typedef TL::SafeTypeAt<L6,5>::Result L6_5; SHOW_TI(L6_5); }
    { typedef TL::SafeTypeAt<L6,6>::Result L6_6; SHOW_TI(L6_6); }

}
Y_UDONE()

