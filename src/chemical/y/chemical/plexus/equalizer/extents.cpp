
#include "y/chemical/plexus/equalizer/extents.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Extents:: Extents(const EqzBanks &banks) noexcept :
        reac(banks),
        prod(banks),
        best(banks.sb)
        {
        }

        Extents:: ~Extents() noexcept
        {
        }

        void Extents:: restart() noexcept {
            reac.restart();
            prod.restart();
            best.restart();
        }


        Resultant Extents:: operator()(const Components &  E,
                                  const XReadable &   C,
                                  const Level         L,
                                  const AddressBook * const wanders)
        {
            try
            {
                best.restart();
                reac(E.reac,C,L,wanders);
                prod(E.prod,C,L,wanders);

                if(reac.required->size<=0)
                {

                    if(prod.required->size<=0)
                    {
                        // no negative concerned concentration
                        return Correct;
                    }
                    else
                    {
                        // only some negative product(s)
                        findBest(reac.limiting,prod.required);
                        return BadProd;
                    }
                }
                else
                {
                    assert(reac.required->size>0);
                    if(prod.required->size<=0)
                    {
                        // only some negative reactant(s)
                        findBest(prod.limiting,reac.required);
                        return BadReac;
                    }
                    else
                    {
                        // blocked
                        return BadBoth;
                    }
                }



            }
            catch(...)
            {
                restart();
                throw;
            }

        }

        void Extents:: findBest(const Boundary &limiting, const Cursors &required)
        {

        }

    }

}

