//! \file


#ifndef Y_Ink_Crux_Coefficients_Included
#define Y_Ink_Crux_Coefficients_Included 1

#include "y/data/small/heavy/list/bare.hpp"
#include "y/sort/merge.hpp"
#include "y/ink/coord.hpp"

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
            //! coefficient is a coordinate with a value
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Coefficient
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Small::BareHeavyList<Coefficient> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline Coefficient(const unit_t x, const unit_t y, const T w) noexcept :
                coord(x,y), value(w)
                {
                }

                //! copy
                inline Coefficient(const Coefficient &coef) noexcept :
                coord(coef.coord),
                value(coef.value)
                {
                }

                //! cleanup
                inline ~Coefficient() noexcept {}

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Coefficient &self)
                {
                    os << self.value << "@" << self.coord;
                    return os;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Coord coord; //!< relative cordinate
                const T     value; //!< value at this coordinate

            private:
                Y_DISABLE_ASSIGN(Coefficient);
            };

            

            //__________________________________________________________________
            //
            //
            //
            //! List of coefficients
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Coefficients : public Coefficient<T>::List
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Coefficient<T>              CoefType; //!< alias
                typedef typename CoefType::List     ListType; //!< alias
                typedef typename ListType::NodeType NodeType; //!< aluas

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Coefficients() noexcept : ListType() {} //!< setup
                inline virtual ~Coefficients() noexcept {}              //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! sort by increasing values
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

    }

}

#endif
