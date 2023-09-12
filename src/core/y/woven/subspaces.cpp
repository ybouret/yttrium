
#include "y/woven/subspaces.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        Subspaces :: ~Subspaces() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Subspaces &subs)
        {
            os << '{' << std::endl;
            for(const Subspace *s=subs.head;s;s=s->next)
            {
                os << "  " << *s << std::endl;
            }
            os << '}';
            return os;
        }

        void Subspaces:: pack()
        {
            Subspace::List temp;

            while(size>0)
            {

                AutoPtr<Subspace> source = popHead();
                bool              packed = false;
                for(Subspace *target=temp.head;target;target=target->next)
                {
                    if(target->indices == source->indices)
                    {
                        assert(target->staying == source->staying);
                        packed = true;
                        break;
                    }

                    if(target->qfamily.contains(source->qfamily))
                    {
                        target->indices |= source->indices;
                        target->staying |= source->staying;
                        target->staying ^= target->indices;
                        packed = true;
                        break;
                    }
                }

                if(!packed)
                    temp.pushTail( source.yield() );
            }

            swapWith(temp);
        }

    }

}
