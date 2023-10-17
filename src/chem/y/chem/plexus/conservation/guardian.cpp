
#include "y/chem/plexus/conservation/guardian.hpp"
//#include "y/data/small/light/list/solo.hpp"

#include "y/type/nullify.hpp"
#include "y/data/pool/cxx.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {

        namespace
        {

            class Monitor : public Object, public XAdd
            {
            public:
                typedef CxxListOf<Monitor> List;
                typedef CxxPoolOf<Monitor> Pool;

                inline explicit Monitor() : Object(), XAdd(), species(0), next(0), prev(0) {}
                inline virtual ~Monitor() noexcept { }


                const Species *species;
                Monitor *      next;
                Monitor *      prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Monitor);
            };

            class Excess
            {
            public:

                inline Excess(const Conservation &h,
                              const xreal        &d) noexcept :
                host(h),
                data(d),
                rank((d*d)/h.nrm2)
                {
                }

                inline ~Excess() noexcept
                {
                }

                Excess(const Excess &other) noexcept :
                host(other.host),
                data(other.data),
                rank(other.rank)
                {
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Excess &xs)
                {
                    os << xs.host << "=" << double(xs.data) << ":rank=" << double(xs.rank);
                    return os;
                }


                const Conservation &host;
                const xreal         data;
                const xreal         rank;

            private:
                Y_DISABLE_ASSIGN(Excess);
            };

        }

        class Guardian:: Code : public Object
        {
        public:
            typedef Small::SoloHeavyList<Excess> XsList;
            typedef XsList::NodeType             XsNode;

            inline virtual ~Code() noexcept {}

            inline explicit Code() : Object(), xadd(), mlist(), mpool()
            {
            }

            inline void setup(const Canon &canon)
            {
                assert(canon.repo.size>0);

                // prepare monitors
                make(canon.repo.size);

                // link monitors
                assert(mlist.size==canon.repo.size);
                Monitor *mine = mlist.head;
                for(const SpNode *node=canon.repo.head;node;node=node->next)
                {
                    mine->species = & **node;
                    mine->free();
                }
            }

            inline void run(const Canon     &canon,
                            Writable<xreal> &C0,
                            XMLog           &xml)
            {
                Y_XML_SECTION_OPT(xml, "Guardian", " size='" << canon.size << "'");
                setup(canon);

                //--------------------------------------------------------------
                //
                // first pass
                //
                //--------------------------------------------------------------
                issue.free();
                for(const Canon::NodeType *node=canon.head;node;node=node->next)
                {
                    const Conservation &cons = **node;
                    const xreal         xs   = cons.excess(C0,xadd);
                    const bool          bad  = xs.mantissa<0;
                    Y_XMLOG(xml, (bad? " (-) " : " (+) " ) << std::setw(15) << double(xs) << " = " << cons);
                    if(bad)
                    {
                        issue << Excess(cons,xs);
                    }
                }


                if(issue.size<=0)
                {
                    Y_XMLOG(xml, "==> no issue found");
                    return;
                }

                //--------------------------------------------------------------
                //
                // find smallest rank
                //
                //--------------------------------------------------------------
                XsNode *best = issue.head; assert(0!=best);
                for(XsNode *node=best->next;node;node=node->next)
                {
                    assert(0!=node);
                    if( (**node).rank < (**best).rank )
                    {
                        best = node;
                    }
                }
                Y_XMLOG(xml, "best: " << *best);

                //--------------------------------------------------------------
                //
                // compute correction
                //
                //--------------------------------------------------------------
                const Excess       &the =    **best;
                const Conservation &cns =  the.host;
                const xreal         num = -the.data;
                const xreal         den =  cns.nrm2;
                for(const Actor *a=cns->head;a;a=a->next)
                {
                    const xreal    dC = (a->xn * num)/den;
                    const Species &sp = a->sp;
                    C0[sp.indx[TopLevel]] += dC;
                }
                std::cerr << "new xs: " << cns.excess(C0,xadd) << std::endl;


            }

            XsList        issue;
            XAdd          xadd;
            Monitor::List mlist;
            Monitor::Pool mpool;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void make(const size_t ns)
            {
                while(mlist.size>ns)
                    mpool.store(mlist.popTail());

                while(mlist.size<ns)
                {
                    if(mpool.size>0)
                        mlist.pushTail( mpool.query() );
                    else
                        mlist.pushTail( new Monitor() );
                }
            }
        };

        Guardian:: ~Guardian() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Guardian:: Guardian() : code( new Code() )
        {
            std::cerr << "sizeof(Guadian::Code)=" << sizeof(Code) << std::endl;
            std::cerr << "sizeof(Monitor)      =" << sizeof(Monitor) << std::endl;
        }

        void Guardian:: run(const Canon     &canon,
                            Writable<xreal> &C0,
                            XMLog           &xml)
        {
            assert(0!=code);
            code->run(canon, C0, xml);
        }


    }

}


