//! \file

#ifndef Y_Lingo_Single_Included
#define Y_Lingo_Single_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Single byte
        //
        //
        //______________________________________________________________________
        class Single : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('B','Y','T','E'); //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Single(const uint8_t)  noexcept; //!< setup
            explicit Single(const Single &) noexcept; //!< copy
            virtual ~Single()               noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t    serialize(OutputStream &)           const; //!< [Serializable] uuid+byte
            virtual Pattern * clone()                             const; //!< [Pattern] new Single(self)
            virtual bool      takes(Token &token, Source &source) const; //!< [Pattern] true on single byte
            virtual void      query( CharDB &firstChars )         const; //!< [Pattern] append byte
            virtual String    regularExpression()                 const; //!< [Pattern] escaped char
            virtual bool      univocal()                 const noexcept; //!< true
            virtual bool      strong()                   const noexcept; //!< true
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint8_t byte; //!< single byte recognition

        private:
            Y_DISABLE_ASSIGN(Single);
        };
    }
}

#endif

