
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


        Resultant Extents:: operator()(XMLog&              xml,
                                       const Components &  E,
                                       const XReadable  &  C,
                                       const Level         L,
                                       const AddressBook * const wanders)
        {
            try
            {
                best.restart();
                reac(E.reac,C,L,wanders);
                prod(E.prod,C,L,wanders);
               // std::cerr << "reac:" << reac << std::endl;
                //std::cerr << "prod:" << prod << std::endl;
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
                        //std::cerr << "best=" << best << std::endl;
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
                        best.xi.neg();
                        //std::cerr << "best=" << best << std::endl;
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

            // look for partial solution
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
        }

        xreal_t Extents:: generate(XAdd             &xadd,
                                XWritable        &Csub,
                                const Components &E,
                                const XReadable  &C,
                                const Level       L,
                                const AddressBook * const wanders) const
        {
            assert(best.size>0);
            std::cerr << "best=" << best << std::endl;
            E.displayCompact( std::cerr << "bad = ", Csub, SubLevel) << std::endl;

            // move according to best
            E.boldMove(Csub, SubLevel, best.xi);

            // ensure vanishing
            for(const SNode *sn=best.head;sn;sn=sn->next)
            {
                (**sn)(Csub,SubLevel) = 0;
            }
            E.displayCompact( std::cerr << "eqz = ", Csub, SubLevel) << std::endl;

            // compute gain for not wandering species
            const xreal_t zero;
            xadd.free();
            {
                size_t n = E->size();
                for(Components::ConstIterator it=E->begin();n-- > 0;++it)
                {
                    const Species &sp = (**it).sp; if(wanders && wanders->has(sp)) continue;
                    const xreal_t c_old = sp(C,L);
                    if(c_old<zero)
                    {
                        const xreal_t c_new = sp(Csub,SubLevel);
                        const xreal_t start = -c_old;
                        xadd << c_new;
                        xadd << start;
                    }
                }
            }
            const xreal_t gain = Max(xadd.sum(),zero);
            std::cerr << "gain=" << gain.str() << std::endl;
            return gain;
        }

    }

}

