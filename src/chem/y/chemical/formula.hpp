
//! \file

#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1

#include "y/chemical/type/lang.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Formula : public Proxy<const XNode>
        {
        public:
            class Linker;
            
            explicit Formula(XNode * const node) noexcept;
            virtual ~Formula() noexcept;

            const Label          text;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Formula);
            const AutoPtr<XNode> code;
            virtual ConstInterface & surrogate() const noexcept;

            void walk(const XNode * const xnode,
                      unsigned            level);

            void add(const Lingo::Lexeme &);
            void add(const char);
        };

    }

}

#endif

