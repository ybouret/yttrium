
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
            while(true)
            {
                bool           done = true;
                Subspace::List temp;
                while(size>0)
                {

                    AutoPtr<Subspace> source = popHead();
                    for(Subspace *target=temp.head;target;target=target->next)
                    {
                        if(target->merged(source))
                        {
                            assert(source.isEmpty());
                            done = false;
                            goto CYCLE;
                        }
                    }
                    assert(source.isValid());
                    temp.pushTail( source.yield() );
                CYCLE:
                    continue;
                }
                swapWith(temp);
                if(done) break;
            }
        }

    }

}
