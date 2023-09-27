
#include "y/jive/lexer.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/nullify.hpp"
#include "y/utest/run.hpp"
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
            typedef Lexical::Scanner              Scanner;
            typedef ArkPtr<String,Scanner>        ScanPtr;
            typedef SuffixSet<String,ScanPtr>     ScanDB;
            typedef ScanDB::Iterator              Iterator;
            typedef Small::SoloLightList<Scanner> History;

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

                Y_SIZEOF(Lexemes);
                Y_SIZEOF(ScanDB);
                Y_SIZEOF(History);
                const ScanPtr ptr( &genesis );
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

            //! restart all
            inline void restart() noexcept
            {
                scanner = &genesis;
                history.free();      assert(0==history.size);
                lexemes.release();   assert(0==lexemes.size);
                for(Iterator it=content.begin();it!=content.end();++it)
                    (**it).cleanup();
            }

            inline Lexeme * get(Source &source)
            {
                if(lexemes.size)
                {
                    // use cache
                    return lexemes.popHead();
                }
                else
                {

                    return 0;
                }

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
            ScanDB               content; //!< scanner database
            History              history; //!< call stack
            const String        &name;    //!< genesis name
            XMLog               &xml;     //!< for verbose output

        private:
            Y_DISABLE_COPY_AND_ASSIGN(App);
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
            Y_SIZEOF(App);
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
    }

}

