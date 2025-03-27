
#include "y/chemical/plexus/conservation/broken.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Broken:: Broken( const Law &   _law,
                            const xreal_t _xs,
                            XWritable &   _cc) noexcept :
            law(_law),
            xs(_xs),
            cc(_cc)
            {
            }

            Broken:: ~Broken() noexcept
            {
            }


            Broken:: Broken(const Broken &_) noexcept :
            law(_.law),
            xs(_.xs),
            cc(_.cc)
            {
            }

        }

    }
}


