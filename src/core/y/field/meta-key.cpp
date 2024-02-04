
#include "y/field/meta-key.hpp"

namespace Yttrium
{
    namespace Field
    {

        MetaKey:: MetaKey(void * const data) noexcept :
        built(0),
        array( static_cast<SharedString *>(data) )
        {
        }

        MetaKey:: ~MetaKey() noexcept
        {
            while(built>0)
                Destruct( & array[--built] );
        }



        void   MetaKey:: push(const SharedString &uuid) noexcept
        {
            assert( size() < maxi() );
            new ( & array[built++] ) SharedString(uuid);
        }

        void   MetaKey:: push(const String &s) noexcept
        {
            assert(s.quantity()>0);
            const SharedString uuid( & Coerce(s) );
            push(uuid);
        }

        unsigned MetaKey:: size() const noexcept { return built; }

        std::ostream & operator<<(std::ostream &os, const MetaKey &mk)
        {
            String ans;
            for(unsigned i=0;i<mk.built;++i) ans += mk[i];
            os << ans;
            return os;
        }

        const String & MetaKey:: operator[](const unsigned dim) const noexcept
        {
            assert(dim<built);
            return *array[dim];
        }

        bool operator==(const MetaKey &lhs, const MetaKey &rhs) noexcept
        {
            const unsigned n = lhs.size();
            if(rhs.size()!=n) return false;

            for(unsigned i=0;i<n;++i)
            {
                if( lhs[i] != rhs[i] ) return false;
            }

            return true;
        }

    }

}
