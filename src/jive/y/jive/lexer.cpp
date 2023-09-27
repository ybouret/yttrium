
#include "y/jive/lexer.hpp"
#include "y/jive/source.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include "y/type/static-retainer.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {



        class Lexer:: App : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Lexical::Scanner                      Scanner;
            typedef SuffixSet<String,Scanner::Pointer>    ScannerDB;
            typedef ScannerDB::Iterator                   Iterator;
            typedef Small::SoloLightList<Scanner>         History;
            typedef void (App::*Branch)(const String &);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with genesis
            inline explicit App(Scanner &self) :
            Object(),
            genesis(  self    ),
            scanner( &genesis ),
            lexemes(          ),
            retains( genesis  ),
            content(),
            history(),
            name( *self.name ),
            xml( genesis.getXMLog() )
            {

                const Scanner::Pointer ptr( &genesis );
                if(!content.insert(ptr))
                    throw Specific::Exception(name.c_str(),"Initialization Failure!");

            }

            inline virtual ~App() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! restart all
            //__________________________________________________________________
            inline void restart() noexcept
            {
                scanner = &genesis;
                history.free();      assert(0==history.size);
                lexemes.release();   assert(0==lexemes.size);
                for(Iterator it=content.begin();it!=content.end();++it)
                    (**it).cleanup();
            }

            //__________________________________________________________________
            //
            //! processing source
            //__________________________________________________________________
            inline Lexeme * get(Source &source)
            {
                //Y_XMLOG(xml, "get lexeme");
                Y_XML_SECTION_OPT(xml,name, " get");
            PROBE:
                assert(0!=scanner);
                assert(scanner->isClean());
                if(lexemes.size)
                {
                    //----------------------------------------------------------
                    //
                    // use cache
                    //
                    //----------------------------------------------------------
                    return lexemes.popHead();
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // probe with current scanner
                    //
                    //----------------------------------------------------------
                    Lexical::Action *action = 0;
                    switch( scanner->probe(source,action) )
                    {
                        case Lexical::ReturnSuccess: {
                            assert(0!=action);
                            const Lexical::Message msg = action->perform();

                            if(msg & Lexical::LX_DROP)
                            {
                                action->motif->release();
                                goto PROBE;
                            }

                            if(msg & Lexical::LX_EMIT) return action->produce();

                            throw Exception("unhandled message");
                        }

                        case Lexical::ReturnFailure: {
                            Token bad;
                            if(source.guess(bad))
                            {
                                const String info  = bad.toPrintable();
                                Exception    excp =  Specific::Exception( name.c_str(), "syntax error '%s'", info.c_str());
                                throw bad.head->stamp(excp);
                            }
                            else
                            {
                                Exception excp = Specific::Exception( name.c_str(), "forbidden trailing blanks");
                                throw source->stamp(excp);
                            }
                        }

                        case Lexical::AtEndOfStream:
                            break;
                    }

                    //----------------------------------------------------------
                    //
                    // end of stream
                    //
                    //----------------------------------------------------------
                    return 0;
                }

            }


            inline void jump(const String &id)
            {
                assert(0!=scanner);
                Scanner::Pointer *sptr = content.search(id);
                if(!sptr) throw Specific::Exception(name.c_str(),"No <%s> to jump to",id());
                Scanner &target = **sptr;
                Y_XMLOG(xml, "[jump] from <" << scanner->name << "> to <" << target.name << ">");
                scanner = & target;
            }

            inline void call(const String &id)
            {
                assert(0!=scanner);
                Scanner::Pointer *sptr = content.search(id);
                if(!sptr) throw Specific::Exception(name.c_str(),"No <%s> to call",id());
                Scanner &target = **sptr;
                history << *scanner;
                Y_XMLOG(xml, "[call] from <" << scanner->name << "> to <" << target.name << ">");
                scanner = &target;

            }

            inline void back()
            {
                assert(0!=scanner);
                if(history.size<=0) throw Specific::Exception(name.c_str(),"cannot come back from <%s>", scanner->name->c_str());
                Scanner &target = history.pullTail();
                Y_XMLOG(xml, "[back] from <" << scanner->name << "> to <" << target.name << ">");
                scanner = &target;
            }

            inline void submit(const Scanner::Pointer &ptr)
            {
                if(!content.insert(ptr))
                    throw Specific::Exception(name.c_str(),"Multiple <%s>", ptr->name->c_str());
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Scanner &            genesis; //!< top-level scanner
            Scanner *            scanner; //!< current scanner
            Lexemes              lexemes; //!< lexemes cache
            const StaticRetainer retains; //!< helper
            ScannerDB            content; //!< scanner database
            History              history; //!< call stack
            const String        &name;    //!< genesis name
            XMLog               &xml;     //!< for verbose output

        private:
            Y_DISABLE_COPY_AND_ASSIGN(App);

        public:
            class Jump
            {
            public:
                inline Jump(App &      who,
                            const Tag &uid,
                            const bool emitLexeme,
                            const bool willReturn) noexcept :
                app( who ),
                tag( uid ),
                msg( Lexical::LX_CNTL | (emitLexeme ? Lexical::LX_EMIT : Lexical::LX_DROP) ),
                fcn( willReturn ? &App::call : &App::jump)
                {
                }

                inline Jump(const Jump &j) noexcept :
                app( j.app ),
                tag( j.tag ),
                msg( j.msg ),
                fcn( j.fcn )
                {
                }


                inline Lexical::Message operator()(const Token &)
                {
                    (app.*fcn)(*tag);
                    return msg;
                }

                inline ~Jump() noexcept {}


                App &                  app;
                const Tag              tag;
                const Lexical::Message msg;
                const Branch           fcn;

            private:
                Y_DISABLE_ASSIGN(Jump);
            };

#if 0
            inline void makeJump(const Tag &expr,
                                 const Tag &uuid,
                                 const bool emit)
            {
                const Motif              motif( RegExp::Compile(*expr, genesis.dict) );
                const Jump               coded(*this,uuid,emit,false);
                const Lexical::Callback  doing = coded;
                const String             named = "jump@" + *uuid;
                Lexical::Action::Pointer which( new Lexical::Action(named,motif,doing)   );
                genesis.submitCode(which);
            }

            inline void makeCall(const Tag &expr,
                                 const Tag &uuid,
                                 const bool emit)
            {
                const Motif              motif( RegExp::Compile(*expr, genesis.dict) );
                const Jump               coded(*this,uuid,emit,true);
                const Lexical::Callback  doing = coded;
                const String             named = "call@" + *uuid;
                Lexical::Action::Pointer which( new Lexical::Action(named,motif,doing)   );
                genesis.submitCode(which);
            }
#endif

        };

        Lexer::App * Lexer::Create(Scanner &self)
        {
            return new App(self);
        }


    }

}
namespace Yttrium
{
    namespace Jive
    {


        Lexer:: ~Lexer() noexcept
        {
            assert(0!=app);
            Nullify(app);
        }
        
        void Lexer:: restart() noexcept
        {
            assert(app);
            app->restart();
        }

        void Lexer:: put(Lexeme *lxm) noexcept
        {
            assert(0!=lxm);
            assert(0!=app);
            app->lexemes.pushHead(lxm);
        }

        Lexeme * Lexer:: get(Source &source)
        {
            assert(0!=app);
            return app->get(source);
        }

        void Lexer:: submit(const Scanner::Pointer &ptr)
        {
            assert(0!=app);
            app->submit(ptr);
        }
    }

}

