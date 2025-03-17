



#include "y/chemical/plexus/cluster/combinatorics.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/apex/api/count-non-zero.hpp"
#include "y/data/small/heavy/list/bare.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        typedef Small::BareHeavyList<const int> WList;
        typedef WList::NodeType                 WNode;

        class MixTab : public Quantized, public CxxArray<int,Memory::Dyadic>
        {
        public:
            static const char * const CallSign;
            typedef CxxArray<int,Memory::Dyadic> ArrayType;

            explicit MixTab(const IntegerSurvey::ArrayType &arr,
                            const iMatrix                  &topo) :
            ArrayType(arr.size()),
            ncof(arr.ncof),
            stoi(topo.cols),
            next(0),
            prev(0)
            {
                const size_t n = arr.size();
                for(size_t i=n;i>0;--i)
                {
                    if( !arr[i].tryCast( (*this)[i]) )
                        throw Specific::Exception(CallSign,"coefficient overflow");
                }

                const size_t m = stoi.size();
                for(size_t j=m;j>0;--j)
                {
                    apz sum = 0;
                    for(size_t i=n;i>0;--i)
                        sum += arr[i] * topo[i][j];

                    if( !sum.tryCast(stoi[j]))
                        throw Specific::Exception(CallSign,"stoichiometry overflow");
                }

            }

            virtual ~MixTab() noexcept
            {
            }


            bool isEfficientFor(const ClusterTopology &cl, AddressBook &source) const
            {
                static const char msg[] = "corrupted mix stoichiometry ";
                source.free();

                // gather created species from stoichiometry
                AddressBook target;
                for(const SNode *sn=cl->species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if( 0 != sp(stoi,SubLevel) ) target += sp;
                }

                // gather original species from intial equilibria
                if(cl->equilibria->size > cl.N )
                {
                    std::cerr << "Expanded Set !!" << std::endl;
                }
                size_t n=cl.N;
                for(const ENode *en=cl->equilibria->head;n-- > 0;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if( 0 != eq(*this,SubLevel) ) eq.gatherSpeciesIn(source);
                }


                // compare
                const size_t src = source.size();
                const size_t tgt = target.size();

                switch( Sign::Of(tgt,src) )
                {
                    case Positive:
                        throw Specific::Exception(CallSign,"%sexcess",msg);

                    case __Zero__:
                        if( !(source==target) )
                            throw Specific::Exception(CallSign,"%sequality!!",msg);
                        return false;

                    case Negative:
                        break;
                }

                for(AddressBook::Iterator it=target.begin();it!=target.end();++it)
                {
                    const void * addr = *it;
                    if(!source.remove_(addr))
                        throw Specific::Exception(CallSign,"%scompression",msg);
                }

                return true;
            }

            //! comparison by ncof then lexicographic
            static SignType Compare(const MixTab * const lhs,
                                    const MixTab * const rhs) noexcept
            {
                switch( Sign::Of(lhs->ncof,rhs->ncof) )
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__:
                        break;
                }
                return LexicographicCompare(*lhs,*rhs);
            }

            const size_t ncof;
            ArrayType    stoi;
            MixTab *     next;
            MixTab *     prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixTab);
        };

        const char * const MixTab::CallSign = "Mixing";

        class MixedEquilibrium : public Equilibrium
        {
        public:

            static String FirstName(const int cf, const String &id)
            {
                String ans;
                assert(cf!=0);
                ans += Formatted::Get("%+d",cf);
                ans += id;
                return ans;
            }

            static String ExtraName(const int cf, const String &id)
            {
                String ans;
                assert(cf>0);
                ans += Formatted::Get("%+d",cf);
                ans += id;
                return ans;
            }


            static const String *MakeName(const WList &wl,
                                          const EList &el)
            {
                assert(wl.size==el.size);
                assert(wl.size>=2);
                const WNode *wn = wl.head;
                const ENode *en = el.head;

                AutoPtr<String> ans = new String();
                *ans += FirstName(**wn,*(**en).name);
                for(wn=wn->next,en=en->next;wn;wn=wn->next,en=en->next)
                {
                    *ans += FirstName(**wn,*(**en).name);
                }


                return ans.yield();
            }

            explicit MixedEquilibrium(WList               &wl,
                                      EList               &el,
                                      const SList         &sl,
                                      const Readable<int> &st,
                                      const size_t         ii,
                                      XWritable           &KK) :
            Equilibrium( MakeName(wl,el), ii),
            topK(KK),
            wlist(),
            elist(),
            xmul()
            {
                // steal data
                Coerce(wlist).swapWith(wl);
                Coerce(elist).swapWith(el);

                // fill
                for(const SNode *sn=sl.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    const int      cf = sp(st,SubLevel);
                    switch( Sign::Of(cf) )
                    {
                        case __Zero__: continue;
                        case Positive: use(Product,  cf,sp); break;
                        case Negative: use(Reactant,-cf,sp); break;
                    }
                }

                latch();
            }

            virtual ~MixedEquilibrium() noexcept {}

            XWritable   &topK;
            const WList  wlist;
            const EList  elist;
            XMul         xmul;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(xreal_t)
            {
                return 1;
            }
        };


    }

}




namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;


        ClusterCombinatorics:: ClusterCombinatorics(XMLog                         &xml,
                                                    const ClusterContent::Pointer &ptr,
                                                    Equilibria                    &eqs,
                                                    XWritable                     &tlK) :
        ClusterConservations(xml,ptr),
        maxOrder(1)
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
            SubEList       &  equilibria = Coerce((*this)->equilibria);
            const SList    &  species    = *((*this)->species);

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
                        Y_XMLOG(xml, "(-) " << mix);
                        continue;
                    }

                    //----------------------------------------------------------
                    //
                    // now we have a valid new mix
                    //
                    //----------------------------------------------------------
                    if(xml.verbose)
                    {
                        missing.display<Species>( xml() << "(+) " << mix << "->") << std::endl;
                    }
                    mixes.pushTail( mix.yield() );
                    InSituMax(Coerce(maxOrder),arr->ncof);
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
            // compile mixes into mixed from primary equilibri[um|a]
            //
            //
            //------------------------------------------------------------------
            for(const MixTab *mix=mixes.head;mix;mix=mix->next)
            {
                WList     wlist;
                EList     elist;
                size_t    count = N;
                for(ENode *en=equilibria->head;count-- > 0;en=en->next)
                {
                    Equilibrium & eq = **en;
                    const int     cf = eq(*mix,SubLevel);
                    if(cf) {
                        wlist << cf;
                        elist << eq;
                    }
                }
                //  AutoPtr<const String> ptr = MixedEqulibrium::MakeName(wlist,elist);
                // std::cerr << "(+) " << ptr << std::endl;

                Equilibrium::Pointer mixed = new MixedEquilibrium(wlist,
                                                                  elist,
                                                                  species,
                                                                  mix->stoi,
                                                                  eqs.nextIndex(),
                                                                  tlK);
                Y_XMLOG(xml,mixed);

            }

            Y_XML_COMMENT(xml, "#mixes = " << mixes.size << " / maxOrder = " << maxOrder);

        }

        ClusterCombinatorics:: ~ClusterCombinatorics() noexcept
        {

        }
    }

}
