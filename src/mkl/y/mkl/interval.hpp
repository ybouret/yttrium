
//! \file

#ifndef Y_MKL_Interval_Included
#define Y_MKL_Interval_Included 1

#include "y/mkl/limit.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace MKL
    {
        
        namespace Kernel
        {
            //__________________________________________________________________
            //
            //
            //! Common code for Intervals
            //
            //__________________________________________________________________
            class Interval
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char LPAREN = '['; //!< alias
                static const char RPAREN = ']'; //!< alias
                static const char MINUS  = '-'; //!< alias
                static const char PLUS   = '+'; //!< alias
                static const char COLON  = ':'; //!< alias
                static const char INFTY[];      //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Interval() noexcept; //!< setup
                virtual ~Interval() noexcept; //!< cleanup



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Interval);
            };
        }


        //______________________________________________________________________
        //
        //
        //
        //! Interval defined by two limits
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Interval : public Kernel::Interval
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Interval() : lower(), upper() {} //!< default: full space
            inline virtual ~Interval() noexcept           {} //!< cleanup

            //! copy
            inline Interval(const Interval &I) : Kernel::Interval(), lower(I.lower), upper(I.upper) {}

            //! setup from limit
            inline Interval(const Limit<T> lo, const Limit<T> up) :
            Kernel::Interval(),
            lower(lo),
            upper(up)
            {
                if(lower.value>upper.value) Memory::OutOfReach::Swap(Coerce(lower), Coerce(upper));
            }


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Interval &I)
            {
                switch(I.lower.type)
                {
                    case UnboundedLimit: os << RPAREN << MINUS << INFTY; break;
                    case ExcludingLimit: os << RPAREN << I.lower.value;  break;
                    case IncludingLimit: os << LPAREN << I.lower.value;  break;
                }
                os << COLON;
                switch(I.upper.type)
                {
                    case UnboundedLimit: os << PLUS << INFTY << LPAREN; break;
                    case ExcludingLimit: os << I.upper.value << LPAREN; break;
                    case IncludingLimit: os << I.upper.value << RPAREN; break;
                }

                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Limit<T> lower; //!< lower limit
            const Limit<T> upper; //!< upper limit



        private:
            Y_DISABLE_ASSIGN(Interval);

        };
    }

}

#endif
