
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            namespace
            {
                //______________________________________________________________
                //
                //
                //
                //! base class to Call/Back
                //
                //
                //______________________________________________________________
                class Event
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! setup
                    inline explicit Event(Lexer &            lx,
                                          const Hook &       hk,
                                          const Unit::Spot & sp) :
                    self(lx),
                    hook(hk),
                    spot(sp)
                    {
                    }

                    //! copy for functor
                    inline explicit Event(const Event & _) :
                    self(_.self),
                    hook(_.hook),
                    spot(_.spot)
                    {
                    }


                    //! cleanup
                    inline virtual ~Event() noexcept {}

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    Lexer &          self; //!< used lexer
                    Hook             hook; //!< event hook
                    const Unit::Spot spot; //!< event spot

                private:
                    Y_DISABLE_ASSIGN(Event);
                };

                //______________________________________________________________
                //
                //
                //
                // Calling a named scanner
                //
                //
                //______________________________________________________________
                class CallEvent : public Event
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    inline explicit CallEvent(const Caption &    id,
                                              Lexer &            lx,
                                              const Hook &       hk,
                                              const Unit::Spot & sp) :
                    Event(lx,hk,sp),
                    uuid(id)
                    {
                    }

                    inline explicit CallEvent(const CallEvent &_) :
                    Event(_),
                    uuid(_.uuid)
                    {
                    }

                    inline virtual ~CallEvent() noexcept
                    {
                    }

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    inline Outcome operator()(const Token &token)
                    {
                        hook(token);              // execute hook
                        self.performCall(uuid);          // call other scanner
                        return Outcome(spot);     // return control outcome wit spot
                    }

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    const Caption uuid; //!< subscanner to call

                private:
                    Y_DISABLE_ASSIGN(CallEvent);
                };


                //______________________________________________________________
                //
                //
                //
                // Back from a  scanner
                //
                //
                //______________________________________________________________
                class BackEvent : public Event
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    inline explicit BackEvent( Lexer &            lx,
                                              const Hook &       hk,
                                              const Unit::Spot & sp) :
                    Event(lx,hk,sp)
                    {
                    }

                    inline explicit BackEvent(const BackEvent &_) :
                    Event(_)
                    {
                    }

                    inline virtual ~BackEvent() noexcept
                    {
                    }

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    inline Outcome operator()(const Token &token)
                    {
                        hook(token);          // execute hook
                        self.performBack();        // back from this lexer
                        return Outcome(spot); // return control outcome wit spot
                    }



                private:
                    Y_DISABLE_ASSIGN(CallEvent);
                };

            }

            Callback Analyzer:: makeCall(const Caption &  goal,
                                         const Hook    &  hook,
                                         const Unit::Spot spot)
            {
                const CallEvent event(goal,lexer,hook,spot);
                return Callback(event);
            }

            Callback Analyzer:: makeBack(const Hook    &  hook,
                                         const Unit::Spot spot)
            {
                const BackEvent event(lexer,hook,spot);
                return Callback(event);
            }

            Analyzer:: Analyzer( Lexer &lx, const String &     id) : Scanner(id,lx), lexer(lx) {}
            Analyzer:: Analyzer( Lexer &lx, const char * const id) : Scanner(id,lx), lexer(lx) {}

        }

    }

}

