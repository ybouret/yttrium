
#include "y/chemical/plexus/cluster/conservations.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: ~Law() noexcept
            {
            }

            Law:: Law() noexcept :
            Actors(Actor::AsConcentration),
            next(0),
            prev(0)
            {
            }
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Laws:: ~Laws() noexcept
            {
            }

            Laws:: Laws() noexcept :
            Proxy<const Law::List>(),
            Assembly(),
            list()
            {
            }

            Y_PROXY_IMPL(Laws,list)


            void Laws:: add(Law *const law) noexcept
            {
                assert(0!=law);
                {
                    String & oldName = Coerce(*law->name);
                    String   newName = "0=d(" + oldName + ")";
                    newName.swapWith(oldName);
                }

                list.pushTail(law)->latch();
                enroll(*law);

            }

            std::ostream & operator<<(std::ostream &os, const Laws &laws)
            {
                os << "{";
                if(laws->size>0)
                {
                    os << std::endl;
                    for(const Law *law=laws->head;law;law=law->next)
                    {
                        os << "\t" << law->name << std::endl;
                    }
                }
                os << "}";
                return os;
            }

        }

    }

}


#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/natural.hpp"
#include "y/apex/api/count-non-zero.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        ClusterConservations:: ~ClusterConservations() noexcept
        {

        }

        ClusterConservations:: ClusterConservations( XMLog &xml, const ClusterContent::Pointer &ptr) :
        ClusterTopology(xml,ptr),
        preserved(),
        conserved()
        {
            Y_XML_SECTION(xml,"ClusterConservations");

            Matrix<apz> Q;
            if(!OrthoSpace::Make(Q,topology)) {
                throw Specific::Exception(CallSign, "no orthogonal space (singular equilibri%s)", Plural::aum(N));
            }

            const NaturalSurvey survey(xml,Q,0);
            if(survey->size<=0) {
                Y_XMLOG(xml,"no conservation found");
                return;
            }

            {
                uMatrix &Cm = Coerce(preserved);
                Cm.make(survey->size,M);


                size_t                cidx = 1;
                for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                {
                    const Readable<apn> &coef = *node; assert(CountNonZero(coef)>=2);
                    Writable<unsigned>  &cons = Cm[cidx];
                    AutoPtr<Conservation::Law> claw = new Conservation::Law();
                    for(const SNode *sn = (*this)->species->head; sn; sn=sn->next)
                    {
                        const Species &sp = **sn;
                        const size_t   sj = sp.indx[SubLevel];
                        unsigned &     cf = cons[sj];
                        if( !coef[sj].tryCast(cf) )
                            throw Specific::Exception(CallSign, "conservation coefficient overflow for [%s]", sp.name->c_str());
                        if(0!=cf)
                        {
                            (*claw)(cf,sp);
                        }
                    }
                    std::cerr << claw->name << std::endl;
                    Coerce(ordinance).add( claw.yield() );
                }
            }

            Y_XMLOG(xml, "topology  = " << topology);
            Y_XMLOG(xml, "preserved = " << preserved);
            Y_XMLOG(xml, "laws      = " << ordinance);
        }


#if 0
        void Cluster:: conservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "conservations");

            Matrix<apz> Q;
            if(!OrthoSpace::Make(Q,topology)) {
                throw Specific::Exception(CallSign, "no orthogonal space");
            }

            const NaturalSurvey survey(xml,Q,0);
            if(survey->size<=0) {
                Y_XMLOG(xml,"no conservation");
                return;
            }

            {
                uMatrix &Cm = Coerce(preserved);
                const size_t M = my.species->size;
                Cm.make(survey->size,M);


                size_t cidx = 1;
                for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                {
                    const Readable<apn> &coef = *node; assert(CountNonZero(coef)>=2);
                    Writable<unsigned>  &cons = Cm[cidx];
                    for(size_t j=M;j>0;--j) {
                        if( !coef[j].tryCast(cons[j]))
                            throw Specific::Exception(CallSign, "conservation coefficient overflow");
                    }
                }
            }

            Y_XMLOG(xml, "topology  = " << topology);
            Y_XMLOG(xml, "preserved = " << preserved);


        }
#endif

    }

}

