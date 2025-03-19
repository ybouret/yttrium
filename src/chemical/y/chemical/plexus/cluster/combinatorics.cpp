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
      
       



        ClusterCombinatorics:: ClusterCombinatorics(XMLog                         &xml,
                                                    const ClusterContent::Pointer &ptr,
                                                    Equilibria                    &eqs,
                                                    XWritable                     &tlK) :
        ClusterConservations(xml,ptr),
        Nu(),
        order(0)
        {
            Y_XML_SECTION(xml, "ClusterCombinatorics");

            //------------------------------------------------------------------
            //
            //
            // prepare variables
            //
            //
            //------------------------------------------------------------------
            assert(ptr->equilibria->size>0);
            CxxListOf<MixTab> mixes;
            ClusterContent &  content    = Coerce(**this);
            SubEList       &  equilibria = content.equilibria;
            const SList    &  species    = *content.species;
            size_t            maxOrder   = 1;
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
            ELists &slot = Coerce(order);
            {
                ELists _(maxOrder);
                _.swapWith(slot);
            }
            assert(order.size()==maxOrder);
            assert(N==equilibria->size);

            for(ENode *en=equilibria->head;en;en=en->next)
            {
                slot[1] << **en;
            }

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

            //------------------------------------------------------------------
            //
            //
            // Finalize
            //
            //
            //------------------------------------------------------------------
            {
                iMatrix &topo = Coerce(Nu);
                topo.make(equilibria->size,M);
                for(ENode *en=equilibria->head;en;en=en->next)
                {
                    Equilibrium &eq = Coerce(**en);
                    eq.fillTopology(eq(topo,SubLevel), SubLevel);
#if 0
                    switch( tierOf(eq) )
                    {
                        case Deserted: throw Specific::Exception("ClusterCombinatorics", "deserted '%s'", eq.key().c_str());
                        case Nebulous: Coerce(nebulous) << eq; break;
                        case ProdOnly: Coerce(prodOnly) << eq; break;
                        case ReacOnly: Coerce(reacOnly) << eq; break;
                        case Standard: Coerce(standard) << eq; break;
                    }
#endif
                }
            }
           // Y_XMLOG(xml, "Nu=" << Nu);
        }

        ClusterCombinatorics:: ~ClusterCombinatorics() noexcept
        {

        }
    }

}
