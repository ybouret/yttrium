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
            static const uint32_t UUID = Y_FOURCC('C','N','T','_'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static   Pattern *Make(const size_t nmin, const size_t nmax, Pattern       *); //!< take care of pattern
            static   Pattern *Make(const size_t nmin, const size_t nmax, const Pattern &); //!< take care of clone
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
            virtual bool     isFragile() const noexcept;      //!< if minCount <= 0
            virtual size_t   serialize(OutputStream&) const;  //!< uuid+guest
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     viz(OutputStream &fp)      const;

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const size_t minCount; //!< min count
            const size_t maxCount; //!< max count

        private:
            Y_DISABLE_ASSIGN(Counting);
            explicit Counting(const size_t nmin, const size_t nmax, Pattern       *); //!< setup directly
        };


    }

}

#endif
