
//! \file

#ifndef Y_Lingo_Exclude_Included
#define Y_Lingo_Exclude_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Exclude one byte
        //
        //
        //______________________________________________________________________
        class Exclude : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('E','X','C','L'); //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Exclude(const uint8_t)   noexcept; //!< setup
            explicit Exclude(const Exclude &) noexcept; //!< copy
            virtual ~Exclude()                noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t    serialize(OutputStream &)           const; //!< [Serializable]
            virtual Pattern * clone()                             const; //!< [Pattern]
            virtual bool      takes(Token &token, Source &source) const; //!< [Pattern] true on single byte
            virtual void      query( CharDB &firstChars )         const; //!< [Pattern] append byte
            virtual String    regularExpression()                 const; //!< [Patterm] [^byte]
            virtual bool      univocal()                 const noexcept; //!< [Pattern] false
            virtual bool      strong()                   const noexcept; //!< [Pattern] false

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint8_t byte; //!< single byte to exclude

        private:
            Y_DISABLE_ASSIGN(Exclude);
        };
    }
}

#endif

