//! \file

#ifndef Y_MKL_Fit_Predictor_Included
#define Y_MKL_Fit_Predictor_Included 1

#include "y/sequence/snake.hpp"
#include "y/mkl/antelope/sum3.hpp"
#include "y/type/proxy.hpp"
#include "y/stream/output.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Fit
        {

            template <typename T>
            class Predictor : public Object, public Proxy< const Snake<T> >
            {
            public:
                typedef Snake<T>               SnakeType;
                typedef Proxy<const SnakeType> ProxyType;

                inline explicit Predictor() : Object(), ProxyType(), snake(3) {}
                inline virtual ~Predictor() noexcept {}

                inline void assign(const T value) noexcept
                {
                    snake.initWith(value);
                }

                inline void refine(const T value) noexcept
                {
                    snake.pushHead(value);
                }

                inline T compute(void) const noexcept
                {
                    static const T _0(0);


                    typename SnakeType::ConstIterator it = snake.begin();
                    switch(snake.size())
                    {
                        case 1: return *it;
                        case 2: {
                            static const T _three(3);
                            static const T _four(4);
                            const T f1 = *(it++);
                            const T f2 = *it;
                            return (_four * f1 - f2)/_three;
                        }

                        case 3: {
                            static const T _x1(18);
                            static const T _x2(-9);
                            static const T _x3(2);
                            static const T den(11);

                            const T f1 = *(it++);
                            const T f2 = *(it++);
                            const T f3 = *it;
                            return Antelope::Sum3<T>::Of(_x1*f1, _x2*f2, _x3*f3)/den;
                        }
                        default:
                            break;
                    }
                    return _0;
                }

                inline void save(OutputStream &fp) const
                {
                    unsigned i=1;
                    for(typename SnakeType::ConstIterator it=snake.begin();it!=snake.end();++it,++i)
                    {
                        fp("%u %.15g\n",i,double(*it));
                    }
                }

            private:
                SnakeType snake;
                Y_DISABLE_COPY_AND_ASSIGN(Predictor);
                typename ProxyType::ConstInterface & surrogate() const noexcept { return snake; }


            };
        }
    }
}

#endif

