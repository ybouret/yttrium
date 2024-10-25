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
            void  optimizing();                         //!< propagate optimize to motif
            void  ignoreCase();                         //!< propagate ignore case to motif


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        protected:
            Pattern * const motif; //!< created pattern for this joker

            //! raise error if guest is not strong
            static void CheckStrong(const char    * const host,
                                    const Pattern * const guest);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joker);
        };

    }

}

#endif

