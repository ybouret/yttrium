
#include "y/chemical/plexus/device.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        bool Device:: stepWasCut(XWritable &       target,
                                 const XReadable & source,
                                 XWritable &       deltaC,
                                 xreal_t * const   result) const
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            xreal_t scale = 1.0;
            bool    abate = false;
            assert( basisOkWith(source,SubLevel) );
            assert(target.size()==source.size());
            assert(target.size()==deltaC.size());

            //------------------------------------------------------------------
            //
            //
            // loop over components, act on negative deltaC
            //
            //
            //------------------------------------------------------------------
            const size_t m = target.size();
            for(size_t j=m;j>0;--j)
            {
                const xreal_t d = deltaC[j];  if(d.mantissa>=0) continue;
                const xreal_t c = source[j];  assert(c.mantissa>=0);
                const xreal_t f = c/(-d);
                if(f<=scale)
                {
                    abate = true;
                    scale = f;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // apply safety
            //
            //
            //------------------------------------------------------------------
            if( abate )
                scale *= shield;

            //------------------------------------------------------------------
            //
            //
            // generate
            //
            //
            //------------------------------------------------------------------
        GENERATE:
            for(size_t j=m;j>0;--j)
                target[j] = source[j] + scale * deltaC[j];

            if( !basisOkWith(target,SubLevel) )
            {
                scale *= shield;
                abate  = true;
                goto GENERATE;
            }

            if(result) *result = scale;

            //------------------------------------------------------------------
            //
            //
            // recompute effective step
            //
            //
            //------------------------------------------------------------------
            for(size_t j=m;j>0;--j)
            {
                deltaC[j] = target[j] - source[j];
            }


            return abate;
        }



        bool Device:: basisOkWith(const XReadable &C, const Level L) const noexcept
        {
            for(const ANode *an=basis.head;an;an=an->next)
            {
                if( ! (**an).eq.canTolerate(C,L) ) return false;
            }
            return true;
        }


    }

}


