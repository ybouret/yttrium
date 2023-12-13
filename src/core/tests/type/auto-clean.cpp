#include "y/type/auto-clean.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace {

    class Dummy
    {
    public:
        inline Dummy() noexcept {
            std::cerr << "[+Dummy]" << std::endl;
        }

        inline ~Dummy() noexcept
        {
            std::cerr << "[~Dummy]" << std::endl;
        }

        inline void clean()
        {
            std::cerr << "[@Dummy] clean" << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dummy);
    };

}

Y_UTEST(type_auto_clean)
{

    Y_SIZEOF(void *);
    Y_SIZEOF( void (Dummy::*)(void) );
    Y_SIZEOF(AutoClean::Meth);
    
    Dummy dummy;
    {
        AutoClean ac(dummy, & Dummy::clean );
    }
    std::cerr << "will end..." << std::endl;

}
Y_UDONE()

