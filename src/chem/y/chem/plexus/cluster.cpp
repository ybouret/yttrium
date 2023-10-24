
#include "y/chem/plexus/cluster.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: ~Cluster() noexcept
        {
        }

        Cluster:: Cluster(const Equilibrium &first) :
        Object(),
        all(),
        lib(),
        last(0),
        edb(0),
        sdb(0),
        cll(),
        law(),
        tier( new Tier()     ),
        kept( 0 ),
        army( new Army()     ),
        sGrp( new EqGroups() ),
        eGrp( new EqGroups() ),
        Nu(),
        Qm(),
        meb(0),
        next(0),
        prev(0),
        eqfm()
        {
            enroll(first);
        }

        void Cluster:: enroll(const Equilibrium &eq)
        {
            assert(!all.has(eq));
            Coerce(all) << eq;
            for(Equilibrium::ConstIterator it = eq->begin(); it != eq->end(); ++it)
            {
                const Species &sp = (*it).sp;
                if(!lib.has(sp)) 
                {
                    Coerce(lib) << sp;
                }
            }
        }

        bool Cluster:: tiedTo(const Equilibrium &eq) const noexcept
        {
            for(const EqNode *node=all.head;node;node=node->next)
            {
                const Equilibrium &mine = **node;
                if(mine.linkedTo(eq))
                    return true;
                
            }
            return false;
        }

        bool Cluster:: tiedTo(const Cluster &other) const noexcept
        {

            for(const EqNode *node=other.all.head;node;node=node->next)
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
        void Cluster:: compile(Equilibria            &Eqs,
                               const Readable<xreal> &Ks,
                               XMLog                 &xml)
        {
            Y_XML_SECTION_OPT(xml,"Cluster"," size='" << all.size << "'");
            assert(all.size>0);
            assert(lib.size>0);
            Y_XMLOG(xml, "all: " << all);
            Y_XMLOG(xml, "lib: " << lib);

            const size_t N  = all.size;
            const size_t M  = lib.size;
            EqArray     *E = new EqArray(N); Coerce(edb) = E;
            SpArray     *S = new SpArray(M); Coerce(sdb) = S;

            //------------------------------------------------------------------
            // start indexing
            //------------------------------------------------------------------
            {
                size_t i=0;
                for(EqNode *node=all.head;node;node=node->next)
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
                for(EqNode *node=all.head;node;node=node->next)
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
            connectAll(xml,Eqs,Ks);

            //------------------------------------------------------------------
            // update status
            //------------------------------------------------------------------
            Coerce(last) = ListOps::Next(all.head,N);

        }

        void Cluster:: updateK(Writable<xreal> &K, const double t)
        {
            for(EqNode *node=all.head;node;node=node->next)
            {
                Equilibrium &eq = Coerce(**node);
                K[eq.indx[TopLevel]] = eq.K(t);
            }
        }


        void  Cluster:: viz(OutputStream &fp, const size_t order) const
        {
            assert(order<=meb->size());
            const EqRepo * const repo = & ((*meb)[order]);
            AddressBook          book;

            //------------------------------------------------------------------
            //
            // fill species
            //
            //------------------------------------------------------------------
            for(const EqNode *node=repo->head;node;node=node->next)
            {
                const Equilibrium &eq = **node;
                eq.insertSpeciesIn(book);
            }

            //------------------------------------------------------------------
            //
            // write all species
            //
            //------------------------------------------------------------------
            for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
            {
                const Species &sp = *static_cast<const Species *>(*it);
                sp.viz(fp);
            }

            //------------------------------------------------------------------
            //
            // write all eqs
            //
            //------------------------------------------------------------------
            for(EqNode *node=repo->head;node;node=node->next)
            {
                (**node).viz(fp);
            }
            
            if(1==order)
            {
                size_t indx=1;
                for(const Canon *canon=law.head;canon;canon=canon->next)
                {
                    canon->viz(fp,indx);
                }
            }
        }

    }

}

