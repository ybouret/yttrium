#include "y/jive/syntax/compound.hpp"
#include "y/associative/address-book.hpp"

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
                    for(const NodeType *sub = head; sub; sub=sub->next)
                    {
                        (**sub).endorse(book);
                    }
                }
            }

            void Compound:: vizLink(OutputStream &fp) const
            {
                unsigned   idx  = 1;
                const bool show = size>1;
                for(const NodeType *sub = head; sub; sub=sub->next, ++idx)
                {
                    const Rule &r = **sub;
                    Arrow(fp, this, &r);
                    if(show)
                    {
                        fp("[label=\"%u\"]",idx);
                    }
                    Endl(fp);
                }
            }

        }

    }

}
