

#include "y/jive/syntax/terminal.hpp"
#include "y/associative/address-book.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            Terminal:: ~Terminal() noexcept
            {
            }

            const char * const Terminal::CallSign = "Terminal";
            
            bool Terminal:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                Y_XML_SECTION_OPT(xml,CallSign," name='" << name << "'");

                Lexeme *lexeme = lexer.get(source);
                if(0==lexeme)
                {
                    Y_XMLOG(xml, "<EOS>");
                    return false; // End Of Stream
                }
                else
                {
                    assert(0==lexeme->next);
                    assert(0==lexeme->prev);
                    if( *(lexeme->name) == *name )
                    {
                        Y_XMLOG(xml, "<success>");
                        XNode::Expand(tree, XNode::Create(*this,lexeme) );
                        return true;
                    }
                    else
                    {
                        Y_XMLOG(xml, "<failure/> (got [" << lexeme->name << "])");
                        lexer.put(lexeme);
                        return false;
                    }
                }
            }

            void Terminal:: endorse(AddressBook &book) const
            {
                const Rule &rule = *this;
                book |= rule;
            }

            void Terminal:: vizCore(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                fp << ",shape=box";
                if(univocal)
                {
                    switch(property)
                    {
                        case IsRegular:
                            fp << ",style=\"rounded,filled\"";
                            break;

                        case IsDivider:
                            fp << ",style=\"rounded,dotted\"";
                            break;
                    }
                }
                else
                {
                    switch(property)
                    {
                        case IsRegular:
                            fp << ",style=\"filled\"";
                            break;

                        case IsDivider:
                            fp << ",style=\"dotted\"";
                            break;
                    }
                }
                fp << ']';
                Endl(fp);
            }

            void Terminal:: vizLink(OutputStream &) const
            {
            }

            
            bool Terminal:: isFrail() const noexcept
            {
                return false;
            }

        }

    }

}


