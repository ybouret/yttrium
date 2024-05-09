
#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Actors:: Actors() noexcept : CxxListOf<Actor>() {}
        Actors:: ~Actors() noexcept {}
        Actors:: Actors(const Actors &A) : CxxListOf<Actor>(A) {}


        String Actors:: toString() const
        {
            String res;
            const Actor *a  = head;
            if(a)
            {
                res += a->asFirst();
                while( 0 != (a=a->next) )
                {
                    res += a->asExtra();
                }
            }
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Actors &A)
        {
            return (os << A.toString());
        }

        int Actors:: charge() const noexcept
        {
            int res = 0;
            for(const Actor *a=head;a;a=a->next) res += a->charge();
            return res;
        }

        bool Actors:: contains(const Species &sp) const noexcept
        {
            for(const Actor *a=head;a;a=a->next)
            {
                if( &(a->sp) == &sp) return true;
            }
            return false;
        }

        size_t Actors:: maxIndex(const Level level) const noexcept
        {
            size_t res = 0;
            for(const Actor *a=head;a;a=a->next) res = Max(res,a->sp.indx[level]);
            return res;
        }

        bool Actors:: contains(const Actor &other) const noexcept
        {
            const Species &sp = other.sp;
            const unsigned nu = other.nu;
            for(const Actor *a=head;a;a=a->next)
            {
                if ( ( &(a->sp) == &sp) && (a->nu==nu) ) return true;
            }
            return false;
        }

        bool Actors:: checked() const noexcept
        {
            for(const Actor *a=head;a;a=a->next)
            {
                if(a->nu<=0) return false;
                for(const Actor *b=a->next;b;b=b->next)
                {
                    if( &(a->sp) == &(b->sp) ) return false;
                }
            }
            return true;
        }

        bool  operator==(const Actors &lhs, const Actors &rhs) noexcept
        {
            assert(lhs.checked());
            assert(rhs.checked());

            if(lhs.size!=rhs.size) return false;

            const Actor *a = lhs.head;
            const Actor *b = rhs.head;
            for(size_t i=lhs.size;i>0;--i,a=a->next,b=b->next)
            {
                assert(0!=a);
                assert(0!=b);
                if(!rhs.contains(*a)) return false;
                if(!lhs.contains(*b)) return false;
            }


            return true;
        }

    }

}
