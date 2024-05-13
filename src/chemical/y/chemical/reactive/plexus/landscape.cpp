#include "y/chemical/reactive/plexus/landscape.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Landscape:: ~Landscape() noexcept {}
        
        Landscape:: Landscape(const BBank &bbank,
                              const SBank &sbank) noexcept :
        capping(bbank,sbank),
        missing(bbank,sbank)
        {
        }

        Landscape:: Landscape(const Landscape &other) : capping(other.capping), missing(other.missing)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Landscape &self)
        {
            os << "capping=" << self.capping << " / missing=" << self.missing;
            return os;
        }

        void Landscape:: reset() noexcept { capping.reset(); missing.reset(); }


        void  Landscape:: shape(const Components &components,
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


        Equilibrium::Status Landscape:: study()
        {
            static const unsigned MISSING_NONE = 0x00;
            static const unsigned MISSING_REAC = 0x01;
            static const unsigned MISSING_PROD = 0x02;
            static const unsigned MISSING_BOTH = MISSING_REAC|MISSING_PROD;;

            unsigned                 flag  = MISSING_NONE;
            if(missing.reac->size>0) flag |= MISSING_REAC;
            if(missing.prod->size>0) flag |= MISSING_PROD;

            switch(flag)
            {
                case MISSING_NONE: return Equilibrium::Running;
                    
                case MISSING_REAC:
                    assert(0==missing.prod->size);
                    assert(missing.reac->size>0);
                    break;

                case MISSING_PROD:
                    assert(0==missing.reac->size);
                    assert(missing.prod->size>0);
                    break;

                case MISSING_BOTH:
                    break;
                default:
                    break;
            }
            assert(MISSING_BOTH==flag);
            return Equilibrium::Blocked;
        }
    }

}

