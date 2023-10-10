
//! \file

#ifndef Y_Jive_Syntax_Analyzer_Included
#define Y_Jive_Syntax_Analyzer_Included 1

#include "y/jive/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {


            class Analyzer
            {
            public:
                virtual ~Analyzer() noexcept;
                explicit Analyzer() noexcept;

                virtual void onTerminal(const String &name,
                                        const Token  &data);
                
                virtual void onInternal(const String &name,
                                        const size_t  size);

                void  run(const XNode &root);

                std::ostream & indent() const;

                const unsigned depth;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                void walk(const XNode *root);

            };

        }
    }

}

#endif
