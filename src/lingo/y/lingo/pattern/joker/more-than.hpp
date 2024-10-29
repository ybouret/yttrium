

//! \file

#ifndef Y_Lingo_MoreThan_Included
#define Y_Lingo_MoreThan_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! MoreThan minCount STRONG motif
        //
        //
        //______________________________________________________________________
        class MoreThan : public Joker
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
            virtual ~MoreThan() noexcept;                          //!< cleanup
            static Pattern * Create(Pattern *, const size_t nmin); //!< with STRONG pattern

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign()          const noexcept; //!< [Identifiable] CallSign
            virtual Pattern *    clone()                      const; //!< [Pattern] protected clone
            virtual bool         strong()            const noexcept; //!< [Pattern] minimalCount>0
            virtual bool         univocal()          const noexcept; //!< [Pattern] false
            virtual String       regularExpression()          const; //!< [Pattern] (motif)(*|+|{n,})*
            virtual bool         takes(Y_Lingo_Pattern_Args)  const; //!< [Pattern] if found minimalCount
            virtual size_t       serialize(OutputStream &)    const; //!< [Serializable] uuid + nmin + motif
            virtual void         viz(OutputStream &)          const; //!< [Pattern]

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t minimalCount; //!< minimal valid count

        private:
            explicit MoreThan(Pattern *, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(MoreThan);
        };
    }

}

#endif

