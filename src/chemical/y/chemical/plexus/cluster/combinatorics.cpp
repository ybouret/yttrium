#include "y/chemical/plexus/cluster/mixtab.hpp"
#include "y/chemical/plexus/cluster/combinatorics.hpp"
#include "y/chemical/reactive/equilibrium/mixed.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/apex/api/count-non-zero.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
      
       
        static inline void MakeELists(const size_t maxOrder,
                                      ClusterCombinatorics::ELists &slot,
                                      const SubEList &equilibria )
        {
            assert(maxOrder>0);

            {
                ClusterCombinatorics::ELists _(maxOrder);
                _.swapWith(slot);
            }

            EList &primary = slot[1];
            for(ENode *en=equilibria->head;en;en=en->next)
            {
                primary << **en;
            }

        }

        void ClusterCombinatorics:: createCombinations(XMLog &xml, Equilibria &eqs, XWritable &tlK)
        {
            //------------------------------------------------------------------
            //
            //
            // prepare variables
            //
            //
            //------------------------------------------------------------------
            CxxListOf<MixTab> mixes;
            ClusterContent &  content    = Coerce(**this);
            SubEList       &  equilibria = content.equilibria;
            const SList    &  species    = *content.species;
            size_t            maxOrder   = 1;
            ELists         &  slot = Coerce(order);
            {
                //--------------------------------------------------------------
                //
                //
                // create survey
                //
                //
                //--------------------------------------------------------------
                const IntegerSurvey survey(xml,topologyT,0);
                if(survey->size<=0) {
                    Y_XML_COMMENT(xml,"no mixed equilibrium");
                    MakeELists(maxOrder,slot,equilibria);
                    return;
                }

                //--------------------------------------------------------------
                //
                //
                // decode survey
                //
                //
                //--------------------------------------------------------------
                AddressBook missing;
                for(const IntegerSurvey::ArrayType *arr=survey->head;arr;arr=arr->next)
                {
                    assert( CountNonZero(*arr)>=2 );
                    AutoPtr<MixTab>   mix = new MixTab(*arr,topology);

                    //----------------------------------------------------------
                    //
                    // sanity check
                    //
                    //----------------------------------------------------------
                    for(const MixTab *rhs = mixes.head;rhs;rhs=rhs->next)
                    {
                        if( *rhs == *mix)            throw Specific::Exception(CallSign,"corrupted mix coefficients");
                        if( rhs->stoi == mix->stoi ) throw Specific::Exception(CallSign,"corrupted mix stoichiometry");
                    }

                    //----------------------------------------------------------
                    //
                    // check that combination get rid of at least one species
                    //
                    //----------------------------------------------------------
                    if(!mix->isEfficientFor(*this,missing))
                    {
                        //Y_XMLOG(xml, "(-) " << mix);
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // now we have a valid new mix
                    //
                    //----------------------------------------------------------
#if 0
                    if(xml.verbose)
                    {
                        missing.display<Species>( xml() << "(+) " << mix << "->") << std::endl;
                    }
#endif

                    mixes.pushTail( mix.yield() );
                    InSituMax(maxOrder,arr->ncof);
                }

            }

            //------------------------------------------------------------------
            //
            //
            // sorting
            //
            //
            //------------------------------------------------------------------
            MergeSort::Call(mixes,MixTab::Compare);


            //------------------------------------------------------------------
            //
            //
            // Dispatch
            //
            //
            //------------------------------------------------------------------
            MakeELists(maxOrder,slot,equilibria);
            assert(order.size()==maxOrder);
            assert(N==equilibria->size);
            assert(N==order[1].size);


            //------------------------------------------------------------------
            //
            //
            // compile mixes into mixed from primary equilibri[um|a]
            //
            //
            //------------------------------------------------------------------
            for(const MixTab *mix=mixes.head;mix;mix=mix->next)
            {

                //--------------------------------------------------------------
                //
                // gather coefficients and their equilibria
                //
                //--------------------------------------------------------------
                WList     wlist;
                EList     elist;
                size_t    count = N;
                for(ENode *en=equilibria->head;count-- > 0;en=en->next)
                {
                    Equilibrium & eq = Coerce(**en);
                    const int     cf = eq(*mix,SubLevel);
                    if(cf) {
                        wlist << cf;
                        elist << eq;
                    }
                }

                //--------------------------------------------------------------
                //
                // create a mixed equilibrium
                //
                //--------------------------------------------------------------
                MixedEquilibrium    *mixedEq = new MixedEquilibrium(wlist,
                                                                    elist,
                                                                    species,
                                                                    mix->stoi,
                                                                    eqs.nextIndex(),
                                                                    tlK);
                Equilibrium::Pointer mixed = mixedEq;

                //--------------------------------------------------------------
                //
                // top level registration for persistent storage
                //
                //--------------------------------------------------------------
                eqs(mixed);

                //--------------------------------------------------------------
                //
                // local registration
                //
                //--------------------------------------------------------------
                equilibria << *mixed;
                content.enroll(*mixed);
                slot[mixedEq->wlist.size] << *mixed;

            }

            Y_XML_COMMENT(xml, "#mixes = " << mixes.size << " / maxOrder = " << maxOrder);
            for(size_t i=1;i<=order.size();++i)
            {
                Y_XML_COMMENT(xml, "|order[" << i << "]| = " << std::setw(3) << order[i].size);
            }
        }

        void ClusterCombinatorics:: setClusterTopology(XMLog &xml)
        {
            Y_XML_SECTION(xml,"makeGlobalTopology");
            {
                iMatrix &topo = Coerce(iNu);
                XMatrix &topx = Coerce(xNu);
                const EList &equilibria = *((**this).equilibria);
                topo.make(equilibria.size,M);
                topx.make(equilibria.size,M);
                for(const ENode *en=equilibria.head;en;en=en->next)
                {
                    const Equilibrium &eq = Coerce(**en);
                    Writable<int>     &nu = eq(topo,SubLevel);
                    eq.fillTopology(nu, SubLevel);
                    eq.fillTopology(eq(topx,SubLevel), SubLevel);
                    Y_XMLOG(xml,nu << " // " << eq.name);
                }
            }
        }

        static inline void CharacteristicOut(XMLog &              xml,
                                             const Characteristic chr,
                                             const Components &   eq)
        {
            Y_XMLOG(xml, " (*) " << CharacteristicText(chr) << " " << eq.name );
        }

        void ClusterCombinatorics:: makeCharacteristic(XMLog &xml)
        {
            static const char fn[] = "makeCharacteritic";
            Y_XML_SECTION(xml,fn);



            for(const ENode *en=(*this)->equilibria->head;en;en=en->next)
            {
                Equilibrium &eq = Coerce(**en);
                switch(eq.kind)
                {
                    case Deserted:
                        throw Specific::Exception(fn,"Deserted '%s'", eq.name->c_str());

                    case ProdOnly:
                        assert( areAllUnbounded(eq.prod) );
                        assert( 0 == eq.reac->size );
                        Coerce(onlyProd) << eq;
                        CharacteristicOut(xml,OnlyProd,eq);
                        break;

                    case ReacOnly:
                        assert( areAllUnbounded(eq.reac) );
                        assert( 0 == eq.prod->size );
                        Coerce(onlyReac) << eq;
                        CharacteristicOut(xml,OnlyReac,eq);
                        break;


                    case Standard:
                        assert(eq.reac->size>0);
                        assert(eq.prod->size>0);
                        if( areAllUnbounded(eq.reac) && areAllUnbounded(eq.prod) )
                        {
                            Coerce(nebulous) << eq;
                            CharacteristicOut(xml,Nebulous,eq);
                        }
                        else
                        {
                            Coerce(definite) << eq;
                            CharacteristicOut(xml,Definite,eq);
                        }
                }

            }
        }


        ClusterCombinatorics:: ClusterCombinatorics(XMLog                         &xml,
                                                    const ClusterContent::Pointer &ptr,
                                                    Equilibria                    &eqs,
                                                    XWritable                     &tlK) :
        ClusterConservations(xml,ptr),
        iNu(),
        xNu(),
        order(0),
        definite(),
        onlyProd(),
        onlyReac(),
        nebulous()
        {
            Y_XML_SECTION(xml, "ClusterCombinatorics");
            assert(ptr->equilibria->size>0);
            createCombinations(xml,eqs,tlK);
            setClusterTopology(xml);
            makeCharacteristic(xml);
        }

        ClusterCombinatorics:: ~ClusterCombinatorics() noexcept
        {

        }

        Characteristic ClusterCombinatorics:: characterize(const Equilibrium &eq) const noexcept
        {
            if( definite->has(eq) ) return Definite;
            if( onlyProd->has(eq) ) return OnlyProd;
            if( onlyReac->has(eq) ) return OnlyReac;
            assert(nebulous->has(eq));
            return Nebulous;
        }

    }

}
