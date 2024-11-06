#include "y/lingo/syntax/xlist.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            XList:: ~XList() noexcept
            {
                std::cerr << "~XList" << std::endl;
            }

            XList:: XList() noexcept
            {
                std::cerr << "XList" << std::endl;
            }

            void XList:: backToLexer(Lexer &lexer) noexcept
            {
                while(size>0) XNode::BackToLexer(lexer, popTail() );
            }

            XNode:: ~XNode() noexcept
            {
                switch(type)
                {
                    case Terminal:
                        if(0!=unit) delete unit;
                        break;

                    case Internal:
                        Destruct( & list() );
                        break;
                }
                zero();
            }

            void XNode:: BackToLexer(Lexer &lexer, XNode *const node) noexcept
            {
                assert(0!=node);
                switch(node->type)
                {
                    case Terminal:
                        assert(0!=node->unit);
                        lexer.put(node->unit);
                        node->unit = 0;
                        break;

                    case Internal:
                        node->list().backToLexer(lexer);
                        break;
                }
                delete node;
            }

        }

    }

}


