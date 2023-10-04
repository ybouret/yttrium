//! \file

#ifndef Y_Jive_Syntax_Node_Included
#define Y_Jive_Syntax_Node_Included 1

#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            class XNode : public Object
            {
            public:
                typedef CxxListOf<XNode> List_;
                class List : public Object, public List_
                {
                public:
                    explicit List() noexcept;
                    List(const List &);
                    virtual ~List() noexcept;

                private:
                    Y_DISABLE_ASSIGN(List);
                };

                XNode *next;
                XNode *prev;
                XNode *parent;
            private:
                Y_DISABLE_ASSIGN(XNode);
            };

        }

    }

}

#endif


