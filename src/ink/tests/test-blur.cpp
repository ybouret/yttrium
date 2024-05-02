#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/sort/merge.hpp"
#include "y/mkl/api.hpp"
#include "y/type/proxy.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/channels.hpp"

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
            void apply(COLOR               &target,
                       const Pixmap<COLOR> &source,
                       const Coord          origin) const
            {
                typedef Color::Channels<COLOR> Ops;
                T channel[4] = {0,0,0,0};
                
                // convert target to floating-point channel(s)
                target = source(origin);
                Ops::Ldz(channel);

                // perform floating-point computation
                for(const WNode *node=weights.head;node;node=node->next)
                {
                    const Weight  w = **node;
                    const Coord   p = w.coord + origin;
                    Ops::Add(channel,w.value,source[p]);
                }

                // rescale to floating point in units of COLOR
                Ops::Div(channel,scale);


                // floating-point channel->target
                Ops::Get(target,channel);
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
        Pixmap<uint8_t> blr8(img.w,img.h);


        for(unit_t y=0;y<img.h;++y)
        {
            for(unit_t x=0;x<img.w;++x)
            {
                blur.apply(blr[y][x], img,  Coord(x,y)  );
                blur.apply(blr8[y][x],img8, Coord(x,y) );
            }
        }

        IMG.save(img,"blur-in.png",0);
        IMG.save(blr,"blur-out.png",0);

        IMG.Codec::save(img8, "blur8-in.png", 0, par, Color::GrayScale::ByteTo<RGBA>);
        IMG.Codec::save(blr8, "blur8-out.png", 0, par, Color::GrayScale::ByteTo<RGBA>);

    }





}
Y_UDONE()

