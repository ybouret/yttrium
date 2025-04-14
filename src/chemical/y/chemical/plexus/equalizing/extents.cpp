
#include "y/chemical/plexus/equalizing/extents.hpp"
#include "y/exception.hpp"
#include "y/type/momentary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
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

            Extents:: Extents(const Banks &banks) noexcept :
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
                                           const AddressBook  &wanders)
            {
                volatile Momentary::Off turnOff( Coerce(xml.verbose) );
                Y_XML_SECTION_OPT(xml, "Extents", E.name);
                try
                {
                    best.restart();
                    reac(E.reac,C,L,wanders);
                    prod(E.prod,C,L,wanders);
                    Y_XMLOG(xml, "(*) reactants :" << reac);
                    Y_XMLOG(xml, "(*) products  :" << prod);

                    
                    if(reac.required->size<=0)
                    {

                        if(prod.required->size<=0)
                        {
                            // no negative concerned concentration
                            Y_XML_COMMENT(xml, ResultantText(Correct) );
                            return Correct;
                        }
                        else
                        {
                            // only some negative product(s)
                            Y_XML_COMMENT(xml, ResultantText(BadProd) );
                            findBest(xml,reac.limiting,prod.required);
                            return BadProd;
                        }
                    }
                    else
                    {
                        assert(reac.required->size>0);
                        if(prod.required->size<=0)
                        {
                            // only some negative reactant(s)
                            Y_XML_COMMENT(xml, ResultantText(BadReac) );
                            findBest(xml,prod.limiting,reac.required);
                            best.xi.neg();
                            return BadReac;
                        }
                        else
                        {
                            // blocked
                            Y_XML_COMMENT(xml, ResultantText(BadBoth) );
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

            void Extents:: findBest(XMLog &xml, const Boundary &limiting, const Cursors &required)
            {
                static const char partial[] = "partial : ";
                static const char reached[] = "reached : ";

                //volatile Momentary::Off turnOff( Coerce(xml.verbose) );

                assert(limiting.size>0);
                assert(required->size>0);
                assert(0==best.size);


                Y_XML_COMMENT(xml,"find " << limiting << " within " << required);
                // look for partial solution
                for(const CrNode *cn=required->head;cn;cn=cn->next)
                {
                    const Cursor &cr = **cn;
                    switch( Sign::Of(limiting.xi, cr.xi) )
                    {
                        case Negative: best = limiting;
                            Y_XMLOG(xml, partial << best);
                            return; // partial

                        case __Zero__:
                            best = limiting;
                            best.add(cr);
                            if(required->tail==cn)
                            {
                                Y_XMLOG(xml, reached << best);
                            }
                            else
                            {
                                Y_XMLOG(xml, partial << best);
                            }
                            return; // special partial/full
                        case Positive: continue;
                    }
                }

                // full
                best = **(required->tail);
                Y_XMLOG(xml, reached << best);

#if 0
                {
                    Y_XML_COMMENT(xml,"only required " << required);

                    //! automatically full
                    best = **(required->tail);
                    Y_XMLOG(xml, reached << best);
                }
#endif
            }

            xreal_t Extents:: generate(XMLog            &xml,
                                       XAdd             &xadd,
                                       XWritable        &Csub,
                                       const Components &E,
                                       const XReadable  &C,
                                       const Level       L,
                                       const AddressBook &wanders) const
            {

                volatile Momentary::Off turnOff( Coerce(xml.verbose) );

                Y_XML_SECTION_OPT(xml, "Gaining", E.name);

                assert(best.size>0);
                if(xml.verbose) E.displayCompact( xml() << "bad = ", Csub, SubLevel) << std::endl;

                //______________________________________________________________
                //
                //
                // move according to best
                //
                //______________________________________________________________
                best.zforward(E, Csub, SubLevel);
                if(xml.verbose) E.displayCompact( xml() << "eqz = ", Csub, SubLevel) << std::endl;

                //______________________________________________________________
                //
                //
                // compute gain for selected species
                //
                //______________________________________________________________
                const xreal_t zero;
                xadd.free();
                {
                    size_t n = E->size();
                    for(Components::ConstIterator it=E->begin();n-- > 0;++it)
                    {
                        const Species &sp = (**it).sp; if(wanders.has(sp)) continue;
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
                Y_XMLOG(xml,"gain = " << gain.str());
                return gain;
            }

        }

    }
}

