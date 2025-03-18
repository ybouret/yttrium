
//! \file


#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1


#include "y/chemical/reactive/components.hpp"
#include "y/mkl/root/zbis.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Outcome
        {
        public:
             Outcome(const Situation   _st,
                     const XReadable & _cc,
                     const Level     & _lv,
                     const xreal_t     _xi) noexcept;
            Outcome(const Outcome &_)      noexcept;
            ~Outcome()                     noexcept;

            const Situation  st;
            const XReadable &cc;
            const Level      lv;
            const xreal_t    xi;
        private:
            Y_DISABLE_ASSIGN(Outcome);
        };



        class Aftermath
        {
        public:
            typedef MKL::ZBis<xreal_t> RooType;

            explicit Aftermath() noexcept;
            virtual ~Aftermath() noexcept;

            XMul               xmul;
            XAdd               xadd;
            RooType            root;
            const xreal_t      zero;

            /**
             \param E   components
             \parem K   constant
             \param C   working  concentration (=C0)
             \param L   working  level
             \param C0  starting point
             \param K0  starting level
             */
            Outcome operator()(const Components &E,
                               const xreal_t     K,
                               XWritable        &C,
                               const Level       L,
                               const XReadable  &C0,
                               const Level       L0);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
            xreal_t improve(const Components &E,
                            const xreal_t     K,
                            XWritable        &C,
                            const Level       L);
        };

    }

}

#endif

