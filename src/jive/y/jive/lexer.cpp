
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

            //! cleanup
            inline virtual ~App() noexcept
            {
            }

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
            inline void set() noexcept
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
                Y_XML_SECTION_OPT(xml,name, " : get next lexeme");

                //--------------------------------------------------------------
                //
                // (re) probing after a drop or a control
                //
                //--------------------------------------------------------------
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
                            //==================================================
                            //
                            // process success
                            //
                            //==================================================
                            assert(0!=action);
                            const Lexical::Message msg = action->perform();

                            //--------------------------------------------------
                            //
                            // always check for newline
                            //
                            //--------------------------------------------------
                            if( 0!= (msg & Lexical::LX_ENDL) )
                            {
                                source.newLine();
                            }

                            //--------------------------------------------------
                            //
                            // then process output or control
                            //
                            //--------------------------------------------------
                            if( 0 != (msg & Lexical::LX_CNTL) )
                            {
                                assert(0==(msg&(Lexical::LX_EMIT|Lexical::LX_DROP)));
                                action->motif->release();
                                goto PROBE;
                            }
                            else
                            {

                                // check drop
                                if( 0 != (msg & Lexical::LX_DROP) )
                                {
                                    checkNoFlags(Lexical::LX_EMIT,msg);
                                    action->motif->release();
                                    goto PROBE;
                                }

                                // check emit
                                if( 0 != (msg & Lexical::LX_EMIT) )
                                {
                                    return action->produce();
                                }

                                // shouldn't happen
                                throw Specific::Exception( name.c_str(),"invalid message 0x%x", msg);
                            }
                        }

                        case Lexical::ReturnFailure: {
                            //==================================================
                            //
                            // process failure
                            //
                            //==================================================
                            Token bad;
                            if(source.guess(bad))
                            {
                                assert(bad.size>0);
                                assert(0!=bad.head);
                                const String        info = bad.toPrintable();
                                Specific::Exception excp( scanner->name->c_str(), "syntax error '%s'", info.c_str());
                                bad.head->stamp(excp);
                                throw excp;
                            }
                            else
                            {
                                Specific::Exception excp( scanner->name->c_str(), "forbidden trailing blanks");
                                source->stamp(excp);
                                throw excp;
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
                    assert(0!=scanner);
                    if( &genesis != scanner )
                    {
                        switch(scanner->_eos)
                        {
                            case Lexical::AcceptEndOfStream:
                                break;
                            case Lexical::RejectEndOfStream:
                                throw Specific::Exception(name.c_str(), "unfinished <%s>", scanner->name->c_str());
                        }
                    }
                    return 0;
                }

            }


            inline void jump(const String &id)
            {
                assert(0!=scanner);
                Scanner &target = get(id,"jump to");
                Y_XMLOG(xml, "[jump] from <" << scanner->name << "> to <" << target.name << ">");
                scanner = & target;
            }

            inline void call(const String &id)
            {
                assert(0!=scanner);
                Scanner &target = get(id,"call");
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

            inline void checkNoFlags(const Lexical::Message flag, const Lexical::Message msg) const
            {
                if( 0 != (msg&flag) ) throw Specific::Exception( name.c_str(), "invalid lexical message");
            }

            inline Scanner & get(const String &id, const char *text)
            {
                assert(0!=text);
                Scanner::Pointer *pp = content.search(id);
                if(!pp) throw Specific::Exception(name.c_str(),"No <%s> to %s",id(),text);
                return **pp;
            }

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
        
        void Lexer:: set() noexcept
        {
            assert(app);
            app->set();
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

        void Lexer:: submit(Scanner *ptr)
        {
            assert(0!=ptr);
            const Scanner::Pointer _(ptr);
            submit(_);
        }


        void Lexer:: jump_(const String &id)
        {
            assert(0!=app);
            app->jump(id);
        }

        void Lexer:: call_(const String &id)
        {
            assert(0!=app);
            app->call(id);
        }

        void Lexer:: back_()
        {
            assert(0!=app);
            app->back();
        }

        bool Lexer:: owns(const Scanner &scanner) const noexcept
        {
            assert(0!=app);
            const Scanner::Pointer *sptr = app->content.search(*scanner.name);
            if(!sptr)                return false;
            if(& **sptr == &scanner) return true;
            return false;
        }

        Lexical::Scanner *Lexer:: query(const String &id) noexcept
        {
            assert(0!=app);
            Scanner::Pointer *sptr = app->content.search(id);
            if(sptr)
            {
                return & **sptr;
            }
            else
            {
                return 0;
            }
        }


    }

}

