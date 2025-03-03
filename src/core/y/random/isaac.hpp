//! \file

#ifndef Y_Random_ISAAC_Included
#define Y_Random_ISAAC_Included 1

#include "y/random/bits.hpp"
#include "y/system/seed.hpp"

#include <cstring>
#include <cstdio>

namespace Yttrium
{
    namespace Random
    {

        //______________________________________________________________________
        //
        //
        //
        //! ISAAC Implementation
        //
        //
        //______________________________________________________________________
        template <const unsigned RANDSIZL>
        class ISAAC : public Bits
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef uint8_t     ub1;                       //!< alias
            typedef uint32_t    ub4;                       //!< alias
            static const size_t RANDSIZ = (1<<RANDSIZL);   //!< alias
            static const char   CID = '0' + RANDSIZL;      //!< '4' or '8'

            //! context of random number generator
            struct randctx
            {
                ub4 randcnt;          //!< current count
                ub4 randrsl[RANDSIZ]; //!< state
                ub4 randmem[RANDSIZ]; //!< aux. state
                ub4 randa;            //!< a
                ub4 randb;            //!< b
                ub4 randc;            //!< c
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with seed
            inline explicit ISAAC(const uint32_t s) noexcept :
            Bits(0xffffffff),
            rctx()
            {
                clear();
                for(size_t i=0;i<RANDSIZ;++i) rctx.randrsl[i] = s;
                randinit(&rctx,true);
            }

            //! setup from system seeds
            inline explicit ISAAC() :
            Bits(0xffffffff),
            rctx()
            {
                clear();
                for(size_t i=0;i<RANDSIZ;++i) rctx.randrsl[i] = SystemSeed::Get();
                randinit(&rctx,true);
            }

            //! cleanup
            inline virtual ~ISAAC() noexcept
            {
                clear();
            }

            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            inline virtual const char * callSign() const noexcept
            {
                static const char id[] = { 'I', 'S', 'A', 'A', 'C', '<', CID, '>', 0 };
                return id;
            }


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! test for ISAAC<8>
            void outputTest()
            {
                clear();
                randinit(&rctx,true);
                for(unsigned i=0; i<2; ++i)
                {
                    isaac(&rctx);
                    for (unsigned j=0; j<RANDSIZ; ++j)
                    {
                        printf("%.8lx", (unsigned long)(rctx.randrsl[j]));
                        if ((j&7)==7) printf("\n");
                    }
                }
            }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define Y_ISAAC_rand(r) \
(!(r)->randcnt-- ?      \
(isaac(r), (r)->randcnt=RANDSIZ-1, (r)->randrsl[(r)->randcnt]) : \
(r)->randrsl[(r)->randcnt])
#endif

            //! interface for bits
            inline virtual uint32_t next32() noexcept
            {
                randctx * const ctx = &rctx;
                return Y_ISAAC_rand(ctx);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ISAAC);
            randctx rctx;
            inline void clear() noexcept { memset(&rctx,0,sizeof(rctx)); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define Y_ISAAC_mix(a,b,c,d,e,f,g,h) \
{ \
a^=b<<11; d+=a; b+=c; \
b^=c>>2;  e+=b; c+=d; \
c^=d<<8;  f+=c; d+=e; \
d^=e>>16; g+=d; e+=f; \
e^=f<<10; h+=e; f+=g; \
f^=g>>4;  a+=f; g+=h; \
g^=h<<8;  b+=g; h+=a; \
h^=a>>9;  c+=h; a+=b; \
}
#endif
            /*
             ------------------------------------------------------------------------------
             If (flag==TRUE), then use the contents of randrsl[0..RANDSIZ-1] as the seed.
             ------------------------------------------------------------------------------
             */
            static void randinit(randctx * const ctx, const bool flag) noexcept
            {
                unsigned i;
                ub4 a,b,c,d,e,f,g,h;
                ub4 *m,*r;
                ctx->randa = ctx->randb = ctx->randc = 0;
                m=ctx->randmem;
                r=ctx->randrsl;
                a=b=c=d=e=f=g=h=0x9e3779b9;  /* the golden ratio */

                for (i=0; i<4; ++i)          /* scramble it */
                {
                    Y_ISAAC_mix(a,b,c,d,e,f,g,h);
                }

                if(flag)
                {
                    /* initialize using the contents of r[] as the seed */
                    for(i=0; i<RANDSIZ; i+=8)
                    {
                        a+=r[i  ]; b+=r[i+1]; c+=r[i+2]; d+=r[i+3];
                        e+=r[i+4]; f+=r[i+5]; g+=r[i+6]; h+=r[i+7];
                        Y_ISAAC_mix(a,b,c,d,e,f,g,h);
                        m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
                        m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
                    }
                    /* do a second pass to make all of the seed affect all of m */
                    for (i=0; i<RANDSIZ; i+=8)
                    {
                        a+=m[i  ]; b+=m[i+1]; c+=m[i+2]; d+=m[i+3];
                        e+=m[i+4]; f+=m[i+5]; g+=m[i+6]; h+=m[i+7];
                        Y_ISAAC_mix(a,b,c,d,e,f,g,h);
                        m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
                        m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
                    }
                }
                else
                {
                    /* fill in m[] with messy stuff */
                    for (i=0; i<RANDSIZ; i+=8)
                    {
                        Y_ISAAC_mix(a,b,c,d,e,f,g,h);
                        m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
                        m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
                    }
                }

                isaac(ctx);            /* fill in the first set of results */
                ctx->randcnt=RANDSIZ;  /* prepare to use the first set of results */
            }

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#undef  Y_ISAAC_mix
#define Y_ISAAC_ind(mm,x)  (*(ub4 *)((ub1 *)(mm) + ((x) & ((RANDSIZ-1)<<2))))
#define Y_ISAAC_rngstep(mix,a,b,mm,m,m2,r,x) \
{ \
x = *m;  \
a = (a^(mix)) + *(m2++); \
*(m++) = y = Y_ISAAC_ind(mm,x) + a + b; \
*(r++) = b = Y_ISAAC_ind(mm,y>>RANDSIZL) + x; \
}

#endif

            static inline void isaac(randctx * const ctx)
            {
                ub4 a,b,x,y,*m,*mm,*m2,*r,*mend;
                mm=ctx->randmem; r=ctx->randrsl;
                a = ctx->randa; b = ctx->randb + (++ctx->randc);
                for (m = mm, mend = m2 = m+(RANDSIZ/2); m<mend; )
                {
                    Y_ISAAC_rngstep( a<<13, a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a>>6 , a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a<<2 , a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a>>16, a, b, mm, m, m2, r, x);
                }
                for (m2 = mm; m2<mend; )
                {
                    Y_ISAAC_rngstep( a<<13, a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a>>6 , a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a<<2 , a, b, mm, m, m2, r, x);
                    Y_ISAAC_rngstep( a>>16, a, b, mm, m, m2, r, x);
                }
                ctx->randb = b; ctx->randa = a;
            }

#undef Y_ISAAC_ind
#undef Y_ISAAC_rngstep

        };

        


    }
}
#endif

