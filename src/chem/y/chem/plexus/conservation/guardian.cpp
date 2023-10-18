
#include "y/chem/plexus/conservation/guardian.hpp"
#include "y/type/nullify.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/address-book.hpp"

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

                inline explicit Monitor() : Object(), XAdd(), next(0), prev(0) {}
                inline virtual ~Monitor() noexcept { }

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
                    os << std::setw(15) << double(xs.data) << " = " << xs.host;
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Small::SoloHeavyList<Excess> XsList;
            typedef XsList::NodeType             XsNode;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Code() : Object(), issue(), xadd(), accum(), sbook(), slist(), mlist(), mpool() {}
            inline virtual ~Code() noexcept {}



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! assume sbook is ok
            inline void correct(const Canon     &canon,
                                Writable<xreal> &Corg,
                                Writable<xreal> &Cerr,
                                XMLog           &xml)
            {
                setup(canon);

                //--------------------------------------------------------------
                //
                // first pass : populate issue and clear error
                //
                //--------------------------------------------------------------
                bool modified = false;
                issue.free();
                for(const Canon::NodeType *node=canon.head;node;node=node->next)
                {
                    const Conservation &cons = **node;
                    const xreal         xs   = cons.excess(Corg,xadd);
                    const bool          bad  = xs.mantissa<0;
                    Y_XMLOG(xml, (bad? " (-) " : " (+) " ) << std::setw(15) << double(xs) << " = " << cons);
                    if(bad)
                    {
                        issue << Excess(cons,xs);
                        modified = true;
                    }
                }



            CYCLE:
                if(issue.size<=0)
                {
                    if(modified)
                    {
                        Y_XMLOG(xml, "  |-- no more issue found");
                        // expand errors
                        size_t i = 1;
                        for(const SpNode *node=canon.repo.head;node;node=node->next,++i)
                        {
                            const Species &sp = **node;
                            Cerr[sp.indx[TopLevel]] = accum[i]->sum();
                        }
                        return;
                    }
                    else
                    {
                        Y_XMLOG(xml, "  |-- no issue found");
                        for(const SpNode *node=canon.repo.head;node;node=node->next)
                        {
                            const Species &sp = **node;
                            Cerr[sp.indx[TopLevel]] = 0;
                        }
                        return;
                    }

                }

                Y_XMLOG(xml, "  |-- processing #issue=" << issue.size);

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
                Y_XMLOG(xml, "  |-- best: " << *best);

                //--------------------------------------------------------------
                //
                // compute correction
                //
                //--------------------------------------------------------------
                {
                    const Excess       &the =    **best;
                    const Conservation &cns =  the.host;
                    const xreal         num = -the.data;
                    const xreal         den =  cns.nrm2;
                    for(const Actor *a=cns->head;a;a=a->next)
                    {
                        const xreal    dC = (a->xn * num)/den;
                        const Species &sp = a->sp;
                        Y_XMLOG(xml, "  |-- [+" << std::setw(15) << double(dC) << "] @" << sp);
                        Corg[sp.indx[TopLevel]] += dC;
                        accum[sp.indx[AuxLevel]]->insert(dC);
                        sbook |= sp;
                    }
                    Y_XMLOG(xml, "  |-- " << double(num) << " -> " << double(cns.excess(Corg,xadd)) );
                }

                //--------------------------------------------------------------
                //
                // no more issue
                //
                //--------------------------------------------------------------
                issue.cutNode(best);

                //--------------------------------------------------------------
                //
                // check remaining issues
                //
                //--------------------------------------------------------------
                {
                    XsList target;
                    while(issue.size>0)
                    {
                        XsNode     *node = issue.head;
                        Excess     &temp = (**node);
                        const xreal left = temp.host.excess(Corg,xadd);
                        if(left.mantissa<0)
                        {
                            // still active
                            Coerce(temp.data) = left;
                            Coerce(temp.rank) = (left*left)/temp.host.nrm2;
                            target.pushTail(issue.popHead());
                            Y_XMLOG(xml, " (-) " << temp);
                        }
                        else
                        {
                            // solved
                            issue.cutHead();
                            Y_XMLOG(xml, " (+) " << temp.host);
                        }
                    }
                    issue.swapWith(target);
                }
                goto CYCLE;
            }

            const SpList  &sendBookToList()
            {
                slist.free();
                const AddressBook::Iterator last = sbook.end();
                for(AddressBook::Iterator it=sbook.begin(); it!=last; ++it)
                {
                    slist << * static_cast<const Species *>( *it );
                }
                return slist;
            }

            XsList         issue;
            XAdd           xadd;
            Vector<XAdd *> accum;
            AddressBook    sbook;
            SpList         slist;
            Monitor::List  mlist;
            Monitor::Pool  mpool;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void setup(const Canon &canon)
            {
                assert(canon.repo.size>0);
                const size_t ns = canon.repo.size;

                //--------------------------------------------------------------
                // adjust accum
                //--------------------------------------------------------------
                accum.adjust(ns,0);

                //--------------------------------------------------------------
                // prepare monitors
                //--------------------------------------------------------------
                while(mlist.size>ns)
                    mpool.store(mlist.popTail());

                while(mlist.size<ns)
                {
                    if(mpool.size>0)
                        mlist.pushTail( mpool.query() );
                    else
                        mlist.pushTail( new Monitor() );
                }

                //--------------------------------------------------------------
                // link monitors
                //--------------------------------------------------------------
                assert(mlist.size==canon.repo.size);
                assert(mlist.size==accum.size());

                size_t i = 1;
                for(Monitor *m=mlist.head;m;m=m->next,++i)
                    (accum[i] = m)->free();
            }


        };

        Guardian:: ~Guardian() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Guardian:: Guardian() : code( new Code() )
        {

        }

        void Guardian:: correct(const Canon     &canon,
                                Writable<xreal> &Corg,
                                Writable<xreal> &Cerr,
                                XMLog           &xml)
        {
            assert(0!=code);
            Y_XML_SECTION_OPT(xml, "Guardian::Canon", " size='" << canon.size << "'");
            code->correct(canon, Corg, Cerr, xml);
        }

        void Guardian:: correct(const Cluster   &cluster,
                                Writable<xreal> &Corg,
                                Writable<xreal> &Cerr,
                                XMLog           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Guardian::Cluster", " #law='" << cluster.law.size << "'");
            for(const Chemical::Canon *canon=cluster.law.head;canon;canon=canon->next)
            {
                correct(*canon,Corg,Cerr,xml);
            }
        }

        const SpList & Guardian:: operator()(const Plexus    &plexus,
                                             Writable<xreal> &Corg,
                                             Writable<xreal> &Cerr,
                                             XMLog           &xml)
        {
            assert(0!=code);
            Y_XML_SECTION_OPT(xml, "Guardian::Plexus", " #cluster='" << plexus->size << "'");

            code->sbook.free();
            for(const Cluster *cluster=plexus->head;cluster;cluster=cluster->next)
            {
                correct(*cluster,Corg,Cerr,xml);
            }
            return code->sendBookToList();
        }
    }

}


