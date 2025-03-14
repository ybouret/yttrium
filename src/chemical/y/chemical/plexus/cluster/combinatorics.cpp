
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/equilibria.hpp"
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
            typedef CxxArray<int,Memory::Dyadic> ArrayType;

            explicit MixTab(const IntegerSurvey::ArrayType &arr,
                            const iMatrix                  &topo) :
            ArrayType(arr.size()),
            ncof(arr.ncof),
            stoi(topo.cols)
            {
                const size_t n = arr.size();
                for(size_t i=n;i>0;--i)
                {
                    if( !arr[i].tryCast( (*this)[i]) )
                        throw Specific::Exception(Cluster::CallSign,"coefficient overflow");
                }

                const size_t m = stoi.size();
                for(size_t j=m;j>0;--j)
                {
                    apz sum = 0;
                    for(size_t i=n;i>0;--i)
                        sum += arr[i] * topo[i][j];

                    if( !sum.tryCast(stoi[j]))
                        throw Specific::Exception(Cluster::CallSign,"stoichiometry overflow");
                }

            }

            virtual ~MixTab() noexcept
            {
            }


            bool isEfficientFor(const ClusterType &cl, AddressBook &source) const
            {
                static const char msg[] = "corrupted mix stoichiometry ";
                source.free();

                AddressBook target;
                for(const SNode *sn=cl.species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if( 0 != sp(stoi,SubLevel) ) target += sp;
                }

                for(const ENode *en=cl.equilibria->head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if( 0 != eq(*this,SubLevel) ) eq.gatherSpeciesIn(source);
                }


                const size_t src = source.size();
                const size_t tgt = target.size();

                switch( Sign::Of(tgt,src) )
                {
                    case Positive:
                        throw Specific::Exception(Cluster::CallSign,"%sexcess",msg);

                    case __Zero__:
                        if( !(source==target) )
                            throw Specific::Exception(Cluster::CallSign,"%sequality!!",msg);
                        return false;

                    case Negative:
                        break;
                }

                for(AddressBook::Iterator it=target.begin();it!=target.end();++it)
                {
                    const void * addr = *it;
                    if(!source.remove_(addr))
                        throw Specific::Exception(Cluster::CallSign,"%scompression",msg);
                }

                return true;
            }

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

        class MixedEqulibrium : public Equilibrium
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

            explicit MixedEqulibrium(WList               &wl,
                                     EList               &el,
                                     const SList         &sl,
                                     const Readable<int> &st,
                                     const size_t         ii) :
            Equilibrium( MakeName(wl,el), ii), wlist(), elist(), xmul()
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

            virtual ~MixedEqulibrium() noexcept {}

            const WList wlist;
            const EList elist;
            XMul        xmul;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEqulibrium);
            virtual xreal_t getK(xreal_t)
            {
                return 1;
            }
        };

        void Cluster:: combinatorics(XMLog &xml, Equilibria &eqs)
        {
            Y_XML_SECTION(xml, "combinatorics");

            CxxListOf<MixTab> mixes;
            size_t            maxOrder = 1;
            {
                const IntegerSurvey survey(xml,topologyT,0);
                if(survey->size<=0) {
                    Y_XML_COMMENT(xml,"no combinatorics");
                    return;
                }
                else
                {
                    AddressBook missing;
                    for(const IntegerSurvey::ArrayType *arr=survey->head;arr;arr=arr->next)
                    {
                        assert( CountNonZero(*arr)>=2 );
                        AutoPtr<MixTab>   mix = new MixTab(*arr,topology);
                        for(const MixTab *rhs = mixes.head;rhs;rhs=rhs->next)
                        {
                            if( *rhs == *mix)            throw Specific::Exception(CallSign,"corrupted mix coefficients");
                            if( rhs->stoi == mix->stoi ) throw Specific::Exception(CallSign,"corrupted mix stoichiometry");
                        }

                        if(!mix->isEfficientFor(**this,missing))
                        {
                            Y_XMLOG(xml, "(-) " << mix);
                            continue;
                        }

                        if(xml.verbose)
                        {
                            missing.display<Species>( xml() << "(+) " << mix << "->") << std::endl;
                        }
                        mixes.pushTail( mix.yield() );
                        InSituMax(maxOrder,arr->ncof);
                    }
                }
            }

            // sorting
            MergeSort::Call(mixes,MixTab::Compare);

            // compiling
            for(const MixTab *mix=mixes.head;mix;mix=mix->next)
            {
                WList     wlist;
                EList     elist;
                for(ENode *en=my.equilibria->head;en;en=en->next)
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

                Equilibrium::Pointer mixed = new MixedEqulibrium(wlist,
                                                                 elist,
                                                                 *my.species,
                                                                 mix->stoi,
                                                                 eqs.nextIndex());
                std::cerr << mixed << std::endl;

            }

            Y_XML_COMMENT(xml, "#mixes = " << mixes.size << " / maxOrder = " << maxOrder);
        }

    }

}
