#include "y/memory/solitary/specimen.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/apex/natural.hpp"

using namespace Yttrium;


Y_UTEST(memory_specimen)
{

    Memory::Specimen<int,Memory::Global>    s1;
    Memory::Specimen<String,Memory::Pooled> s2;
    Memory::Specimen<String,Memory::Dyadic> s3;

    int &a = s1.build(); a = 2; std::cerr << a << std::endl;
    String &str = s2.build(); str = "Hello"; std::cerr << str << std::endl;

    String &txt = s3.build("World"); std::cerr << txt << std::endl;

}
Y_UDONE()

