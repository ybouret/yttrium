
#include "y/lingo/syntax/translator.hpp"
#include "y/lingo/syntax/xlist.hpp"


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Translator:: ~Translator() noexcept
            {
            }

            Translator:: Translator() :
            tmap(),
            imap(),
            deep(0),
            verbose(false)
            {

            }

            void Translator:: init()
            {

            }

            void Translator:: quit()
            {
            }

            

            void Translator:: operator()(const XNode &root)
            {
                Coerce(deep) = 0;
                if(verbose) indent() << "<init>" << std::endl;
                init();
                walk( &root );
                if(verbose) indent() << "<quit>" << std::endl;
                quit();
            }

            void Translator:: walk(const XNode * const node)
            {
                assert(0!=node);
                switch(node->type)
                {
                    case XNode::Terminal: onTerminal(node->name(),node->lexeme()); break;

                    case XNode::Internal: onInternal(node->name(),node->branch()); break;
                }
            }

            std::ostream & Translator:: indent() const
            {
                return Core::Indent(std::cerr << "|_",deep,'_');
            }


            void Translator:: onTerminal(const String &name, const Lexeme &unit)
            {
                if(verbose)
                {
                    std::ostream & os = indent()  << "push " << '[' << name << ']';
                    if(unit.size)  os << '=' << unit.toPrintable();
                    os << std::endl;
                }
            }


            void Translator:: onInternal(const String &name, const XList &chld)
            {
                ++Coerce(deep);
                for(const XNode *node=chld.head;node;node=node->next)
                {
                    walk(node);
                }
                --Coerce(deep);
                if(verbose)
                {
                    indent() << "call " << '[' << name << ']' << '/' << chld.size << std::endl;
                }
            }


        }

    }

}

