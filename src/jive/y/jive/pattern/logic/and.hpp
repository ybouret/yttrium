
//! \file

#ifndef Y_Jive_Pattern_And_Included
#define Y_Jive_Pattern_And_Included 1

#include "y/jive/pattern/logic/compound.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class And : public Compound
        {
        public:
            static const uint32_t UUID = Y_FOURCC('_','&','&','_');

            explicit And() noexcept;
            explicit And(const And &);
            virtual ~And() noexcept;
            
            virtual Pattern *clone() const;
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const;
            virtual bool     isFragile() const noexcept;  

        private:
            Y_DISABLE_ASSIGN(And);
        };
    }

}

#endif
