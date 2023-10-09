#include "y/jive/lexical/plugin/string.hpp"
#include "y/system/exception.hpp"
#include "y/stream/xmlog.hpp"
#include "y/text/ascii/printable.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            String_:: ~String_() noexcept
            {
            }

            static const char DQUOTES_RX[] = "\\\"";

            const char *  String_:: GetEntryRX(const Kind sk) noexcept
            {
                switch(sk)
                {
                    case DQUOTES: break;
                    case QUOTES:   return "'";
                    case BRACKETS: return "<";
                }

                return DQUOTES_RX;
            }



            const char * const String_:: Com =
            "["
            " !#$%&()*+,-./"
            "0123456789:;=?"
            "@ABCDEFGHIJKLMNO"
            "PQRSTUVWXYZ\\[\\]^_"
            "`abcdefghijklmno"
            "pqrstuvwxyz{|}~"
            "]";

            void String_:: initialize(const Kind sk)
            {
                //--------------------------------------------------------------
                //
                // coming back !
                //
                //--------------------------------------------------------------
                switch(sk)
                {
                    case DQUOTES:  ret(DQUOTES_RX); break;
                    case QUOTES:   ret("'");        break;
                    case BRACKETS: ret(">");       break;
                }

                //--------------------------------------------------------------
                //
                // common to everyone
                //
                //--------------------------------------------------------------
                String_ &self = *this;
                self("com",Com, self, & String_::onCom);

                //--------------------------------------------------------------
                //
                // adding authorized
                //
                //--------------------------------------------------------------
                switch(sk)
                {
                    case DQUOTES:
                        self("oth","['<>]", self, &String_::onCom);
                        break;

                    case QUOTES:
                        self("oth","[\"<>]", self, &String_::onCom);
                        break;

                    case BRACKETS:
                        self("oth","['\"]", self, &String_::onCom);
                }

                //--------------------------------------------------------------
                //
                // adding specific escaped
                //
                //--------------------------------------------------------------
                switch(sk)
                {
                    case DQUOTES:
                        self("esc0","\\x5c(\\x22|\\x27)",self,& String_::eEcho);
                        break;

                    case QUOTES:
                        self("esc0","\\x5c(\\x22|\\x27)",self,& String_::eEcho);
                        break;

                    case BRACKETS:
                        self("esc0","\\x5c(<|>)",self,& String_::eEcho);
                        break;
                }

                self("esc1","\\x5c[\\x5c\\x2f]",self, & String_::eEcho);
                self("esc2","\\x5c[nrtfb]",     self, & String_::eCode);

            }

            Message String_:: onCom(const Token &token)
            {
                content << token;
                return LX_DROP;
            }

            Message String_:: eEcho(const Token &token)
            {
                assert(2==token.size);
                content.pushTail( new Char( *token.tail ) );
                return LX_DROP;
            }


            static inline
            void grow(Token &content, const Char * const source, const char replace)
            {
                assert(0!=source);
                Char *ch = new Char(*source);
                content.pushTail(ch);
                Coerce(**ch) = replace;
            }

            Message String_:: eCode(const Token &token)
            {
                assert(2==token.size);
                const Char * const source = token.head;
                const uint8_t      select = **token.tail;
                switch(select)
                {
                    case 'n' : grow(content,source,'\n'); break;
                    case 'r' : grow(content,source,'\r'); break;
                    case 't' : grow(content,source,'\t'); break;
                    case 'f' : grow(content,source,'\f'); break;
                    case 'b' : grow(content,source,'\b'); break;
                    default:
                        throw Specific::Exception(name->c_str(), "invalid escaped char '%s'", ASCII::Printable::Char[select]);

                }
                return LX_DROP;
            }


            Message String_:: enter(const Token &token)
            {
                content.release();
                content << token;
                return 0;
            }

            Message String_:: leave(const Token &token)
            {
                content << token;
                Y_XMLOG( getXMLog(), "<" << name << "> = " << content.toPrintable());
                pushUnit(content);
                return 0;
            }

        }

    }

}

