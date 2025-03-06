

#include "y/chemical/plexus/conservation/broken.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {

            Broken:: ~Broken() noexcept {}


            Broken:: Broken(const Law  & _law,
                            const xReal  _bad,
                            XWritable  & _cok) noexcept :
            law(_law),
            bad(_bad),
            cok(_cok)
            {
                assert(bad>0.0);
            }


            Broken:: Broken(const Broken &_) noexcept :
            law(_.law),
            bad(_.bad),
            cok(_.cok)
            {
                
            }

            bool Broken:: fixed(XAdd &xadd, const XReadable &Cin) {
                bad = law.excess(xadd, Cin, AuxLevel);
                if(bad>0.0)
                {
                    law.project(xadd,cok,Cin,AuxLevel);
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }

    }

}

