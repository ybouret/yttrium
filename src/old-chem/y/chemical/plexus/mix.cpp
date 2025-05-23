
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Mix::ConstInterface & Mix:: surrogate() const noexcept { return my; }

        Mix:: ~Mix() noexcept
        {
        }


        const char * const Mix:: CallSign = "Chemical::Mix";

        Mix:: Mix(const Equilibrium &first):
        Proxy<const Connected>(),
        my(first,0),
        conservancy(),
        auth(0),
        order(0),
        genus(0),
        grade(0),
        topology(),
        keeper(0),
        solver(0),
        next(0),
        prev(0)
        {
        }

        bool Mix:: accepts(const Equilibrium &eq) const noexcept
        {
            return my.accepts(eq);
        }

        bool Mix:: accepts(const Mix &mix) const noexcept
        {
            for(const ENode *en=mix->head;en;en=en->next)
            {
                if( accepts( **en ) ) return true;
            }
            return false;
        }

        void Mix:: addPrimary(const Equilibrium &eq)
        {
            my.connect(eq);
        }


        void Mix:: addPrimary(const Mix &mix)
        {
            for(const ENode *en=mix->head;en;en=en->next)
            {
                my.connect(**en);
            }
        }

       



        std::ostream & operator<<(std::ostream &os, const Mix &mix)
        {
            static const char pfx[] = "\t\t";
            assert(mix->size>0);
            os << "\t{" << std::endl;
            os << pfx << "species=" << mix->species << std::endl;
            for(const ENode *en=mix->head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                mix->print(os << pfx << "@",eq,eq.K(0)) << std::endl;
            }
            os << pfx << "topology=" << mix->topology << std::endl;
            size_t total = 0;
            for(size_t i=1;i<=mix.order.size();++i)
            {
                const EList &elist = mix.order[i];
                const size_t count = elist.size;
                if(count<=0) continue;
                const String o = Formatted::Get("%u",unsigned(i));
                os << pfx << "#order[" << Justify(o,3,Justify::Center) << "] = " << std::setw(8) << count << std::endl;
                total += count;
            }
            assert(total==mix->size);
            os << pfx << "#total      = " << std::setw(8) << total << "  (=" << mix.order[1].size << "+" << total-mix.order[1].size << ")" << std::endl;

            const size_t nAct = mix.auth->acts->size;
            if(nAct>0)
            {
                os << pfx << "#law=" << mix.auth->laws->size << " => #act=" << nAct << std::endl;
                for(const Conservation::Act *act=mix.auth->acts->head;act;act=act->next)
                {
                    os << pfx << "  0=" << *act << std::endl;
                }
            }
            if(mix.genus->conserved.list.size>0) os << pfx << "conserved = " << mix.genus->conserved.list << std::endl;
            if(mix.genus->unbounded.list.size>0) os << pfx << "unbounded = " << mix.genus->unbounded.list << std::endl;

            if(mix.grade->reacOnly.list.size>0)  os << pfx << "reacOnly  = " << mix.grade->reacOnly.list << std::endl;
            if(mix.grade->prodOnly.list.size>0)  os << pfx << "prodOnly  = " << mix.grade->prodOnly.list << std::endl;
            if(mix.grade->oneSided.list.size>0)  os << pfx << "oneSided  = " << mix.grade->oneSided.list << std::endl;
            if(mix.grade->limiting.list.size>0)  os << pfx << "limiting  = " << mix.grade->limiting.list << std::endl;
            if(mix.grade->floating.list.size>0)  os << pfx << "floating  = " << mix.grade->floating.list << std::endl;



            os << "\t}";
            return os;
        }


        void Mix:: solve(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {
            solver->run(xml,C,L,K);
        }


        void Mix:: guard(XMLog &xml, XWritable &C, const Level L, XWritable &injected)
        {
            keeper->run(xml,C,L,injected);
        }


    }

}
