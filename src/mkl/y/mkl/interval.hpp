
//! \file

#ifndef Y_MKL_Interval_Included
#define Y_MKL_Interval_Included 1

#include "y/mkl/limit.hpp"
#include "y/mkl/triplet.hpp"
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
            inline explicit Interval() : lower(Infinity), upper(Infinity) {} //!< default: full space
            inline virtual ~Interval() noexcept                           {} //!< cleanup

            //! copy
            inline Interval(const Interval &I) : Kernel::Interval(), lower(I.lower), upper(I.upper) {}

            //! setup from limits
            inline Interval(const Limit<T> lo, const Limit<T> up) :
            Kernel::Interval(),
            lower(lo),
            upper(up)
            {
                if(lower.value>upper.value) Memory::OutOfReach::Swap(Coerce(lower), Coerce(upper));
            }

            //! assign to safely change values
            Interval & operator=(const Interval &I) noexcept
            {
                Memory::OutOfReach::Copy( Coerce(lower), I.lower);
                Memory::OutOfReach::Copy( Coerce(upper), I.upper);
                return *this;
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
            //! check x is inside the interval
            //__________________________________________________________________
            inline bool contains(ConstType x) const
            {
                switch(lower.type)
                {
                    case UnboundedLimit:
                        break;

                    case ExcludingLimit:
                        if(x<=lower.value) return false;
                        break;

                    case IncludingLimit:
                        if(x<lower.value) return false;
                        break;
                }

                switch(upper.type)
                {
                    case UnboundedLimit:
                        break;

                    case ExcludingLimit:
                        if(x>=upper.value) return false;
                        break;

                    case IncludingLimit:
                        if(x>upper.value) return false;
                        break;
                }

                return true;
            }

            //__________________________________________________________________
            //
            //! shift lower value within interval
            /**
             \param x x.a <= x.b <= x.c, this->contains(x.b)
             \return positive value if x.a was shifted up
             */
            //__________________________________________________________________
            inline T shiftLower(Triplet<T> &x) const
            {
                assert( contains(x.b) );
                assert( x.isIncreasing() );
                T shift(0);

                switch(lower.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.a<lower.value)
                        {
                            shift = lower.value - x.a;
                            x.a   = lower.value;
                            assert(x.isIncreasing());
                        }
                        break;

                    case ExcludingLimit:
                        if(x.a<=lower.value)
                        {
                            static const T half(0.5);
                            const T        xOld = x.a;
                            do x.a = x.b + (x.a-x.b) * half; while(x.b<=lower.value);
                            shift = x.a - xOld;
                        }
                        break;
                }
                return shift;

            }

            //__________________________________________________________________
            //
            //! shift upper value withing interval
            /**
             \param x x.a <= x.b <= x.c, this->contains(x.b)
             \return positive value if x.c was shifted down
             */
            //__________________________________________________________________
            inline T shiftUpper(Triplet<T> &x) const
            {
                assert( contains(x.b) );
                assert( x.isIncreasing() );
                T shift(0);

                switch(upper.type)
                {
                    case UnboundedLimit:
                        break;

                    case IncludingLimit:
                        if(x.c>upper.value)
                        {
                            shift = x.c - upper.value;
                            x.c   = upper.value;
                            assert(x.isIncreasing());
                        }
                        break;

                    case ExcludingLimit:
                        if(x.c>=upper.value)
                        {
                            static const T half(0.5);
                            const T        xOld = x.c;
                            do x.c = x.b + (x.c-x.b) * half; while(x.c>=upper.value);
                            shift = xOld-x.c;
                        }
                        break;
                }
                return shift;
            }

            //__________________________________________________________________
            //
            //! Try to pack triplet within interval and conserve length
            //__________________________________________________________________
            inline bool tryPack( Triplet<T> &x ) const
            {
                bool movedLower = false;
                bool movedUpper = false;

            TRY:
                {
                    const T shift = shiftLower(x);
                    if(shift>0)
                    {
                        movedLower = true;
                        if(!movedUpper) x.c += shift;
                    }
                }

                {
                    const T shift = shiftUpper(x);
                    if(shift>0)
                    {
                        movedUpper = true;
                        if(!movedLower)
                        {
                            x.a -= shift;
                            goto TRY;
                        }
                    }
                }

                return !movedLower && !movedUpper;
            }



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Limit<T> lower; //!< lower limit
            const Limit<T> upper; //!< upper limit




        };
    }

}

#endif
