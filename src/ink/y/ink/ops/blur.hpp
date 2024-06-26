//! \file

#ifndef Y_Ink_Blur_Included
#define Y_Ink_Blur_Included 1

#include "y/ink/pixmap.hpp"
#include "y/ink/ops/blur/coefficients.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/color/channels.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            //__________________________________________________________________
            //
            //
            //
            //! base class for Blur
            //
            //
            //__________________________________________________________________
            class Blur : public Object, public Counted
            {
            public:
                static const char * const CallSign; //!< "Ink::Blur"
                virtual ~Blur() noexcept;           //!< cleanup
            protected:
                explicit Blur() noexcept;           //!< setup
                static void ThrowInvalidSigma(const double); //!< raise exception
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Blur);
            };
        }

        //__________________________________________________________________
        //
        //
        //
        //! Blur as a list of coefficients
        //
        //
        //__________________________________________________________________
        template <typename T>
        class Blur : public Crux::Blur, public Proxy< Crux::Coefficients<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Crux::Coefficient<T>       Weight;  //!< alias
            typedef Crux::Coefficients<T>      Weights; //!< alias
            typedef typename Weights::NodeType WNode;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup for exp(-r^2/(2*sig^2)) <= 1/256
            explicit Blur(const T sig) :
            Crux::Blur(),
            Proxy<Weights>(),
            weights(),
            scale(0),
            sigma(  MKL::Fabs<T>::Of(sig) ),
            sigma2( Sigma2(sig)           )
            {
                setup();
            }

            //! cleanup
            virtual ~Blur() noexcept {}


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! apply from source to target
            template <typename COLOR> inline
            void operator()(Slabs &slabs, Pixmap<COLOR> &target, const Pixmap<COLOR> &source) const
            {
                slabs( Apply<COLOR>, target, *this, source);
            }


        private:
            Weights     weights; //!< internal weights
        public:
            const T     scale;   //!< sum of weights
            const T     sigma;   //!< standard deviatiom
            const T     sigma2;  //!< variance

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur);
            inline virtual const Weights & surrogate() const noexcept { return weights; }

            template <typename COLOR> inline
            void apply(COLOR               &target,
                       const Pixmap<COLOR> &source,
                       const Coord          origin) const
            {
                typedef Color::Channels<COLOR> Ops;

                //--------------------------------------------------------------
                // store all components
                //--------------------------------------------------------------
                target = source(origin);

                //--------------------------------------------------------------
                // convert target to floating-point channel(s)
                //--------------------------------------------------------------
                T channel[4] = {0,0,0,0};
                Ops::Ldz(channel);

                //--------------------------------------------------------------
                // perform floating-point computation
                //--------------------------------------------------------------
                for(const WNode *node=weights.head;node;node=node->next)
                {
                    const Weight  w = **node;
                    const Coord   p = w.coord + origin;
                    Ops::Add(channel,w.value,source[p]);
                }

                //--------------------------------------------------------------
                // rescale to floating point in units of COLOR
                //--------------------------------------------------------------
                Ops::Div(channel,scale);

                //--------------------------------------------------------------
                // floating-point channel -> nearest target
                //--------------------------------------------------------------
                Ops::Get(target,channel);
            }
            template <typename COLOR> static inline
            void Apply(Slab &slab, Pixmap<COLOR> &target, const Blur &blur, const Pixmap<COLOR> &source)
            {
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment     seg = slab.hseg[k];
                    Coord                   pos(seg.x,seg.y);
                    PixRow<COLOR>          &tgt = target[pos.y];
                    for(unit_t i=seg.w;i>0;--i,++pos.x)
                    {
                        blur.apply(tgt[pos.x],source,pos);
                    }
                }
            }

            static inline T Sigma2(const T sig)
            {
                if(sig<=0) ThrowInvalidSigma(double(sig));
                return sig*sig;
            }

            inline void setup()
            {
                static const T one   = 1;
                static const T wmin  = one/256;
                const T        r2max = 12 * sigma2;
                const T        rrmax = MKL::Sqrt<T>::Of(r2max);
                const unit_t   rmax  = static_cast<unit_t>(MKL::Floor<T>::Of(rrmax));
                const T den = sigma2+sigma2;
                MKL::Antelope::Add<T> xadd;
                for(unit_t y=-rmax;y<=rmax;++y)
                {
                    const unit_t y2 = y*y;
                    for(unit_t x=-rmax;x<=rmax;++x)
                    {
                        const unit_t x2 = x*x;
                        const T      r2 = (y2+x2);
                        const T      arg = -r2/den;
                        const T      w   = exp(arg);
                        if(w>=wmin)
                        {
                            const Weight weight(x,y,w);
                            weights << weight;
                            xadd    << weight.value;
                        }
                    }
                }
                weights.sortByIncreasingValue();
                Coerce(scale) = xadd.sum();
            }



        };
    }

}

#endif

