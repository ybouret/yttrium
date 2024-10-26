#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/printable.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        
        Pattern * RXC:: subExpr()
        {
            AutoPtr<Logic>     motif = new And();
            const char * const start = curr;

            while(curr<last)
            {
                const char c = *(curr++);
                
                switch(c)
                {

                        //------------------------------------------------------
                        //
                        // grouping
                        //
                        //------------------------------------------------------
                    case LPAREN:
                        ++depth;
                        motif->pushTail( subExpr() );
                        break;

                    case RPAREN:
                        if(--depth<0) throw Specific::Exception(CallSign,"extraneous '%s' in '%s'",ASCII::Printable::Text(c),start);
                        goto END;

                        //------------------------------------------------------
                        //
                        // alias
                        //
                        //------------------------------------------------------
                    case LBRACE: jkBraced(*motif); break;

                        //------------------------------------------------------
                        //
                        // escaped sequence
                        //
                        //------------------------------------------------------
                    case '\\':
                        motif->pushTail( subExprEsc() );
                        break;

                    case '.':
                        motif->pushTail( posix::dot() );
                        break;

                    case RBRACK:
                        throw Specific::Exception(CallSign, "RBRACK must be escaped in '%s'", expr);

                        //------------------------------------------------------
                        //
                        // alternation
                        //
                        //------------------------------------------------------
                    case ALT:
                        if(motif->size<=0)
                            throw Specific::Exception(CallSign,"missing expression before '%s' in '...%s'",ASCII::Printable::Text(c),start);

                    {
                        AutoPtr<Logic> alt = new Or();
                        alt->pushTail( motif.yield() ); // lhs: current motif
                        alt->pushTail( subExpr() );     // rhs: next expression
                        assert(alt->size>0);
                        return alt.yield();
                    }
                        //------------------------------------------------------
                        //
                        // sub bank
                        //
                        //------------------------------------------------------
                    case LBRACK:
                        motif->pushTail( subBank() );
                        break;

                        //------------------------------------------------------
                        //
                        // one byte joker
                        //
                        //------------------------------------------------------
                    case '*':
                    case '+':
                    case '?':
                    case '&':
                        if(motif->size<=0) throw Specific::Exception(CallSign,"missing expression before '%s' after '%s'",ASCII::Printable::Text(c),start);
                        Jokerize(*motif,c);
                        break;

                        //------------------------------------------------------
                        //
                        // new byte
                        //
                        //------------------------------------------------------
                    default:
                        motif->add(c);
                        break;
                }

            }

        END:
            if( motif->size <= 0 )
                return new Void();


            return motif.yield();
        }


    }

}

