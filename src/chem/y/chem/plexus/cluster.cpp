
#include "y/chem/plexus/cluster.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Tier::  Tier() noexcept :regular(),  roaming() {}
        Tier:: ~Tier() noexcept {}
    }

}
namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: ~Cluster() noexcept
        {
        }

        Cluster:: Cluster(const Equilibrium &first) :
        Object(),
        eqs(),
        lib(),
        edb(0),
        sdb(0),
        cll(),
        law(),
        tier(),
        Nu(),
        Qm(),
        next(0),
        prev(0)
        {
            enroll(first);
        }

        void Cluster:: enroll(const Equilibrium &eq)
        {
            assert(!eqs.has(eq));
            Coerce(eqs) << eq;
            for(Equilibrium::ConstIterator it = eq->begin(); it != eq->end(); ++it)
            {
                const Species &sp = (*it).sp;
                if(!lib.has(sp)) Coerce(lib) << sp;
            }
        }

        bool Cluster:: tiedTo(const Equilibrium &eq) const noexcept
        {
            for(const EqNode *node=eqs.head;node;node=node->next)
            {
                const Equilibrium &mine = **node;
                if(mine.linkedTo(eq))
                    return true;
                
            }
            return false;
        }

        bool Cluster:: tiedTo(const Cluster &other) const noexcept
        {

            for(const EqNode *node=other.eqs.head;node;node=node->next)
            {
                const Equilibrium &otherEq = **node;
                if(tiedTo(otherEq))
                    return true;
            }
            return false;
        }

    }

}

#include "y/chem/algebraic.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: compile(Equilibria            &all,
                               const Readable<xreal> &Ks,
                               XMLog                 &xml)
        {
            Y_XML_SECTION_OPT(xml,"Cluster"," size='" << eqs.size << "'");
            assert(eqs.size>0);
            assert(lib.size>0);
            Y_XMLOG(xml, "eqs: " << eqs);
            Y_XMLOG(xml, "lib: " << lib);

            const size_t N  = eqs.size;
            const size_t M  = lib.size;
            EqArray     *E = new EqArray(N); Coerce(edb) = E;
            SpArray     *S = new SpArray(M); Coerce(sdb) = S;

            //------------------------------------------------------------------
            // indexing
            //------------------------------------------------------------------
            {
                size_t i=0;
                for(EqNode *node=eqs.head;node;node=node->next)
                {
                    const Equilibrium &eq = **node;
                    Coerce(eq.indx[SubLevel]) = ++i;
                    Coerce((*E)[i]) = &Coerce(eq);
                }
            }

            {
                size_t j=0;
                for(SpNode *node=lib.head;node;node=node->next)
                {
                    const Species &sp = **node;
                    Coerce(sp.indx[SubLevel]) = ++j;
                    Coerce((*S)[j]) = &Coerce(sp);
                }
            }

            //------------------------------------------------------------------
            // main topology
            //------------------------------------------------------------------
            {
                Matrix<int> &nu = Coerce(Nu);
                nu.make(N,M);
                for(EqNode *node=eqs.head;node;node=node->next)
                {
                    const Equilibrium &eq = **node;
                    eq.fill(nu[ eq.indx[SubLevel]], SubLevel);
                }
            }
            Y_XMLOG(xml,"Nu = "<<Nu);

            //------------------------------------------------------------------
            // conservations
            //------------------------------------------------------------------
            Chemical::Algebraic::Compute(Coerce(Qm),Nu,xml);
            Y_XMLOG(xml,"Nu = "<<Nu);
            Y_XMLOG(xml,"Qm = "<<Qm);
            createCons(xml);

            //------------------------------------------------------------------
            // connection
            //------------------------------------------------------------------
            connectAll(xml,all,Ks);
        }


    }

}

