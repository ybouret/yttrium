
#include "y/jive/lexer.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {

            const char * const Scanner:: BackPrefix = "back$";

            namespace
            {
                class Back
                {
                public:
                    // C++
                    inline  Back(const Callback &cb, Lexer &lx) : leave(cb), lexer(lx) { }
                    inline  Back(const Back &_) : leave(_.leave), lexer(_.lexer)       { }
                    inline ~Back() noexcept {}


                    // functor call
                    inline Message operator()(const Token &token)
                    {
                        const Message msg = leave(token);
                        lexer.back_();
                        return msg | LX_CNTL;
                    }


                    Callback leave; //!< todo before leave
                    Lexer   &lexer; //!< lexer

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
                const String     label = BackPrefix + *name;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }
        }


    }

}


namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {

            const char * const Scanner::JumpPrefix = "jump@";
            const char * const Scanner::CallPrefix = "call@";

            class Jump
            {
            public:
                // C++
                inline  Jump(const String   &to,
                             const Callback &cb,
                             Lexer &         lx,
                             const bool      isCall) :
                where(to),
                leave(cb),
                lexer(lx),
                xcode(isCall ? &Lexer::call_ : &Lexer::jump_ )
                { }

                inline  Jump(const Jump &_) : 
                where(_.where),
                leave(_.leave),
                lexer(_.lexer),
                xcode(_.xcode)
                { }

                inline ~Jump() noexcept {}


                // functor call
                inline Message operator()(const Token &token)
                {
                    const Message msg = leave(token);
                    lexer.back_();
                    return msg | LX_CNTL;
                }

                const String  where; //!< target scanner
                Callback      leave; //!< todo before leave
                Lexer        &lexer; //!< lexer
                Lexer::Branch xcode; //!< to call

            private:
                Y_DISABLE_ASSIGN(Back);
            };

            void Scanner:: jumpOn(const Motif    &motif,
                                  const String   &where,
                                  const Callback &enter,
                                  Lexer          &lexer,
                                  const bool      iCall)
            {
                assert(lexer.owns(*this));
                const Jump would(where,enter,lexer,iCall);
                const Callback   doing(would);
                const String     label = (iCall ? CallPrefix : JumpPrefix) + *name;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }

        }

    }

}

