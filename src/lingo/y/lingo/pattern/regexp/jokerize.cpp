#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/joker/all.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        void RXC:: Jokerize(Patterns &p, const char joker)
        {
            assert(p.size>0);
            Pattern * const motif = p.popTail();
            switch(joker)
            {
                case '*': p.pushTail( MoreThan::Create(motif,0)  ); break;
                case '+': p.pushTail( MoreThan::Create(motif,1)  ); break;
                case '?': p.pushTail( Optional::Create(motif)    ); break;
                case '&': p.pushTail( Pattern::IgnoreCase(motif) ); break;
                default:
                    delete motif;
                    throw Specific::Exception(CallSign, "unexpected joker '%c'",joker);
            }
        }

    }

}


#include "y/text/ascii/convert.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        void RXC:: jkBraced(Patterns &p)
        {
            const char * const start = curr-1;
            const char * const init  = curr;
            assert(*start == LBRACE);

            //------------------------------------------------------------------
            //
            // look for RBRACE
            //
            //------------------------------------------------------------------
            while(curr<last && RBRACE != *curr)
                ++curr;
            if(curr>=last)
                throw Specific::Exception(CallSign, "missing '%c' after '%s'", RBRACE, start);


            //------------------------------------------------------------------
            //
            // extract content
            //
            //------------------------------------------------------------------
            String content(init,curr-init);
            ++curr;   // skip '}'
            std::cerr << "content='" << content << "'" << std::endl;

            if(content.size()<=0)
                throw Specific::Exception(CallSign, "empty braces in '%s'", expr);

            //------------------------------------------------------------------
            //
            // analyze content
            //
            //------------------------------------------------------------------
            const char c = content[1];

            if( isalpha(c) || '_' == c)
            {
                // assuming alias
                if( 0 == dict ) throw Specific:: Exception(CallSign, "no dictionary for '%s'", content.c_str());
                p.pushTail( (*dict)(content) );
                return;
            }

            if(isdigit(c))
            {
                // assuming joker
                if(p.size<=0) throw Specific::Exception(CallSign, "no pattern before braces in '%s'",expr);
                char *buffer = (char *)content.c_str();
                char *comma  = strchr(buffer,',');
                if(comma)
                {
                    // counting or repeating
                    *(comma++) = 0;
                    const String lo   = buffer;
                    const String hi   = comma;
                    const size_t nmin = ASCII::Convert::To<size_t>(lo,"Regular Expression Min Count");
                    if(hi.size()>0)
                    {
                        const size_t nmax = ASCII::Convert::To<size_t>(hi,"Regular Expression Max Count");
                        p.pushTail( Counting::Create(p.popTail(), nmin, nmax));
                        return;
                    }
                    else
                    {
                        p.pushTail( MoreThan::Create(p.popTail(),nmin));
                        return;
                    }

                    
                }
                else
                {
                    // counting
                    const size_t n = ASCII::Convert::To<size_t>(content,"Regular Expression Counting");
                    p.pushTail( Counting::Create(p.popTail(), n, n));
                    return;
                }
            }


            throw Specific::Exception(CallSign,"invalid char '%c' in braces of '%s'",c,expr);
        }
    }

}

