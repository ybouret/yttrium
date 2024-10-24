
//! \file

#ifndef Y_Lingo_Optional_Included
#define Y_Lingo_Optional_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! Optional motif
        //
        //
        //______________________________________________________________________
        class Optional : public Joker
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[','?','?',']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Optional()       noexcept; //!< cleanup
            static Pattern * Create(Pattern *); //!< create

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Pattern * clone()                      const; //!< [Pattern] protected clone
            virtual bool      strong()            const noexcept; //!< [Pattern] false
            virtual bool      univocal()          const noexcept; //!< [Pattern] motif->univocal()
            virtual String    regularExpression()          const; //!< [Pattern] motif?
            virtual bool      takes(Token &, Source &)     const; //!< [Pattern] always true
            virtual size_t    serialize(OutputStream &)    const; //!< uuid + motif
            virtual void      viz(OutputStream &)          const; //!< [Pattern]

        private:
            explicit Optional(Pattern *) noexcept;;
            Y_DISABLE_COPY_AND_ASSIGN(Optional);
        };
    }

}

#endif

