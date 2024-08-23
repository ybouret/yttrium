
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/woven/survey/natural.hpp"
#include "y/woven/subspaces.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/type/temporary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Conservations");

            //------------------------------------------------------------------
            //
            // preparing survey
            //
            //------------------------------------------------------------------
            WOVEn::NaturalSurvey survey(xml);

            //------------------------------------------------------------------
            //
            // survey orthogonal space of topology
            //
            //------------------------------------------------------------------
            {
                const Temporary<bool> quiet(Coerce(xml.verbose),false);
                Matrix<apz> Q;
                if(! MKL::OrthoSpace::Make(Q,Nu) )
                    throw Specific::Exception("Conservations", "singular topology ortho-space");
                if(Q.cols>0)
                    WOVEn::Explore(Q,survey,true);
            }


            //------------------------------------------------------------------
            //
            // if no result, all species are unbounded
            //
            //------------------------------------------------------------------
            if(survey.size<=0)
            {
                Y_XMLOG(xml, "No conservation law");
                for(const SNode *sn=species.head;sn;sn=sn->next)
                    Coerce(unbounded) << **sn;
                return;
            }

            //------------------------------------------------------------------
            //
            // else convert survey to laws
            //
            //------------------------------------------------------------------
            const size_t m = species.size;
            {
                survey.sort();
                Matrix<unsigned> &Q = Coerce(Qm);
                Q.make(survey.size,m);
                size_t i = 0;
                for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
                {
                    Writable<unsigned>        &q = Q[++i];
                    const Readable<const apn> &a = *arr;
                    for(size_t j=m;j>0;--j)    q[j] = a[j].cast<unsigned>("conservation");
                }
            }
            Y_XML_SECTION(xml, "Summary");
            Y_XMLOG(xml, "Nu=" << Nu );
            Y_XMLOG(xml, "Qm=" << Qm );

            //------------------------------------------------------------------
            //
            // Create individual laws
            //
            //------------------------------------------------------------------
            Coerce(laws) = new Conservation::Laws(species,Qm);
            AddressBook cdb;
            for(CLaw *law = laws->head;law;law=law->next)
            {
                law->record(cdb);    // record conserved species
                law->extract(*this); // here, will use only primary
            }

            //------------------------------------------------------------------
            //
            // Collect final group info
            //
            //------------------------------------------------------------------
            for(CLaws::Group *g=laws->groups.head;g;g=g->next)
            {
                Y_XML_SECTION_OPT(xml, "Conservation::Group", " size='" << g->size << "'");
                AddressBook book;
                for(const Conservation::LNode *ln=g->head;ln;ln=ln->next)
                {
                    const CLaw &law = **ln;
                    Y_XMLOG(xml,law);
                    //Y_XMLOG(xml, "\t|_keep=" << law.keep);
                    Y_XMLOG(xml, "\t|_nrm2=" << real_t(law.xden) );
                    const Matrix<real_t> proj(CopyOf,law.proj);
                    Y_XMLOG(xml, "\t|_proj=" << proj);
                    Y_XMLOG(xml, "\t|_base=" << law.base);
                    for(const ENode *en = law.base.head;en;en=en->next)
                        book |= **en;
                }

                Indexed::SortBy<TopLevel>::Using( book.sendTo(Coerce(g->base)) );
                Y_XMLOG(xml, "group.base=" << g->base);
            }


            //------------------------------------------------------------------
            //
            // qualify species according to cdb content
            //
            //------------------------------------------------------------------
            for(const SNode *sn=species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if( cdb.has(sp) )
                    Coerce(conserved) << sp;
                else
                    Coerce(unbounded) << sp;
            }

            Indexed::SortBy<TopLevel>::Using( Coerce(conserved.list) );
            Indexed::SortBy<TopLevel>::Using( Coerce(unbounded.list) );
            

        }
    }

}
