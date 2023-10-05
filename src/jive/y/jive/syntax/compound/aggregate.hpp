
//! \file

#ifndef Y_Jive_Syntax_Aggregate_Included
#define Y_Jive_Syntax_Aggregate_Included 1

#include "y/jive/syntax/compound.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Aggregation of Rule(s)
            //
            //
            //__________________________________________________________________
            class Aggregate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A','G','G',':'); //!< alias


                //! forward name and uuid
                template <typename ID>
                explicit Aggregate(const ID &id) : Compound(id,UUID) {}

                //! cleanup
                virtual ~Aggregate() noexcept;


                //! accept all rules on manifest
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

        }

    }

}

#endif
