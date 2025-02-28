

//! \file

#ifndef Y_Lingo_Void_Included
#define Y_Lingo_Void_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! void...
        //
        //
        //______________________________________________________________________
        class Void : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('V','O','I','D'); //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Void()             noexcept; //!< setup
            explicit Void(const Void &) noexcept; //!< copy
            virtual ~Void()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()                 const noexcept; //!< [Identifiable] CallSign
            virtual size_t       serialize(OutputStream &)           const; //!< [Serializable] uuid
            virtual Pattern *    clone()                             const; //!< [Pattern] new Void(self)
            virtual bool         takes(Y_Lingo_Pattern_Args)         const; //!< [Pattern] false BUT end of source
            virtual void         query( CharDB &firstChars )         const; //!< [Pattern] free
            virtual String       regularExpression()                 const; //!< [Pattern] ^full range
            virtual bool         univocal()                 const noexcept; //!< true
            virtual bool         strong()                   const noexcept; //!< false
            virtual void         viz(OutputStream &)                 const; //!< [Pattern]


        private:
            Y_DISABLE_ASSIGN(Void);
        };
    }
}

#endif

