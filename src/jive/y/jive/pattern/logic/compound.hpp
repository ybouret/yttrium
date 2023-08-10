//! \file

#ifndef Y_Jive_Pattern_Compound_Included
#define Y_Jive_Pattern_Compound_Included 1

#include "y/jive/pattern.hpp"
#include "y/data/list/cloneable.hpp"

namespace Yttrium
{
    namespace Jive
    {
        typedef ListOfCloneable<Pattern> Patterns;

        class Compound : public Pattern
        {
        public:
            virtual ~Compound() noexcept;

            virtual void   reset() noexcept;
            virtual size_t serialize(OutputStream &) const;
            void           retrieve(InputStream &);

            const Patterns patterns;

        protected:
            explicit Compound(const uint32_t  ) noexcept;
            explicit Compound(const Compound &);
            
        private:
            Y_DISABLE_ASSIGN(Compound);
        };

    }

}

#endif
