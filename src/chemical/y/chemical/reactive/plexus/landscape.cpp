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


        void Landscape:: shape(const Components &components,
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
    }

}

