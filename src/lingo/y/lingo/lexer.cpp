
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            scanners.release();
            (void)liberate();
        }

        void Lexer:: initialize()
        {
            withhold();
            const ScanPtr ps = this;
            mustInsert(ps);
        }

        void Lexer:: mustInsert(const ScanPtr &ps)
        {
            if(!scanners.insert(ps))
                throw Specific::Exception(name->c_str(),"unexpected multiple '%s'", ps->name->c_str());
        }

        

        void Lexer:: restart() noexcept
        {
            lexemes.release();
            history.free();
            scanner = this;
        }

        void Lexer:: call(const Caption &label)
        {
            assert(0!=scanner);
            ScanPtr *pps = scanners.search( *label );
            if(0==pps)
                throw Specific::Exception(name->c_str(),"no [%s] to be called from [%s]", label->c_str(), scanner->name->c_str());

            history << *scanner;
            scanner = & **pps;
        }

        void Lexer:: back()
        {
            assert(0!=scanner);
            if(history.size<=0) throw Specific::Exception(name->c_str(),"cannot go back from [%s]", scanner->name->c_str() );
            scanner = & history.pullTail();
        }

        namespace Lexical
        {
            namespace
            {
                //!
                class Event
                {
                public:

                    inline explicit Event(Lexer &            lx,
                                   const Hook &       hk,
                                   const Unit::Spot & sp) :
                    self(lx),
                    hook(hk),
                    spot(sp)
                    {
                    }

                    inline explicit Event(const Event & _) :
                    self(_.self),
                    hook(_.hook),
                    spot(_.spot)
                    {
                    }


                    inline virtual ~Event() noexcept {}

                    Lexer &          self;
                    Hook             hook;
                    const Unit::Spot spot;

                private:
                    Y_DISABLE_ASSIGN(Event);
                };

                class CallEvent : public Event
                {
                public:
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

                    inline Outcome operator()(const Token &token)
                    {
                        hook(token);              // execute hook
                        self.call(uuid);          // call other scanner
                        return Outcome(spot);     // return control outcome wit spot
                    }

                    const Caption uuid; // subscanner to call

                private:
                    Y_DISABLE_ASSIGN(CallEvent);
                };

            }

            Callback Scanner:: makeCall(Lexer         &  lexer,
                                        const Caption &  goal,
                                        const Hook    &  hook,
                                        const Unit::Spot spot)
            {
                const CallEvent event(goal,lexer,hook,spot);
                return Callback(event);
            }
        }

    }

}


