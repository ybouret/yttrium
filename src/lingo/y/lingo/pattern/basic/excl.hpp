
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
        class Excl  : public Pattern
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
            explicit Excl(const uint8_t) noexcept; //!< setup
            explicit Excl(const Excl &)  noexcept; //!< copy
            virtual ~Excl()              noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t    serialize(OutputStream &)           const; //!< [Serializable]
            virtual Pattern * clone()                             const; //!< [Pattern]
            virtual bool      takes(Token &token, Source &source) const; //!< [Pattern] true on different single byte
            virtual void      query( CharDB &firstChars )         const; //!< [Pattern] append all but byte
            virtual String    regularExpression()                 const; //!< [Patterm] [^byte]
            virtual bool      univocal()                 const noexcept; //!< [Pattern] false
            virtual bool      strong()                   const noexcept; //!< [Pattern] true
            virtual void      viz(OutputStream &)                 const; //!< [Pattern]

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint8_t byte; //!< single byte to exclude

        private:
            Y_DISABLE_ASSIGN(Excl);
        };
    }
}

#endif

