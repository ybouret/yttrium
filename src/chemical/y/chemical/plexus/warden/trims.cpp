
#include "y/chemical/plexus/warden/trims.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Trims:: Trims(const Fund &fund) noexcept :
        reac(fund),
        prod(fund)
        {
        }

        Trims:: ~Trims() noexcept
        {
        }

        void Trims:: free() noexcept
        {
            reac.free();
            prod.free();
        }

        Trims::Kind Trims:: operator()(const XReadable   &C,
                                       const Level        L,
                                       const Components  &E,
                                       const AddressBook &conserved)
        {
            free();
            try {

                if(reac(C,L,E.reac,conserved))
                {
                    //------------------------------------------------------
                    assert(reac.required.size>0);
                    //------------------------------------------------------
                    if(prod(C,L,E.prod,conserved))
                    {
                        //--------------------------------------------------
                        assert(reac.required.size>0);
                        assert(prod.required.size>0);
                        //--------------------------------------------------
                        return BadBoth;
                    }
                    else
                    {
                        //--------------------------------------------------
                        assert(reac.required.size>0);
                        assert(prod.required.size<=0);
                        //--------------------------------------------------
                        return BadReac;
                    }

                }
                else
                {
                    //------------------------------------------------------
                    assert(reac.required.size<=0);
                    //------------------------------------------------------
                    if(prod(C,L,E.prod,conserved))
                    {
                        //--------------------------------------------------
                        assert(reac.required.size<=0);
                        assert(prod.required.size>0);
                        //--------------------------------------------------
                        return BadProd;
                    }
                    else
                    {
                        //--------------------------------------------------
                        assert(reac.required.size<=0);
                        assert(prod.required.size<=0);
                        //--------------------------------------------------
                        return BadNone;
                    }
                }
            } catch(...) {
                free(); throw;
            }
        }
    }

}

