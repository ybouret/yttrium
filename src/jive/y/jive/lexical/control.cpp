
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
                //______________________________________________________________
                //
                //
                //! Base class for a control event
                //
                //______________________________________________________________
                class Event
                {
                public:
                    //__________________________________________________________
                    //
                    // Defintions
                    //__________________________________________________________
                    static const Message LXIO = LX_EMIT | LX_DROP; //!< non-control flags
                    static const Message MASK = ~LXIO;             //!< mask to exclude I/O flags

                    //__________________________________________________________
                    //
                    //! process token and unfold event
                    //__________________________________________________________
                    Message operator()(const Token &token)
                    {
                        const Message res = hatch(token) & MASK;
                        unfold();
                        return res | LX_CNTL;
                    }

                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    inline virtual ~Event()               noexcept {}

                protected:
                    inline explicit Event(Lexer &lx, const Callback &cb) : lexer(lx), hatch(cb) {}
                    inline explicit Event(const Event &_)   : lexer(_.lexer), hatch(_.hatch) {}


                    Lexer    &lexer;
                    Callback  hatch;

                private:
                    Y_DISABLE_ASSIGN(Event);
                    virtual void unfold() = 0;

                };
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

            const char * const Scanner:: BackPrefix = "<==";

            namespace
            {
                //______________________________________________________________
                //
                //
                //! Back event: process token and come back
                //
                //______________________________________________________________
                class Back : public Event
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    inline explicit Back(const Callback &cb, Lexer &lx) : Event(lx,cb) { }
                    inline explicit Back(const Back &_) : Event(_)                     { }
                    inline virtual ~Back() noexcept {}


                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                private:
                    Y_DISABLE_ASSIGN(Back);
                    inline virtual void unfold() { lexer.back_(); }
                };

            }

            void Scanner:: backOn(const Motif    &motif,
                                  const Callback &leave,
                                  Lexer          &lexer)
            {
                //assert(lexer.owns(*this));

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

            //______________________________________________________________
            //
            //
            //! base class for Jump/Call
            //
            //______________________________________________________________
            class JMP : public Event
            {
            public:
                inline virtual ~JMP() noexcept {}

            protected:
                inline explicit JMP(const Tag      &to,
                                    const Callback &cb,
                                    Lexer          &lx) :
                Event(lx,cb),
                where(to)
                {
                }

                inline explicit JMP(const JMP &_) :
                Event(_),
                where(_.where)
                {
                }

                const Tag where;

            private:
                Y_DISABLE_ASSIGN(JMP);
            };




            const char * const Scanner::JumpPrefix = "-->";

            class Jump : public JMP
            {
            public:
                inline explicit Jump(const Tag      &to,
                                     const Callback &cb,
                                     Lexer &         lx):
                JMP(to,cb,lx) {}

                inline explicit Jump(const Jump &_) : JMP(_) {}
                inline virtual ~Jump() noexcept {}



            private:
                Y_DISABLE_ASSIGN(Back);

                virtual void unfold()
                {
                    lexer.jump_(*where);
                }
            };

            const char * const Scanner::CallPrefix = "==>";
            
            class Call : public Event
            {
            public:
                inline explicit Call(const Tag      &to,
                                     const Callback &cb,
                                     Lexer &         lx) :
                Event(lx,cb),
                where(to)
                { 
                }

                inline  explicit Call(const Call &_) :
                Event(_),
                where(_.where)
                { 
                }

                inline virtual ~Call() noexcept 
                {
                }



            private:
                Y_DISABLE_ASSIGN(Back);
                const Tag     where; //!< target scanner

                virtual void unfold()
                {
                    lexer.call_(*where);
                }
            };



            void Scanner:: jumpOn(const Motif    &motif,
                                  const Tag      &where,
                                  const Callback &enter,
                                  Lexer          &lexer )
            {
                //assert(lexer.owns(*this));
                const Jump       would(where,enter,lexer);
                const Callback   doing(would);
                const String     label = JumpPrefix + *where;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }

            void Scanner:: callOn(const Motif    &motif,
                                  const Tag      &where,
                                  const Callback &enter,
                                  Lexer          &lexer )
            {
                //assert(lexer.owns(*this));
                const Call       would(where,enter,lexer);
                const Callback   doing(would);
                const String     label = CallPrefix + *where;
                Action::Pointer  action( new Action(label,motif,doing) );
                submitCode(action);
            }

        }

    }

}

