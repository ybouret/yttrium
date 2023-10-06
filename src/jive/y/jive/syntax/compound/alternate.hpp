//! \file

#ifndef Y_Jive_Syntax_Alternate_Included
#define Y_Jive_Syntax_Alternate_Included 1

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
            //! Alternation of Rule(s)
            //
            //
            //__________________________________________________________________
            class Alternate : public Compound
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('A','L','T',':'); //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! name and uuid
                template <typename ID>
                explicit Alternate(const ID &id) : Compound(id,UUID)
                {
                }

                //! cleanup
                virtual ~Alternate() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! accept the first matching rule on manifest
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;


                //! helper
                Alternate & operator |=(const Rule &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };

        }

    }

}

#endif
