
#include "y/lingo/syntax/translator.hpp"
#include "y/lingo/syntax/xlist.hpp"
#include "y/lingo/syntax/rule.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Translator:: ~Translator() noexcept
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
                    case XNode::Terminal: pushTerminal(node->rule.name,node->lexeme()); break;
                    case XNode::Internal: callInternal(node->rule.name,node->branch()); break;
                }
            }

            std::ostream & Translator:: indent() const
            {
                return Core::Indent(std::cerr << "|_",deep,'_');
            }

            void Translator:: onNotFound(const Caption &    label,
                                         const char * const where) const
            {
                switch(policy)
                {
                    case Permissive: break;
                    case Restricted: throw Specific::Exception(name->c_str(), "missing %s('%s')",where,label->c_str());
                }
            }

            void Translator:: pushTerminal(const Caption &tlabel, const Lexeme &lexeme)
            {
                if(verbose)
                {
                    std::ostream & os = indent()  << "push " << '[' << tlabel << ']';
                    if(lexeme.size>0)  os << '=' << lexeme.toPrintable();
                    os << std::endl;
                }

                OnTerminal * const proc = tmap.search(tlabel);
                if(0!=proc)
                    (*proc)(lexeme);
                else
                    onNotFound(tlabel, "push Terminal");

            }


            void Translator:: callInternal(const Caption &ilabel, const XList &branch)
            {
                ++Coerce(deep);
                for(const XNode *node=branch.head;node;node=node->next)
                    walk(node);
                --Coerce(deep);

                if(verbose) indent() << "call " << '[' << ilabel << ']' << '/' << branch.size << std::endl;
                OnInternal * const proc = imap.search(ilabel);
                if(0!=proc)
                    (*proc)(branch.size);
                else
                    onNotFound(ilabel, "call to Internal");
            }

            void Translator:: on(const Caption    & label,
                                 const OnTerminal & tproc)
            {
                if( !tmap.insert(label,tproc) )
                    throw Specific::Exception(name->c_str(),"multiple OnTerminal('%s')", label->c_str() );
            }

            void Translator:: on(const Caption    & label,
                                 const OnInternal & iproc)
            {
                if( !imap.insert(label,iproc) )
                    throw Specific::Exception(name->c_str(),"multiple OnInternal('%s')", label->c_str() );
            }
        }

    }

}

