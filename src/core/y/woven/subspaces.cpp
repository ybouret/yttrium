
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
                for(Subspace *target=temp.head;target;target=target->next)
                {
                    if(target->merged(source))
                    {
                        assert(source.isEmpty());
                        goto CYCLE;
                    }
                }

                assert(source.isValid());
                temp.pushTail( source.yield() );
            CYCLE:
                continue;
            }
            swapWith(temp);

        }


        void Subspaces:: conductInitial(QSurvey &survey) const
        {
            for(const Subspace *sub = head; sub; sub=sub->next)
            {
                assert(sub->qfamily.size>0);
                survey(*(sub->qfamily.tail));
            }
        }

    }

}
