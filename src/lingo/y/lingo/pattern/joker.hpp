//! \file

#ifndef Y_Lingo_Joker_Included
#define Y_Lingo_Joker_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        class Joker : public Pattern
        {
        protected:
            //! pattern must be protected
            explicit Joker(const uint32_t t, Pattern *m) noexcept;

        public:
            virtual ~Joker() noexcept;

            virtual size_t serialize(OutputStream &) const; //!< uuid + motif
            virtual void   query(CharDB &)           const; //!< motif->query

        protected:
            Pattern * const motif;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joker);
        };

    }

}

#endif

