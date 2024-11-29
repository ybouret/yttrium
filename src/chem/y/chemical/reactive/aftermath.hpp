
//! \file

#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Outcome
        {
        public:
            Outcome(const Situation  &_st,
                    const Components &_eq,
                    const xReal       _eK,
                    const XReadable &_C,
                    const Level      _L);
            Outcome(const Outcome &) noexcept;
            ~Outcome() noexcept;


            const Situation   st;
            const Components &eq;
            const xReal       eK;
            const XReadable  &C;
            const Level       L;

        private:
            Y_DISABLE_ASSIGN(Outcome);
        };

        Outcome:: Outcome(const Situation  &_st,
                          const Components &_eq,
                          const xReal       _eK,
                          const XReadable &_C,
                          const Level      _L) :
        st(_st),
        eq(_eq),
        eK(_eK),
        C(_C),
        L(_L)
        {

        }

        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        C(_.C),
        L(_.L)
        {

        }


        Outcome:: ~Outcome() noexcept {}

        class Aftermath
        {
        public:

            explicit Aftermath();
            virtual ~Aftermath() noexcept;

            Outcome solve(const Components &eq,
                          const xReal       eK,
                          XWritable        &C1,
                          const Level       L1,
                          const XReadable  &C0,
                          const Level       L0);

            XMul xmul;
            XAdd xadd;
            const xReal zero;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
        };

    }

}

#endif

