#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/sort/merge.hpp"
#include "y/mkl/api.hpp"
#include "y/type/proxy.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/scalar-conv.hpp"

namespace Yttrium
{
    namespace Ink
    {
        namespace Crux
        {
            template <typename T>
            class Coefficient
            {
            public:
                typedef Small::BareHeavyList<Coefficient> List;

                inline Coefficient(const unit_t x, const unit_t y, const T w) noexcept :
                coord(x,y), value(w)
                {
                }

                inline Coefficient(const Coefficient &coef) noexcept :
                coord(coef.coord),
                value(coef.value)
                {
                }

                inline ~Coefficient() noexcept {}

                inline friend std::ostream & operator<<(std::ostream &os, const Coefficient &self)
                {
                    os << self.value << "@" << self.coord;
                    return os;
                }

                const Coord coord;
                const T     value;

            private:
                Y_DISABLE_ASSIGN(Coefficient);
            };



            template <typename T>
            struct Channels
            {
                enum { Value = 1 };

                template <typename U> static inline
                void Ldz(U * const channels) noexcept
                {
                    assert(0!=channels);
                    channels[0] = 0;
                }

                template <typename U> static inline
                void Add(U * const channels, const U &cof, const T &arg) noexcept
                {
                    assert(0!=channels);
                    channels[0] += cof * static_cast<U>(arg);
                }

                template <typename U> static inline
                void Div(U * const channels, const U &den) noexcept
                {
                    assert(0!=channels);
                    channels[0] /= den;
                }

            };

            template <typename TYPE>
            struct Channels< Color::RGBA<TYPE> >
            {
                typedef typename Color::RGBA<TYPE> COLOR;
                enum { Value = 3 };

                template <typename U> static inline
                void Ldz(U * const channels) noexcept
                {
                    assert(0!=channels);
                    channels[0] = 0;
                    channels[1] = 0;
                    channels[2] = 0;
                }

                template <typename U> static inline
                void Add(U * const channels, const U &cof, const COLOR &arg) noexcept
                {
                    assert(0!=channels);
                    const TYPE * const c = (const TYPE *) &arg;
                    channels[0] += cof * static_cast<U>(c[0]);
                    channels[1] += cof * static_cast<U>(c[1]);
                    channels[2] += cof * static_cast<U>(c[2]);
                }

                template <typename U> static inline
                void Div(U * const channels, const U &den) noexcept
                {
                    assert(0!=channels);
                    channels[0] /= den;
                    channels[1] /= den;
                    channels[2] /= den;
                }
            };

            template <typename TYPE>
            struct Channels< Color::RGB<TYPE> >
            {
                typedef typename Color::RGB<TYPE> COLOR;
                enum { Value = 3 };

                template <typename U> static inline
                void Ldz(U * const channels) noexcept
                {
                    assert(0!=channels);
                    channels[0] = 0;
                    channels[1] = 0;
                    channels[2] = 0;
                }

                template <typename U> static inline
                void Add(U * const channels, const U &cof, const COLOR &arg) noexcept
                {
                    assert(0!=channels);
                    const TYPE * const c = (const TYPE *) &arg;
                    channels[0] += cof * static_cast<U>(c[0]);
                    channels[1] += cof * static_cast<U>(c[1]);
                    channels[2] += cof * static_cast<U>(c[2]);
                }

                template <typename U> static inline
                void Div(U * const channels, const U &den) noexcept
                {
                    assert(0!=channels);
                    channels[0] /= den;
                    channels[1] /= den;
                    channels[2] /= den;
                }
            };






            template <typename T>
            class Coefficients : public Coefficient<T>::List
            {
            public:
                typedef Coefficient<T>              CoefType;
                typedef typename CoefType::List     ListType;
                typedef typename ListType::NodeType NodeType;

                inline explicit Coefficients() noexcept : ListType() {}
                inline virtual ~Coefficients() noexcept {}

                inline void sortByIncreasingValue() noexcept
                {
                    MergeSort::Call(*this,Compare);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Coefficients);
                static inline
                SignType Compare(const NodeType * const lhs, const NodeType * const rhs) noexcept
                {
                    return Sign::Of( (**lhs).value, (**rhs).value );
                }
            };
        }



        template <typename T>
        class Blur : public Proxy< Crux::Coefficients<T> >
        {
        public:
            typedef Crux::Coefficient<T>       Weight;
            typedef Crux::Coefficients<T>      Weights;
            typedef typename Weights::NodeType WNode;

            explicit Blur(const T sig) :
            Proxy<Weights>(),
            weights(),
            scale(0),
            sigma( MKL::Fabs<T>::Of(sig) ),
            sigma2( Sigma2(sig) )
            {
                const T      one   = 1;
                const T      wmin  = one/256;
                const T      r2max = 12 * sigma2;
                const T      rrmax = MKL::Sqrt<T>::Of(r2max);
                const unit_t rmax  = static_cast<unit_t>(MKL::Floor<T>::Of(rrmax));
                //std::cerr << "sqrt(" << r2max << ")=" << rrmax << " => " << rmax << std::endl;
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
                        //std::cerr << "r2=" << r2 << "=>" << w << " (" << (w>=1.0/256) << ")" << std::endl;
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
                std::cerr << weights << std::endl;
                std::cerr << "#w=" << weights.size << std::endl;
                std::cerr << "scale=" << scale << std::endl;
            }

            template <typename COLOR> inline
            void apply(T * const            channels,
                       const Pixmap<COLOR> &source,
                       const Coord          origin) const
            {
                typedef Crux::Channels<COLOR> CHANNELS;
                CHANNELS::Ldz(channels);
                for(const WNode *node=weights.head;node;node=node->next)
                {
                    const Weight  w = **node;
                    const Coord   p = w.coord + origin;
                    Crux::Channels<COLOR>::Add(channels,w.value,source[p.y][p.x]);
                }
                CHANNELS::Div(channels,scale);
            }



        private:
            Weights     weights;
        public:
            const T     scale;
            const T     sigma;
            const T     sigma2;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur);
            inline virtual const Weights & surrogate() const noexcept { return weights; }

            static inline T Sigma2(const T sig)
            {
                if(sig<=0) throw Exception("sigma<=0");
                return sig*sig;
            }
        };

    }
}

#include "y/text/ascii/convert.hpp"
#include "y/concurrent/loop/crew.hpp"

using namespace Yttrium;
using namespace Ink;

Y_UTEST(blur)
{
    float sig = 2.4;
    if(argc>1)
    {
        sig = ASCII::Convert::ToReal<float>(argv[1],"sigma");
    }
    Blur<float> blur(sig);

    if(argc>2)
    {
        Concurrent::Topology   topo;
        Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
        Slabs                  par( crew );
        
        Codecs &        IMG = Ink::Codecs::Std();
        Pixmap<RGBA>    img = IMG.load(argv[2],0);
        Pixmap<RGBA>    blr(img.w,img.h);
        Pixmap<uint8_t> img8(par,Color::GrayScale::From<RGBA>,img);
        float           ch[4] = { 0,0,0,0 };

        for(unit_t y=0;y<img.h;++y)
        {
            for(unit_t x=0;x<img.w;++x)
            {
                blur.apply(ch,img, Coord(x,y) );
            }
        }
    }





}
Y_UDONE()

