
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        static inline void Probe(CharDB    &db,
                                 const char ini,
                                 const char end,
                                 const int  off) noexcept
        {

            for(char c=ini;c<=end;++c)
                if(db.has(c))
                    db.set( int(c)+off );

        }

        static inline
        Pattern * IgnoreCaseOfBasic(const AutoPtr<Pattern> &motif)
        {
            assert(motif->isBasic());
            CharDB db;
            motif->query(db);
            Probe(db,'a', 'z', int('A') - int('a') );
            Probe(db,'A', 'Z', int('a') - int('A') );
            return db.compile();
        }



        Pattern * Pattern:: IgnoreCase(Pattern * const p)
        {
            assert(0!=p);

            AutoPtr<Pattern> motif = p;
            switch(motif->uuid)
            {
                case Any1:: UUID:
                case Void:: UUID:
                case Byte:: UUID:
                case Lump:: UUID:
                case Excl:: UUID:
                    return  IgnoreCaseOfBasic(motif);

                case Optional:: UUID: motif->as<Optional>()->ignoreCase(); break;
                case MoreThan:: UUID: motif->as<MoreThan>()->ignoreCase(); break;
                case Counting:: UUID: motif->as<Counting>()->ignoreCase(); break;

                case And :: UUID: motif->as<And >()->ignoreCase(); break;
                case Or  :: UUID: motif->as< Or >()->ignoreCase(); break;
                case None:: UUID: motif->as<None>()->ignoreCase(); break;
            }


            return  motif.yield();
        }
    }

}
