
#include "y/jive/lexer.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/nullify.hpp"
#include "y/utest/run.hpp"
#include "y/system/exception.hpp"
#include "y/type/static-retainer.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Jive
    {


        class Lexer:: App : public Object
        {
        public:
            typedef Lexical::Scanner              Scanner;
            typedef ArkPtr<String,Scanner>        ScanPtr;
            typedef SuffixSet<String,ScanPtr>     ScanDB;
            typedef Small::SoloLightList<Scanner> History;


            inline explicit App(Scanner &self) :
            Object(),
            genesis(  self    ),
            scanner( &genesis ),
            lexemes(          ),
            retains( genesis  ),
            content(),
            history(),
            name( *self.name )
            {

                Y_SIZEOF(Lexemes);
                Y_SIZEOF(ScanDB);
                Y_SIZEOF(History);
                const ScanPtr ptr( &genesis );
                if(!content.insert(ptr))
                    throw Specific::Exception(name.c_str(),"Initialization Failure!");

            }

            inline virtual ~App() noexcept {}


            Scanner &            genesis;
            Scanner *            scanner; //!< current scanner
            Lexemes              lexemes; //!< lexemes cache
            const StaticRetainer retains; //!< helper
            ScanDB               content; //!< scanner database
            History              history; //!< call stack
            const String        &name;    //!< genesis name

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
        

    }

}

