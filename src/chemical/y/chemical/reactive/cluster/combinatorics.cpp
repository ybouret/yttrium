
#include "y/chemical/reactive/cluster/combinatorics.hpp"
#include "y/chemical/reactive/cluster.hpp"

#include "y/chemical/reactive/equilibrium/mixed.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/apex/mylar.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        class Mixed : public Object
        {
        public:

            explicit Mixed(const Readable<int> &w,
                           const Readable<int> &s,
                           const size_t         d) :
            weight(CopyOf,w),
            stoich(CopyOf,s),
            snorm1( Norm1(stoich) ),
            order(d),
            next(0),
            prev(0)
            {

            }

            static inline SignType Compare(const Mixed * lhs, const Mixed * rhs) noexcept
            {
                assert(lhs); assert(rhs);
                return Sign::Of(lhs->snorm1,rhs->snorm1);
            }

            virtual ~Mixed() noexcept {}

            const CxxArray<int> weight;
            const CxxArray<int> stoich;
            const apz           snorm1;
            const size_t        order;
            Mixed              *next;
            Mixed              *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mixed);
            inline static apz Norm1(const Readable<int> &arr)
            {
                apz res = 0;
                for(size_t i=arr.size();i>0;--i) res += abs(arr[i]);
                return res;
            }
        };



        String ClusterCombinatorics:: buildMixedName(const Readable<int> &w) const
        {
            String res;
            bool   first = true;
            for(size_t i=1;i<=w.size();++i)
            {
                const int          nu = w[i]; if(!nu) continue;
                const Equilibrium &eq = eqset[i];


                if(nu>0)
                {
                    if(!first) res += '+';
                    if(nu>1)
                        res += Formatted::Get("%d*",nu);
                    res += eq.name;
                }
                else
                {
                    res += '-';
                    if(nu< -1)
                        res += Formatted::Get("%d*",-nu);
                    res += eq.name;
                }

                first = false;
            }
            return res;
        }

        ClusterCombinatorics:: ~ClusterCombinatorics() noexcept {}

        ClusterCombinatorics:: ClusterCombinatorics(Equilibria        &eqs,
                                                    const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterConservations(fragment,topK,xml),
        blend()
        {
            static const char here[] = "Chemical::Cluster::Combinatorics";
            static const char mesg[] = "corrupted topology: ";
            Y_XML_SECTION(xml,here);
            WOVEn::IntegerSurvey survey(xml);

            //------------------------------------------------------------------
            //
            // WOVen survey linear combination
            //
            //------------------------------------------------------------------
            {
                Matrix<int> NuT(TransposeOf,Nu);
                Matrix<int> NuTx;
                Apex::Mylar::Compress(NuTx,NuT);
                Y_XMLOG(xml,"NuT  = "   << NuT);
                Y_XMLOG(xml,"NuTx = "  << NuTx);
                WOVEn::Explore(NuTx, survey, false);
                survey.sort();
                const size_t maxOrder = Max<size_t>(1,survey.maxOrder());
                Y_XMLOG(xml,"maxOrder = " << maxOrder << " / " << eqs->size() );
                Coerce(blend).reserve(maxOrder);
                const EList empty;
                for(size_t i=maxOrder;i>0;--i)
                {
                    Coerce(blend) << empty;
                }
                assert(maxOrder==blend.size());
            }

            //------------------------------------------------------------------
            //
            // Registering first order into blend[1]
            //
            //------------------------------------------------------------------
            {
                EList & el = Coerce(blend[1]);
                for(const ENode *en=head;en;en=en->next) el << **en;
            }

            if(survey.size<=0) return;

            //------------------------------------------------------------------
            //
            // convert all weights to stoichiometry
            //
            //------------------------------------------------------------------
            Y_XML_SECTION_OPT(xml, "Mixing", " survey='" << survey.size << "'");
            CxxListOf<Mixed> mixed;
            const size_t  n = Nu.rows;
            const size_t  m = Nu.cols;
            CxxArray<int> weight(n,0);
            CxxArray<int> stoich(m,0);

            WOVEn::Indices incoming(m);
            WOVEn::Indices outgoing(m);

            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                assert(arr->size()==n);
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                incoming.free();
                outgoing.free();
                const Readable<const apz> &w = *arr;

                //--------------------------------------------------------------
                // weights from apz
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) weight[i] = w[i].cast<int>("weight");

                //--------------------------------------------------------------
                // compute resulting stoichiometry with incoming/outgoind
                //--------------------------------------------------------------
                for(size_t j=m;j>0;--j)
                {
                    apz nu = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        const apz &wi = w[i];     if(__Zero__ == wi.s) continue;
                        const int  cf = Nu[i][j]; if(0==cf)            continue;
                        incoming |= j;
                        nu += wi * Nu[i][j];
                    }
                    if( 0 != (stoich[j] = nu.cast<int>("stoichiometric coefficient") ) )
                    {
                        outgoing |= j;
                    }
                }

                //--------------------------------------------------------------
                // check meaningfull
                //--------------------------------------------------------------
                const size_t numIncoming = incoming.size();
                const size_t numOutgoing = outgoing.size();

                switch( Sign::Of(numOutgoing,numIncoming) )
                {
                    case Negative: break;
                    case Positive: throw                        Specific::Exception(here, "%screating species!",mesg);
                    case __Zero__: if(incoming!=outgoing) throw Specific::Exception(here, "%sbad status-quo",mesg);
                        goto DONE;
                }

                //--------------------------------------------------------------
                // check original
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    if( Nu[i] == stoich ) goto DONE;
                }

                for(const Mixed *mx=mixed.head;mx;mx=mx->next)
                {
                    if(mx->stoich == stoich) goto DONE;
                }

                //--------------------------------------------------------------
                // store!
                //--------------------------------------------------------------
                (void) mixed.pushTail( new Mixed(weight,stoich,arr->order) );

                //--------------------------------------------------------------
                // compute missing
                //--------------------------------------------------------------
                incoming ^= outgoing;
                Y_XMLOG(xml," (+) " << weight << " -> " << stoich << ", missing=" << incoming);
            DONE:
                ;
            }

            Y_XMLOG(xml," (*) #mixed=" << mixed.size);
            if(mixed.size<=0) return;
            MergeSort::Call(mixed,Mixed::Compare);
            for(const Mixed *mx=mixed.head;mx;mx=mx->next)
            {

                //--------------------------------------------------------------
                // create mixed equilibrium
                //--------------------------------------------------------------
                const Readable<int> &weight = mx->weight;
                const String         eqName = buildMixedName(weight);
                Equilibrium         &eq     = eqs.insert( new MixedEquilibrium(eqName,eqs.topLevel(),weight,eqset,sharedK) );


                //--------------------------------------------------------------
                // register in blend
                //--------------------------------------------------------------
                Coerce(blend[mx->order]) << eq;

                //--------------------------------------------------------------
                // fill it with species
                //--------------------------------------------------------------
                const Readable<int> &stoich = mx->stoich;
                for(size_t j=1;j<=m;++j)
                {
                    const int      nu = stoich[j]; if(0==nu) continue;
                    const Species &sp = spset[j];
                    eq(nu,sp);
                }

                //--------------------------------------------------------------
                // update this cluster and eq
                //--------------------------------------------------------------
                (*this) << eq;
                Coerce(eq.indx[SubLevel]) = size; assert( ! eqset.search(size) );
                (void)  Coerce(eqset).record(eq);

                //--------------------------------------------------------------
                // update formatting both eqs and eqfmt
                //--------------------------------------------------------------
                Coerce(eqfmt).upgradeWith(eq);
                eqs.upgradeWith(eq);

                Y_XMLOG(xml," (+) " << mx->stoich << " --> " << eq.rstr << eq.LeftRightArrow << eq.pstr);

            }

            if( xml.verbose )
            {
                for(size_t i=2;i<=blend.size();++i)
                {
                    Y_XMLOG(xml, "#blend[" << std::setw(3) << i << "] = " << blend[i].size);
                }
            }

        }



    }

}
