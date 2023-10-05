
#include "y/jive/syntax/compound.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Compound:: ~Compound() noexcept
            {
            }

            void Compound:: endorse(AddressBook &book) const
            {
                const Rule &rule = *this;
                if(book.insert(rule))
                {
                    for(const NodeType *node = head; node; node=node->next)
                    {
                        (**node).endorse(book);
                    }
                }
            }

        }

    }

}
