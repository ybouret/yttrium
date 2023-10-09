
#include "y/jive/syntax/wildcard.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Wildcard:: ~Wildcard() noexcept
            {
            }

            void Wildcard:: endorse(AddressBook &book) const
            {
                const Rule &self = *this;
                if(book.insert(self))
                    rule.endorse(book);
            }


            void Wildcard:: vizLink(OutputStream &fp) const
            {
                Arrow(fp, this, &rule);
            }
        }

    }

}

