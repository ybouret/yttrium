
#include "y/chem/plexus/custodian.hpp"
#include "y/type/nullify.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace
        {
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
        }


    }

}


namespace Yttrium
{
    namespace Chemical
    {

        class Custodian:: Code : public Object
        {
        public:

            inline explicit Code() : accum(), sbook(), xlist(), xpool() {}
            inline virtual ~Code() noexcept {}


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

                // link vector
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
        }



    }

}
