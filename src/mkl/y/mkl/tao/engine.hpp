//! \file

#ifndef Y_MKL_TAO_Carver_Included
#define Y_MKL_TAO_Carver_Included 1

#include "y/concurrent/loop/simd.hpp"
#include "y/mkl/tao/par/engine1d.hpp"
#include "y/mkl/tao/par/engine2d.hpp"
#include "y/container/matrix/metrics.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            //__________________________________________________________________
            //
            //
            //
            //! handle engines for containers
            //
            //
            //__________________________________________________________________
            class Engine
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Concurrent::SIMD<Engine1D> In1D;   //!< alias
                typedef Concurrent::SIMD<Engine2D> In2D;   //!< alias
                typedef In2D::Vertex               Vertex; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Engine(const Concurrent::SharedLoop &); //!< setup engines
                virtual ~Engine() noexcept;                      //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void setup(const size_t         n); //!< setup in1d(1,n,1)
                void setup(const MatrixMetrics &M); //!< setup in2d( (1,1) -> (cols,rows)

                //! link resource to each engine
                template <typename NODE> inline
                void link1D(NODE *node)
                {
                    for(size_t i=in1D.size();i>0;--i,node=node->next)
                    {
                        assert(0!=node);
                        in1D[i].anonymous = node;
                    }
                }

                //! reset resource from each engine
                void free1D() noexcept;

                //! link resource to each engine
                template <typename NODE> inline
                void link2D(NODE *node)
                {
                    for(size_t i=in2D.size();i>0;--i,node=node->next)
                    {
                        assert(0!=node);
                        in2D[i].anonymous = node;
                    }
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                In1D in1D; //!< 1D engines
                In2D in2D; //!< 2D engines

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
                const size_t one;
                const Vertex org;
            };
        }

    }
}


#endif

