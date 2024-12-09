#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"

#include "y/chemical/reactive/cluster/mixed.hpp"
#include "y/woven/subspaces.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: classifySpecies(XMLog &xml)
        {

            Y_XML_SECTION(xml,"SpeciesClassification");

            // extracting conserved from laws
            {
                AddressBook &abiding = Coerce(conserved.book);
                for(const Conservation::Law *ln=laws->head;ln;ln=ln->next)
                {
                    for(const Actor *a = (*ln)->head;a;a=a->next)
                        abiding |= a->sp;
                }
            }
            Coerce(conserved).compile();
            Y_XMLOG(xml,"conserved=" << conserved.list);

            // excluding undounded from conserved
            {
                AddressBook &rebels = Coerce(unbounded.book);
                for(const SNode *sn=my.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if(conserved.book.has(sp)) continue;
                    rebels += sp;
                }
            }
            Coerce(unbounded).compile();
            Y_XMLOG(xml,"unbounded=" << unbounded.list);
        }

        namespace
        {
            typedef Vector<int,MemoryModel> iVector;

            class iStoi : public Object, public iVector
            {
            public:
                typedef CxxListOf<iStoi> List;

                inline explicit iStoi(const Readable<int> &arr) :
                Object(), iVector(arr.size(),0), next(0), prev(0)
                {
                    for(size_t i=size();i>0;--i) (*this)[i]  = arr[i];
                }

                inline virtual ~iStoi() noexcept {}

                iStoi *next, *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(iStoi);
            };

            class iStoiList : public iStoi::List
            {
            public:
                inline explicit iStoiList(const Matrix<int> &topo) :
                iStoi::List(), dims(topo.cols)
                {
                    for(size_t i=1;i<=topo.rows;++i) {
                        if( !push(topo[i]) )
                            throw Specific::Exception(Grouping::CallSign,"unexpected multiple stoichiometry");
                    }
                }

                inline virtual ~iStoiList() noexcept {}

                bool push(const Readable<int> &rhs)
                {
                    assert( dims == rhs.size() );
                    for(const iStoi *stoi=head;stoi;stoi=stoi->next) {
                        const Readable<int> &lhs = *stoi;
                        if( lhs == rhs ) return false;
                    }

                    pushTail( new iStoi(rhs) );
                    return true;
                }



                const size_t dims;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(iStoiList);

            };

        };

        void Cluster:: compile(Equilibria &eqs, XMLog &xml)
        {
            const EList &el = my;
            const SList &sl = my.species;
            Y_XML_SECTION_OPT(xml,Grouping::CallSign, el << '/' << sl);
            assert(laws.isEmpty());

            //------------------------------------------------------------------
            //
            //
            // create laws
            //
            //
            //------------------------------------------------------------------
            Coerce(laws) = new Conservation::Laws(*this,xml);

            //------------------------------------------------------------------
            //
            //
            // classify species accordingly
            //
            //
            //------------------------------------------------------------------
            classifySpecies(xml);


            //------------------------------------------------------------------
            //
            //
            // create lists of equilibria and register primary
            //
            //
            //------------------------------------------------------------------
            ELists &elists = Coerce(* ( Coerce(order) = new ELists(my.iTopology.rows) ));
            for(ENode *en=my.head;en;en=en->next)
            {
                elists[1] << **en;
            }

            //------------------------------------------------------------------
            //
            //
            // create combinatorics
            //
            //
            //------------------------------------------------------------------
            WOVEn::IntegerSurvey survey(xml);
            {
                Y_XML_SECTION(xml, "WOVEn::Explore");
                const Matrix<int>    mu(TransposeOf,my.iTopology);
                WOVEn::Explore(mu,survey,false);
                survey.sort();
            }

            //------------------------------------------------------------------
            //
            //
            // scan combinatorics
            //
            //
            //------------------------------------------------------------------
            iStoiList               existing(my.iTopology);
            const size_t            nspc = sl.size;
            const apz               zero = 0;
            Vector<apz,MemoryModel> stoi(nspc,0);
            Vector<int,MemoryModel> coef(nspc,0);
            AddressBook             original;
            AddressBook             combined;
            for(const WOVEn::IntegerArray *warr=survey.head;warr;warr=warr->next)
            {
                const WOVEn::IntegerArray &comb = *warr;

                //--------------------------------------------------------------
                //
                // initialize state
                //
                //--------------------------------------------------------------
                stoi.ld(zero);
                coef.ld(0);
                original.free();
                combined.free();

                for(const ENode *en=my.head;en;en=en->next)
                {
                    //----------------------------------------------------------
                    // get coefficient
                    //----------------------------------------------------------
                    const Equilibrium &eq = **en;
                    const Indexed     &id = eq;
                    const apz         &cf = id(comb,SubLevel);

                    if(zero==cf)
                        continue;

                    //----------------------------------------------------------
                    // use equilibrium
                    //----------------------------------------------------------
                    eq.addSpeciesTo(original);
                    for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                    {
                        const Component &cm = *it;
                        const Species   &sp = cm.actor.sp;
                        const apz        nu = cm.actor.nu; assert(nu>0);
                        switch(cm.role)
                        {
                            case Product:  sp(stoi,SubLevel) += nu * cf; break;
                            case Reactant: sp(stoi,SubLevel) -= nu * cf; break;
                        }
                    }
                }


                //--------------------------------------------------------------
                //
                // fetch new set of species
                //
                //--------------------------------------------------------------
                for(const SNode *sn=sl.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    const apz     &cf = sp(stoi,SubLevel);
                    if(zero!=cf)
                    {
                        combined += sp; // must appear only once
                        sp(coef,SubLevel) = cf.cast<int>("stoichiometry");
                    }
                }

                if( !existing.push(coef) )
                {
                    Y_XML_COMMENT(xml, "duplicate");
                }


                SList oldSpecies; original.sendTo(oldSpecies);
                SList newSpecies; combined.sendTo(newSpecies);
                SList evaporated(oldSpecies);

                DBOps::Revamp<SList>::Sort(oldSpecies);
                DBOps::Revamp<SList>::Sort(newSpecies);


                switch( Sign::Of(newSpecies.size,oldSpecies.size) )
                {
                    case __Zero__:
                        if(  !oldSpecies.alike(newSpecies) )
                            throw Specific::Exception(Grouping::CallSign, "corrupted same size combinatorics!");
                        //Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / useless");
                        continue;

                    case Positive:
                        //Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / corrupted!");
                        throw Specific::Exception(Grouping::CallSign, "expanding combinatorics!");

                    case Negative:
                        evaporated.subtract(newSpecies);
                        //Y_XMLOG(xml, comb << " : " << oldSpecies << " => " << newSpecies << " / (-) " << evaporated);
                        break;
                }


                //--------------------------------------------------------------
                //
                // build mixed equilibrium
                //
                //--------------------------------------------------------------
                const String eqName = MixedEquilibrium::MakeName(my,comb);
                Y_XMLOG(xml, "(+) " << eqName << ", order=" << comb.order << ", removing " << evaporated);
                

            }





        }
    }

}

