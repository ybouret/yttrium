
//! \file

#ifndef Y_Lingo_Range_Included
#define Y_Lingo_Range_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Range of bytes
        //
        //
        //______________________________________________________________________
        class Range : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('R','N','G','E'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Range(uint8_t, uint8_t) noexcept; //!< setup
            explicit Range(const Range &)    noexcept; //!< copy
            virtual ~Range()                 noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t    serialize(OutputStream &)           const; //!< [Serializable] uuid+lower+upper
            virtual Pattern * clone()                             const; //!< [Pattern] new Range(self)
            virtual bool      takes(Token &token, Source &source) const; //!< [Pattern] true on single byte
            virtual void      query(CharDB &firstChars )          const; //!< [Pattern] append byte
            virtual String    regularExpression()                 const; //!< [Pattern] [lower-upper]
            virtual bool      univocal()                 const noexcept; //!< [Pattern] true if lower>=upper
            virtual bool      strong()                   const noexcept; //!< [Pattern] false
            virtual void      viz(OutputStream &)                 const; //!< [Pattern]

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint8_t lower; //!< lower value
            const uint8_t upper; //!< upper value

        private:
            Y_DISABLE_ASSIGN(Range);
        };
    }
}

#endif

