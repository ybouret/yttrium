
#include "y/jive/lexer.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {


        namespace Lexical
        {

            namespace
            {
                class Back
                {
                public:
                    inline Back(Lexer &lx) noexcept :
                    lexer(lx)
                    {
                    }

                    inline ~Back() noexcept {}

                    inline Back(const Back &other) noexcept :
                    lexer(other.lexer)
                    {
                    }



                    Message operator()(const Token &)
                    {
                        lexer.back_();
                        return LX_CNTL;
                    }

                    Lexer &lexer;

                private:
                    Y_DISABLE_ASSIGN(Back);
                };

            }

            void Scanner:: backOn(const Motif &motif, Lexer &lexer)
            {
                const Back       _back(lexer);
                const Callback   doing( _back );
                const String     label = "back$" + *name;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }
        }


    }

}

