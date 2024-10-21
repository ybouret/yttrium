
//! \file

#ifndef Y_Lingo_Any1_Included
#define Y_Lingo_Any1_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Any One Byte
        //
        //
        //______________________________________________________________________
        class Any1 : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('A','N','Y','1'); //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Any1()             noexcept; //!< setup
            explicit Any1(const Any1 &) noexcept; //!< copy
            virtual ~Any1()           noexcept; //!< cleanup

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
            virtual String    regularExpression()                 const; //!< [Pattern] full range
            virtual bool      univocal()                 const noexcept; //!< false
            virtual bool      strong()                   const noexcept; //!< true


        private:
            Y_DISABLE_ASSIGN(Any1);
        };
    }
}

#endif

