
//! \file

#ifndef Y_Chemical_Landscape_Included
#define Y_Chemical_Landscape_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Landscape
        {
        public:
            Landscape(const BBank &, const SBank &) noexcept ;
            Landscape(const Landscape &);
            ~Landscape() noexcept;
            Y_OSTREAM_PROTO(Landscape);


            void reset() noexcept { capping.reset(); missing.reset(); }

            void shape(const Components &components,
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


            Limits capping;
            Limits missing;

        private:
            Y_DISABLE_ASSIGN(Landscape);
        };

    }

}

#endif

