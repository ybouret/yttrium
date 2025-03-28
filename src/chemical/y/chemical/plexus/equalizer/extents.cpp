
#include "y/chemical/plexus/equalizer/extents.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

#define Y_Case(ID) case ID: return #ID
        const char * ResultantText(const Resultant res) noexcept
        {
            switch(res)
            {
                    Y_Case(Correct);
                    Y_Case(BadReac);
                    Y_Case(BadProd);
                    Y_Case(BadBoth);
            }
            return Core::Unknown;
        }

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
                std::cerr << "reac:" << reac << std::endl;
                std::cerr << "prod:" << prod << std::endl;
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
                        best.xi.neg();
                        std::cerr << "best=" << best << std::endl;
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
                        std::cerr << "best=" << best << std::endl;
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
            std::cerr << "Find " << limiting << " within " << required << std::endl;
            assert(required->size>0);
            assert(0==best.size);

            for(const CrNode *cn=required->head;cn;cn=cn->next)
            {
                const Cursor &cr = **cn;
                switch( Sign::Of(limiting.xi, cr.xi) )
                {
                    case Negative: best = limiting;               return; //
                    case __Zero__: best = limiting; best.add(cr); return; // special partial/full
                    case Positive: continue;
                }
            }

            // full
            best = **(required->tail);


#if 0
            const CrNode *lower = required->head;
            if(1==required->size)
            {
                const Cursor &cr = **lower;
                switch( Sign::Of(limiting.xi, cr.xi) )
                {
                    case Negative: best = limiting;               break; // partial
                    case __Zero__: best = limiting; best.add(cr); break; // special exact
                    case Positive: best = cr;                     break; // fully solved
                }
            }
            else
            {
                assert(required->size>=2);
                throw Exception("need to work");
            }
#endif

        }

    }

}

