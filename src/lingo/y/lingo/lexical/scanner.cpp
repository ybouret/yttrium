
#include "y/lingo/lexical/scanner.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            void Scanner:: initialize() noexcept
            {
                for(unsigned i=0;i<CHARS;++i) new ( &rlist[i] ) RList();
            }


            Scanner:: ~Scanner() noexcept
            {
                unsigned i=CHARS;
                while(i-- > 0)
                    Destruct( &rlist[i] );
            }

            Scanner::ConstInterface &   Scanner:: surrogate() const noexcept
            {
                return rules;
            }

            void Scanner:: operator()(Rule * const rule)
            {
                assert(0!=rule);
                AutoPtr<Rule> ptr(rule);
                CharDB        cdb;
                Rule &        ref = *rule;
                rule->motif->query(cdb);
                std::cerr << "firstChars of " << rule->name << " = '" << cdb << "'" << std::endl;
                try {

                    // register
                    for(unsigned i=0;i<CHARS;++i)
                        if( cdb.has(i) )
                            rlist[i] << ref;

                    // and finally append
                    rules.pushTail( ptr.yield() );
                }
                catch(...)
                {
                    for(unsigned i=0;i<CHARS;++i)
                        if( cdb.has(i) )
                            (void) rlist[i].no(ref);
                    throw;
                }
            }

        }

    }

}

