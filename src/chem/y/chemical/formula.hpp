
//! \file

#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1

#include "y/lingo/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Lingo::Syntax::XNode XNode;

        class Formula : public Proxy<const XNode>
        {
        public:

            explicit Formula(XNode * const node) noexcept;
            virtual ~Formula() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Formula);
            const AutoPtr<XNode> code;
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif

