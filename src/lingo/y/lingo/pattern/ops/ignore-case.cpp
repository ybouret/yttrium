
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


#define Y_Lingo_IgnoreCase(TYPE) case TYPE:: UUID: motif->as<TYPE>()->ignoreCase(); break

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
                    return IgnoreCaseOfBasic(motif);

                    Y_Lingo_IgnoreCase(Optional);
                    Y_Lingo_IgnoreCase(Repeated);
                    Y_Lingo_IgnoreCase(Counting);

                    Y_Lingo_IgnoreCase(And);
                    Y_Lingo_IgnoreCase(Or);
                    Y_Lingo_IgnoreCase(None);
            }


            return motif.yield();
        }
    }

}
