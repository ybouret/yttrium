
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
                    inline Back(const Callback &cb,
                                Lexer          &lx) :
                    leave(cb),
                    lexer(lx)
                    {
                    }

                    inline ~Back() noexcept {}

                    inline Back(const Back &_) :
                    leave(_.leave),
                    lexer(_.lexer)
                    {
                    }



                    Message operator()(const Token &token)
                    {
                        const Message msg = leave(token);
                        lexer.back_();
                        return msg | LX_CNTL;
                    }

                    Callback leave;
                    Lexer   &lexer;

                private:
                    Y_DISABLE_ASSIGN(Back);
                };

            }

            void Scanner:: backOn(const Motif    &motif,
                                  const Callback &leave,
                                  Lexer          &lexer)
            {
                assert(lexer.owns(*this));
                
                const Back       would(leave,lexer);
                const Callback   doing(would);
                const String     label = "back$" + *name;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }
        }


    }

}

