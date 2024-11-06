//! \file

#ifndef Y_Lingo_Syntax_Rule_Included
#define Y_Lingo_Syntax_Rule_Included 1

#include "y/lingo/syntax/xlist.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            class Rule : public Entity, public GraphViz::Vizible
            {
            protected:
                template <typename NAME> inline
                explicit Rule(const NAME &   _name,
                              const uint32_t _uuid):
                Entity(_name,AsCaption),
                GraphViz::Vizible(),
                uuid(_uuid),
                self(0), next(0), prev(0)
                {
                }

            public:
                virtual ~Rule() noexcept;

                // Interface
                virtual void viz(OutputStream &fp) const = 0;

                // Methods
                void        graphViz(OutputStream &fp) const;
                bool        isInternal() const noexcept;
                bool        isTerminal() const noexcept;
                XNode::Type typeOfNode() const noexcept;

                // Members
            public:
                const uint32_t uuid;

            protected:
                void * const   self;
            public:
                Rule *         next;
                Rule *         prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

            typedef CxxListOf<Rule> Rules;
        }
    }
}

#endif
