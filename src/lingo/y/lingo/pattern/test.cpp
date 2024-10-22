#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Lingo
    {

        
        bool operator==(const Pattern &lhs, const Pattern &rhs)
        {
            return Pattern::Equality(lhs,rhs);
        }

        bool Pattern:: Equality(const Pattern &lhs, const Pattern &rhs)
        {
            const uint32_t uuid = lhs.uuid; if(uuid!=rhs.uuid) return false;

            assert(lhs.uuid==rhs.uuid);
            switch(uuid)
            {
                    // basic
                case Any1:: UUID: return true;
                case Void:: UUID: return true;
                case Byte:: UUID: return lhs.as<Byte>()->byte   == rhs.as<Byte>()->byte;
                case Excl:: UUID: return lhs.as<Excl>()->byte   == rhs.as<Excl>()->byte;
                case Lump:: UUID: return lhs.as<Lump>()->code() == rhs.as<Lump>()->code();

                    // joker
                case Optional:: UUID: return Equality(**(lhs.as<Optional>()), **(lhs.as<Optional>()) );
                case Repeated:: UUID: {
                    const Repeated &l = *lhs.as<Repeated>();
                    const Repeated &r = *rhs.as<Repeated>();
                    return (l.minimalCount==r.minimalCount && Equality(*l,*r));
                }
                case Counting:: UUID: {
                    const Counting &l = *lhs.as<Counting>();
                    const Counting &r = *rhs.as<Counting>();
                    return (l.minimalCount==r.minimalCount && l.maximalCount==r.maximalCount && Equality(*l,*r));
                }

                    // logic
                case And::  UUID: return Logic::HaveSameContent( *lhs.as<And>(),  *rhs.as<And>()  );
                case Or::   UUID: return Logic::HaveSameContent( *lhs.as<Or>(),   *rhs.as<Or>()   );
                case None:: UUID: return Logic::HaveSameContent( *lhs.as<None>(), *rhs.as<None>() );
            }
            throw Specific::Exception("Pattern::Equality", "unknown UUID='%s'", FourCC::ToText(uuid));
        }

    }

}
