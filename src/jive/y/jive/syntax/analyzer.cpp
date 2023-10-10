
#include "y/jive/syntax/analyzer.hpp"
#include "y/jive/syntax/rule.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Analyzer:: ~Analyzer() noexcept
            {
            }

            Analyzer:: Analyzer() noexcept  : depth(0)
            {
            }

            std::ostream & Analyzer:: indent() const
            {
                std::cerr << "|_";
                for(size_t i=depth;i>0;--i) std::cerr << "__";
                return std::cerr;
            }

            void Analyzer:: onTerminal(const String &name,
                                       const Token  &data)
            {
                indent() << "'" << name << "'";
                if(data.size) std::cerr << "='" << data.toPrintable() << "'";
                std::cerr << std::endl;
            }

            void Analyzer:: onInternal(const String &name,
                                       const size_t size)
            {
                indent() << "'" << name << "'/" << size;

                std::cerr << std::endl;
            }

            void  Analyzer:: run(const XNode &root)
            {
                Coerce(depth) = 0;
                walk( &root );
            }


            void Analyzer:: walk(const XNode *root)
            {
                assert(0!=root);
                const String &name = *(root->rule.name);
                switch(root->type)
                {
                    case IsTerminal:
                        onTerminal(name,root->lexeme());
                        break;

                    case IsInternal: {
                        const XList &chld = root->branch();
                        ++Coerce(depth);
                        for(const XNode *node=chld.head;node;node=node->next)
                        {
                            walk(node);
                        }
                        --Coerce(depth);
                        onInternal(name,chld.size);
                    }
                }
            }
            
        }

    }

}

