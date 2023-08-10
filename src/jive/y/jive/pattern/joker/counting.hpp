//! \file

#ifndef Y_Jive_Pattern_Counting_Included
#define Y_Jive_Pattern_Counting_Included 1

#include "y/jive/pattern/joker/guest.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Counting pattern recognition
        //
        //
        //______________________________________________________________________
        class Counting : public Guest
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('C','N','T',':'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Counting(const size_t nmin, const size_t nmax, const Pattern &); //!< setup from clone
            explicit Counting(const size_t nmin, const size_t nmax, Pattern       *); //!< setup directly
            explicit Counting(const Counting&); //!< duplicate
            virtual ~Counting() noexcept;       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;                   //!< clone
            virtual bool     takes(Source &);                 //!< try guest, return true
            virtual bool     isFragile() const noexcept;      //!< if minCount <= 0 or motif is fragile
            virtual size_t   serialize(OutputStream&) const;  //!< uuid+guest

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const size_t minCount;
            const size_t maxCount;

        private:
            Y_DISABLE_ASSIGN(Counting);
        };


    }

}

#endif
