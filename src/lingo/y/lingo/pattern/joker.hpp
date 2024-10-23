//! \file

#ifndef Y_Lingo_Joker_Included
#define Y_Lingo_Joker_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! a Joker is a Pattern with a motif (a.k.a smart pointer)
        //
        //
        //______________________________________________________________________
        class Joker : public Pattern
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

        protected:
            //! pattern must be protected !
            explicit Joker(const uint32_t t, Pattern *m) noexcept;

        public:
            virtual ~Joker() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void   query(CharDB &) const; //!< [Pattern] motif->query

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Pattern & operator*() const noexcept; //!< accept internal motif
            void  optimize();                           //!< propagate optimize to motif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        protected:
            Pattern * const motif; //!< created pattern for this joker

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joker);
        };

    }

}

#endif

