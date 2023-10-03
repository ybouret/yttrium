#include "y/jive/lexical/plugin/string.hpp"
#include "y/system/exception.hpp"
#include "y/stream/xmlog.hpp"

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
                //static const char *tab[] = { "", "", "" };

                switch(sk)
                {
                    case DQUOTES:  return DQUOTES_RX;
                    case QUOTES:   return "'";
                    case BRACKETS: return "<";
                }
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
                switch(sk)
                {
                    case DQUOTES:  ret(DQUOTES_RX); break;
                    case QUOTES:   ret("'");        break;
                    case BRACKETS: ret(">");       break;
                }

                String_ &self = *this;
                self("com",Com, self, & String_::onCom);
            }

            Message String_:: onCom(const Token &token)
            {
                content << token;
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
                return 0;
            }

        }

    }

}

