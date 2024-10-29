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
        class Byte : public Pattern
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
            explicit Byte(const uint8_t)  noexcept; //!< setup
            explicit Byte(const Byte &)   noexcept; //!< copy
            virtual ~Byte()               noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()                 const noexcept; //!< [Identifiable] CallSign
            virtual size_t       serialize(OutputStream &)           const; //!< [Serializable] uuid+byte
            virtual Pattern *    clone()                             const; //!< [Pattern] new Single(self)
            virtual bool         takes(Y_Lingo_Pattern_Args)         const; //!< [Pattern] true on single byte
            virtual void         query( CharDB &firstChars )         const; //!< [Pattern] append byte
            virtual String       regularExpression()                 const; //!< [Pattern] escaped char
            virtual bool         univocal()                 const noexcept; //!< true
            virtual bool         strong()                   const noexcept; //!< true
            virtual void         viz(OutputStream &)                 const; //!< [Pattern]

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint8_t byte; //!< single byte recognition

        private:
            Y_DISABLE_ASSIGN(Byte);
        };
    }
}

#endif

