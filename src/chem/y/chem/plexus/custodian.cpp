
#include "y/chem/plexus/custodian.hpp"
#include "y/type/nullify.hpp"
#include "y/data/pool/cxx.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {


        namespace
        {
            //__________________________________________________________________
            //
            //
            //! Accumulator of correction for a conserved species
            //
            //__________________________________________________________________
            class Accumulator : public Object, public XAdd
            {
            public:
                typedef CxxListOf<Accumulator> List;
                typedef CxxPoolOf<Accumulator> Pool;

                inline explicit Accumulator() : Object(), XAdd(), host(0), next(0), prev(0) {}
                inline virtual ~Accumulator() noexcept {}

                const Species *host;
                Accumulator *  next;
                Accumulator *  prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Accumulator);
            };


            //__________________________________________________________________
            //
            //
            //! store conservation excess
            //
            //__________________________________________________________________
            class Excess
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
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

                //______________________________________________________________
                //
                // member
                //______________________________________________________________
                const Conservation &host; //!< unbalanced conservation
                const xreal         data; //!< raw excess (negative)
                const xreal         rank; //!< rank

            private:
                Y_DISABLE_ASSIGN(Excess);
            };
        }


    }

}


namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! code for Custodian
        //
        //
        //______________________________________________________________________
        class Custodian:: Code : public Object
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
            inline explicit Code() : issue(), accum(), sbook(), xlist(), xpool() {}
            inline virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! prepare memory for a given plexus
            inline void prepare(const Plexus &plexus, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Custodian::Prepare");
                size_t msi = 0;
                size_t nsp = 0;
                for(const Cluster *cluster=plexus->head;cluster;cluster=cluster->next)
                {
                    msi  = Max(cluster->kmsi,msi);
                    nsp += cluster->knum;
                }
                Y_XMLOG(xml, "-- Max Kept Species Index = " << msi);
                Y_XMLOG(xml, "-- All Kept Species Count = " << nsp);

                // prepare vector of accumulator pointers
                accum.adjust(msi,0);

                // prepare accumulators
                while(xlist.size>nsp) xpool.store(xlist.popTail())->free();
                while(xlist.size<nsp) xlist.pushTail( (xpool.size>0) ? xpool.query() : new Accumulator() );

                // link accumulator in given slot
                accum.ld(0);
                Accumulator *acc = xlist.head;
                for(const Cluster *cluster=plexus->head;cluster;cluster=cluster->next)
                {
                    const Booleans &  kept = *(cluster->kept);
                    for(const SpNode *node = cluster->lib.head;node;node=node->next)
                    {
                        const Species &sp = **node;
                        if(! kept[ sp.indx[SubLevel]] ) continue;

                        assert(sp.indx[TopLevel] <= accum.size());
                        assert(0!=acc);

                        (accum[sp.indx[TopLevel]] = acc)->host = &sp;
                        acc = acc->next;
                    }
                }
            }


            void apply(const Canon     &canon,
                       Writable<xreal> &Corg,
                       XMLog           &xml)
            {
                Y_XML_SECTION(xml, "Custodian::Canon");
                //--------------------------------------------------------------
                //
                // first pass : populate issues
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

                //--------------------------------------------------------------
                //
                // second pass : reduce issues
                //
                //--------------------------------------------------------------
                size_t cycle = 0;
            CYCLE:
                ++cycle;
                if(issue.size<=0)
                {
                    if(modified)
                    {
                        // done
                        Y_XMLOG(xml, "  |-- no more issue found @cycle=" << cycle);
                        return;
                    }
                    else
                    {
                        // early done
                        Y_XMLOG(xml, "  |-- no issue found");
                        return;
                    }

                }

                Y_XMLOG(xml, "  |-- processing #issue=" << issue.size << " @cycle=" << cycle);

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
                assert(0!=best);
                Y_XMLOG(xml, "  |-- selected: " << *best);

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
                        const size_t   sj = sp.indx[TopLevel];
                        Y_XMLOG(xml, "  |-- [+" << std::setw(15) << double(dC) << "] @" << sp);
                        assert(0!=accum[sj]);
                        Corg[sj] += dC;
                        accum[sj]->insert(dC);
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

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XsList                               issue;
            XAdd                                 xadd;
            Vector<Accumulator *,Memory::Dyadic> accum; //!< dispatched accumulator for species top-level
            AddressBook                          sbook; //!< modified species
            Accumulator::List                    xlist; //!< active accumulators
            Accumulator::Pool                    xpool; //!< available accumulators



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

    }

}

namespace Yttrium
{
    namespace Chemical
    {

        Custodian:: Custodian() : code( new Code() )
        {

        }

        Custodian:: ~Custodian() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        void Custodian:: prepare(const Plexus &plexus, XMLog &xml)
        {
            assert(0!=code);
            code->prepare(plexus,xml);
        }

        void Custodian:: enter() noexcept
        {
            assert(0!=code);
            for(Accumulator *acc=code->xlist.head;acc;acc=acc->next)
            {
                assert(0!=acc->host);
                acc->free();
            }
            code->sbook.free();
        }



        void Custodian:: operator()(const Cluster   &cluster,
                                    Writable<xreal> &Corg,
                                    XMLog           &xml)
        {
            Y_XML_SECTION(xml, "Custodian::Cluster");
            assert(0!=code);
            for(const Canon *canon=cluster.law.head;canon;canon=canon->next)
            {
                code->apply(*canon,Corg,xml);
            }
        }

        //! apply the law for a prepared plexus
        void Custodian:: operator()(const Plexus    &plexus,
                                    Writable<xreal> &Corg,
                                    XMLog           &xml)
        {
            Y_XML_SECTION(xml, "Custodian::Plexus");
            assert(0!=code);
            for(const Cluster *cluster=plexus->head;cluster;cluster=cluster->next)
            {
                (*this)(*cluster,Corg,xml);
            }
        }
    }

}
