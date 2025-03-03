
#include "y/lingo/lexical/add-on/string.hpp"
#include "y/lingo/pattern/all.hpp"

#include "y/text/ascii/printable.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/system/exception.hpp"
#include <cstring>

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

            static const char   Cntl[] = "abrntvf0";
            static const char   Code[] = "\a\b\r\n\t\v\f\0";
            //static const size_t nCtl   = sizeof(Cntl)/sizeof(Cntl[0]) - 1;

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


                //--------------------------------------------------------------
                //
                // Escape Raw
                //
                //--------------------------------------------------------------
                {
                    String raw;
                    raw += '\\';
                    raw += enterChar;
                    raw += leaveChar;
                    Logic *          p     = new And();
                    AutoPtr<Pattern> motif = p;
                    p->add('\\');
                    p->pushTail( Pattern::Among(raw) );
                    const Caption    rname("escRaw");
                    const Callback   xcode(this, & String_:: onEscRaw);
                    add( Rule::Create(rname, motif, xcode) );
                }

                //--------------------------------------------------------------
                //
                // Escaped Control
                //
                //--------------------------------------------------------------
                {
                    Logic *          p     = new And();
                    AutoPtr<Pattern> motif = p;
                    p->add('\\');
                    p->pushTail( Pattern::Among(Cntl) );
                    const Caption    rname("escCtl");
                    const Callback   xcode(this, & String_:: onEscCtl);
                    add( Rule::Create(rname, motif, xcode) );
                }

                //--------------------------------------------------------------
                //
                // Escaped Control
                //
                //--------------------------------------------------------------
                {
                    on("escHex", "\\\\x[:xdigit:][:xdigit:]", *this, & String_ :: onEscHex );
                }

            }

            Outcome String_:: onCore(const Token &token)
            {
                content += token;
                return Outcome(Unit::Drop, Unit::Bulk);
            }

            Outcome String_:: onEscRaw(const Token &esc)
            {
                assert(2==esc.size);
                content += *esc.tail;
                return Outcome(Unit::Drop, Unit::Bulk);
            }

            Outcome String_:: onEscCtl(const Token &esc)
            {
                assert(2==esc.size);
                assert(sizeof(Cntl) == sizeof(Code));
                const char         c = **esc.tail;                    // get char
                const char * const s = strchr(Cntl, c); assert(0!=s); // find it in Cntl
                const char         k = Code[s-Cntl];                  // translate
                **content.pushTail( new Char(*esc.head) ) = k;        // duplicate Char and change its content
                return Outcome(Unit::Drop, Unit::Bulk);
            }

            Outcome String_:: onEscHex(const Token &esc)
            {
                assert(4==esc.size);
                const int  lo = Hexadecimal::ToDecimal( **(esc.tail)       ); assert(lo>=0);
                const int  hi = Hexadecimal::ToDecimal( **(esc.tail->prev) ); assert(hi>=0);
                const char ch = char( (hi<<4) | lo );
                **content.pushTail( new Char(*esc.head) ) = ch;
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

