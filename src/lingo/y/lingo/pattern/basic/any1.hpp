
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
            static const uint32_t     UUID = Y_FOURCC('A','N','Y','1'); //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Any1()             noexcept; //!< setup
            explicit Any1(const Any1 &) noexcept; //!< copy
            virtual ~Any1()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()                 const noexcept; //!< [Identifiable] CallSign
            virtual size_t       serialize(OutputStream &)           const; //!< [Serializable] uuid
            virtual Pattern *    clone()                             const; //!< [Pattern] new Single(self)
            virtual bool         takes(Y_Lingo_Pattern_Args)         const; //!< [Pattern] true on single byte
            virtual void         query( CharDB &firstChars )         const; //!< [Pattern] fill
            virtual String       regularExpression()                 const; //!< [Pattern] full lump
            virtual bool         univocal()                 const noexcept; //!< false
            virtual bool         strong()                   const noexcept; //!< true
            virtual void         viz(OutputStream &)                 const; //!< [Pattern]

        private:
            Y_DISABLE_ASSIGN(Any1);
        };
    }
}

#endif

