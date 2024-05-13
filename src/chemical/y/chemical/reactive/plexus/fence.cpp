#include "y/chemical/reactive/plexus/fence.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fence:: ~Fence() noexcept {}

        Fence:: Fence(const BBank &bbank,
                      const SBank &sbank) noexcept :
        capping(bbank,sbank),
        missing(bbank,sbank)
        {
        }

        Fence:: Fence(const Fence &other) : capping(other.capping), missing(other.missing)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Fence &self)
        {
            os << "capping=" << self.capping << " / missing=" << self.missing;
            return os;
        }

        void Fence:: reset() noexcept { capping.reset(); missing.reset(); }


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
                    capping.reac(sp,cc/nu);
                }
                else
                {
                    missing.reac(sp,(-cc)/nu);
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
                    capping.prod(sp,cc/nu);
                }
                else
                {
                    missing.prod(sp,(-cc)/nu);
                }
            }


        }


        unsigned Fence:: study(const Boundary * &how, XMLog &xml)
        {
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
            how = 0;
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
                case MISSING_NONE: Y_XMLOG(xml, "MISSING_NONE");
                    //
                    //----------------------------------------------------------
                    return 0; // RUNNING

                    //----------------------------------------------------------
                    //
                case MISSING_REAC: Y_XMLOG(xml, "MISSING_REAC");
                    //
                    //----------------------------------------------------------

                    assert(0==missing.prod->size);
                    assert(missing.reac->size>0);
                    std::cerr << "MISSING_REAC todo" << std::endl;
                    exit(0);
                    break;

                    //----------------------------------------------------------
                    //
                case MISSING_PROD: Y_XMLOG(xml, "MISSING_PROD");
                    //
                    //----------------------------------------------------------
                {

                    assert(0==missing.reac->size);
                    assert(missing.prod->size>0);
                    const Boundary * const bad = & **(missing.prod->head);
                    const Boundary * const dom = capping.reac.dominant();
                    if(0==dom)
                    {
                        how = bad;
                    }
                    else
                    {

                    }
                    std::cerr << "MISSING_PROD todo" << std::endl;
                    exit(0);
                }   break;

                    //----------------------------------------------------------
                    //
                case MISSING_BOTH:
                default:           Y_XMLOG(xml, "MISSING_BOTH");
                    //
                    //----------------------------------------------------------
                    break;
            }
            assert(MISSING_BOTH==flag);
            return 1;
        }
    }

}

