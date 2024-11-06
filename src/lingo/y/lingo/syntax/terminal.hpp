
//! \file

#ifndef Y_Lingo_Syntax_Terminal_Included
#define Y_Lingo_Syntax_Terminal_Included 1

#include "y/lingo/syntax/rule.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            
            class Terminal : public Rule
            {
            public:
                enum Kind
                {
                    Standard,
                    Univocal
                };

                enum Role
                {
                    Semantic,
                    Dividing
                };

                static const uint32_t UUID = Y_FOURCC('T','E','R','M');

                template <typename NAME> inline
                explicit Terminal(const NAME & _name,
                                  const Kind   _kind,
                                  const Role   _role) :
                Rule(_name,UUID),
                kind(_kind),
                role(_role)
                {
                    Y_Lingo_Syntax_Rule(Terminal);
                }

                virtual ~Terminal() noexcept;


                virtual void viz(OutputStream &fp)        const;
                virtual bool accepts(Y_Lingo_Syntax_Args) const;
                virtual bool robust() const noexcept; //!< true

                const Kind kind;
                const Role role;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}


#endif

