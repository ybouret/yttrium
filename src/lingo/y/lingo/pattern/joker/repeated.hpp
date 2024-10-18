

//! \file

#ifndef Y_Lingo_Repeated_Included
#define Y_Lingo_Repeated_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Repeated STRONG motif
        //
        //
        //______________________________________________________________________
        class Repeated : public Joker
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[', '>', '=', ']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Repeated() noexcept;                          //!< cleanup
            static Pattern * Create(Pattern *, const size_t nmin); //!< with STRONG pattern

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Pattern * clone()                      const; //!< [Pattern] protected clone
            virtual bool      strong()            const noexcept; //!< [Pattern] minimalCount>0
            virtual bool      univocal()          const noexcept; //!< [Pattern] false
            virtual String    regularExpression()          const; //!< [Pattern] (motif)(*|+|[n,])*
            virtual bool      takes(Token &, Source &)     const; //!< [Pattern] if found minimalCount
            virtual size_t    serialize(OutputStream &)    const; //!< [Serializable] uuid + nmin + motif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t minimalCount; //!< minimal valid count

        private:
            explicit Repeated(Pattern *, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Repeated);
        };
    }

}

#endif

