#include "y/chemical/reactive/plexus/fence.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fence:: ~Fence() noexcept {}

        Fence:: Fence(const BBank &bbank,
                      const SBank &sbank) noexcept :
        capping(bbank,sbank),
        missing(bbank,sbank),
        cursor(0),
        zeroed(sbank)
        {
        }

        Fence:: Fence(const Fence &other) : 
        capping(other.capping),
        missing(other.missing),
        cursor(other.cursor),
        zeroed(other.zeroed)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Fence &self)
        {
            os << "capping=" << self.capping << " / missing=" << self.missing;
            return os;
        }

        void Fence:: reset() noexcept { 
            Coerce(capping).reset();
            Coerce(missing).reset();
            Coerce(cursor) = 0;
            Coerce(zeroed).free();
        }


        void  Fence:: shape(const Components &components,
                            const XReadable  &C,
                            const Level       level)
        {
            //--------------------------------------------------------------
            //
            // initialize
            //
            //--------------------------------------------------------------
            const xreal_t zero = 0;
            reset();

            //--------------------------------------------------------------
            //
            // scan reactants
            //
            //--------------------------------------------------------------
            for(const Actor *a=components.reac.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[level]];
                if(cc>=zero)
                {
                    Coerce(capping).reac(sp,cc/nu);
                }
                else
                {
                    Coerce(missing).reac(sp,(-cc)/nu);
                }
            }

            //--------------------------------------------------------------
            //
            // scan products
            //
            //--------------------------------------------------------------
            for(const Actor *a=components.prod.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[level]];
                if(cc>=zero)
                {
                    Coerce(capping).prod(sp,cc/nu);
                }
                else
                {
                    Coerce(missing).prod(sp,(-cc)/nu);
                }
            }


        }


        void Fence:: initWith(const Boundary * const bad, const bool reverse)
        {
            assert(0!=bad);
            const Boundary &bnd = *bad;
            Coerce(cursor) = reverse ? -(bnd.xi) : bnd.xi;
            Coerce(zeroed) << *bnd;
            assert(zeroed.size==bnd->size);
        }

        unsigned Fence:: study(XMLog &xml)
        {
            assert(0==zeroed.size);
            assert(MKL::Fabs<xreal_t>::Of(cursor) <= xreal_t(0));

            //------------------------------------------------------------------
            //
            //
            // local definitions
            //
            //
            //------------------------------------------------------------------
            static const unsigned MISSING_NONE = 0x00;
            static const unsigned MISSING_REAC = 0x01;
            static const unsigned MISSING_PROD = 0x02;
            static const unsigned MISSING_BOTH = MISSING_REAC|MISSING_PROD;;

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            unsigned                 flag  = MISSING_NONE;
            if(missing.reac->size>0) flag |= MISSING_REAC;
            if(missing.prod->size>0) flag |= MISSING_PROD;

            //------------------------------------------------------------------
            //
            //
            // process cases
            //
            //
            //------------------------------------------------------------------
            switch(flag)
            {
                    //----------------------------------------------------------
                    //
                    //
                case MISSING_NONE: Y_XMLOG(xml, " (-) missing none");
                    //
                    //
                    //----------------------------------------------------------
                    return RUNNING;

                    //----------------------------------------------------------
                    //
                    //
                case MISSING_REAC: {  assert(0==missing.prod->size); assert(missing.reac->size>0);
                    //
                    //
                    //----------------------------------------------------------
                    const Boundary * const bad = & **(missing.reac->head); Y_XMLOG(xml, " (+) missing  reac=" << *bad);
                    const Boundary * const dom = capping.prod.dominant();
                    if(0==dom)
                    {
                        Y_XMLOG(xml, " (-) no dominant prod");
                        initWith(bad,false);
                        return EQUATED | BY_REAC;
                    }
                    else
                    {
                        Y_XMLOG(xml, " (+) dominant prod=" << *dom );
                    }
                    exit(0);
                } return -1;

                    //----------------------------------------------------------
                    //
                    //
                case MISSING_PROD: {
                    assert(0==missing.reac->size); assert(missing.prod->size>0);
                    //
                    //
                    //----------------------------------------------------------
                    const Boundary * const bad = & **(missing.prod->head); Y_XMLOG(xml, " (+) missing  prod=" << *bad);
                    const Boundary * const dom = capping.reac.dominant();
                    if(0==dom)
                    {
                        Y_XMLOG(xml, " (-) no dominant reac");
                        initWith(bad,true);
                        return EQUATED | BY_PROD;
                    }
                    else
                    {
                        Y_XMLOG(xml, " (+) dominant reac=" << *dom );
                    }
                    exit(0);
                } return -1;

                    //----------------------------------------------------------
                    //
                    //
                case MISSING_BOTH:
                default:           Y_XMLOG(xml, " (+) missing both");
                    //
                    //----------------------------------------------------------
                    break;
            }
            assert(MISSING_BOTH==flag);
            return BLOCKED | BY_BOTH;
        }
    }

}

