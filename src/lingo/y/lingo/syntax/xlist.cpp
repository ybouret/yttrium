#include "y/lingo/syntax/xlist.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            XList:: ~XList() noexcept
            {
            }

            XList:: XList() noexcept
            {
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




            void XNode:: fusion(XNode * const node) noexcept
            {
                assert(0!=node);
                assert(Internal==type);
                list().pushTail(node)->sire = this;
            }


            void XNode:: fusion(XList &other) noexcept
            {
                assert(Internal==type);
                while(other.size>0) fusion( other.popHead() );
            }


            const XNode   & XNode:: last() const noexcept
            {
                //std::cerr << "last for '" << name() << "'" << std::endl;
                switch(type)
                {
                    case Terminal: return *this;
                    case Internal: break;
                }
                const XList &self = branch();
                if(self.size)
                {
                    return branch().tail->last();
                }
                else
                {
                    return *this;
                }
            }

            const Lexeme  * XNode:: lastLexeme() const noexcept
            {
                // go to bottom
                const XNode &bottom = last();

                // then go back to top, intercepting terminal
                switch(bottom.type)
                {
                    case Terminal: return & bottom.lexeme();
                    case Internal: break;
                }

                // going back branch
                for(const XNode *node=bottom.branch().tail;node;node=node->prev)
                {
                    const Lexeme * const lexeme = node->lastLexeme();
                    if(lexeme) return lexeme;
                }

                // go up if possible
                if(bottom.sire) return bottom.sire->lastLexeme();

                // nope...
                return 0;
            }



            void XNode:: Grow(XNode * &tree, XNode * const node) noexcept
            {
                assert(0!=node);
                if(0==tree)
                {
                    tree       = node;
                    node->sire = 0;
                }
                else
                {
                    assert(Internal==tree->type);
                    tree->fusion(node);
                }
            }

            bool XNode:: isWellFormed() const
            {
                switch(type)
                {
                    case Terminal:
                        if(0!=sire)
                        {
                            if(Internal!=sire->type) {
                                std::cerr << "node's sire is not Internal!!" << std::endl;
                                return false;
                            }

                            if(!sire->list().owns(this)) {
                                std::cerr << "node's sire is not node's owner!!" << std::endl;
                                return false;
                            }

                        }
                        break;

                    case Internal:
                        for(const XNode *node = list().head;node;node=node->next)
                        {
                            if(!node->isWellFormed()) return false;
                        }
                        break;
                }

                return true;
            }


            XNode:: XNode(const XNode &root) :
            rule(root.rule),
            type(root.type),
            sire(0),
            next(0),
            prev(0)
            {
                switch(type)
                {
                    case Terminal: unit = new Lexeme( *root.unit ); break;

                    case Internal: {
                        XList &self = * new ( &list() ) XList();
                        try
                        {
                            for(const XNode *sub=root.list().head;sub;sub=sub->next)
                                self.pushTail( new XNode(*sub) )->sire = this;
                        }
                        catch(...)
                        {
                            self.release();
                            throw;
                        }
                    }
                }
            }

        }

    }

}




