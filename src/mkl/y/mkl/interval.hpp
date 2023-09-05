
//! \file

#ifndef Y_MKL_Interval_Included
#define Y_MKL_Interval_Included 1

#include "y/mkl/limit.hpp"
#include "y/config/shallow.hpp"

namespace Yttrium
{

    namespace MKL
    {
        Y_SHALLOW_DECL(Infinity);

        namespace Kernel
        {

            class Interval
            {
            public:
                static const char LPAREN = '[';
                static const char RPAREN = ']';
                static const char MINUS  = '-';
                static const char PLUS   = '+';
                static const char COLON  = ':';
                static const char INFTY[];

                explicit Interval() noexcept;
                virtual ~Interval() noexcept;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Interval);
            };
        }

        template <typename T>
        class Interval : public Kernel::Interval
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            inline explicit Interval() : lower(), upper() {}
            inline virtual ~Interval() noexcept {}
            inline Interval(const Interval &I) :
            Kernel::Interval(), lower(I.lower), upper(I.upper) {}

            inline Interval(ConstType lo, ConstType up) :
            lower(lo),
            upper(up)
            {
            }

            inline Interval(const Infinity_ &, ConstType up, const bool takenUp) :
            lower(),
            upper(up,takenUp)
            {
            }


            const Limit<T> lower;
            const Limit<T> upper;


            inline friend std::ostream & operator<<(std::ostream &os, const Interval &I)
            {
                switch(I.lower.type)
                {
                    case LimitNone:   os << RPAREN << MINUS << INFTY; break;
                    case LimitOpen:   os << RPAREN << I.lower.value;  break;
                    case LimitClosed: os << LPAREN << I.lower.value;  break;
                }
                os << COLON;
                switch(I.upper.type)
                {
                    case LimitNone:   os << PLUS << INFTY << LPAREN; break;
                    case LimitOpen:   os << I.upper.value << LPAREN; break;
                    case LimitClosed: os << I.upper.value << RPAREN; break;
                }

                return os;
            }

        private:
            Y_DISABLE_ASSIGN(Interval);

        };
    }

}

#endif
