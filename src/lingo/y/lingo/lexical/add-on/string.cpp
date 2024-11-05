
#include "y/lingo/lexical/add-on/string.hpp"
#include "y/lingo/pattern/all.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            String_:: ~String_() noexcept
            {
            }

            void String_:: enter(const Token &token)
            {
                content.release();
                content += token;
            }

            void String_:: setup(const char enterChar, const char leaveChar)
            {
                //--------------------------------------------------------------
                //
                // core = all 7-bit minus backslash and I/O chars
                //
                //--------------------------------------------------------------
                {
                    AutoPtr<Logic> p = new Or();
                    {
                        static const unsigned b = '\\';
                        const unsigned u = enterChar;
                        const unsigned v = leaveChar;
                        for(unsigned i=32;i<128;++i)
                        {
                            if( i==b || i == u || i == v ) continue;
                            p->add(i);
                        }
                    }

                    {
                        AutoPtr<Pattern> motif = Pattern::Optimize( p.yield() );
                        const Caption    rname("core");
                        const Callback   xcode(this, & String_:: onCore);
                        add( Rule::Create(rname, motif, xcode) );
                        //{ const String  dotName = *name + ".dot"; GraphViz::Vizible::DotToPng(dotName, *motif); }
                    }
                }

                
            }

            Outcome String_:: onCore(const Token &token)
            {
                content += token;
                return Outcome(Unit::Drop, Unit::Bulk);
            }
        }

    }

}

#include "y/lingo/lexer.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            void String_:: leave(const Token &token)
            {
                content += token;
                lexer.put( produce(content) );
            }
        }

    }

}

