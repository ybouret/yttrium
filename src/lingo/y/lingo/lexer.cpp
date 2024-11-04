
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            analyzers.release();
            (void)liberate();
        }

        void Lexer:: initialize()
        {
            withhold();
            const Analyzer::Pointer ps = this;
            mustInsert(ps);
        }

        void Lexer:: mustInsert(const Analyzer::Pointer &ps)
        {
            if(!analyzers.insert(ps))
                throw Specific::Exception(name->c_str(),"unexpected multiple '%s'", ps->name->c_str());
        }



        void Lexer:: restart() noexcept
        {
            lexemes.release();
            history.free();
            analyzer = this;
        }

        void Lexer:: call(const Caption &label)
        {
            assert(0!=analyzer);
            Analyzer::Pointer *pps = analyzers.search( *label );
            if(0==pps)
                throw Specific::Exception(name->c_str(),"no [%s] to be called from [%s]", label->c_str(), analyzer->name->c_str());

            history << *analyzer;
            analyzer = & **pps;
        }

        void Lexer:: back()
        {
            assert(0!=analyzer);
            if(history.size<=0) throw Specific::Exception(name->c_str(),"cannot go back from [%s]", analyzer->name->c_str() );
            analyzer = & history.pullTail();
        }

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
                        self.call(uuid);          // call other scanner
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
                        self.back();        // back from this lexer
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
        }

    }

}


