
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
                host(h), data(d)
                {
                }

                inline ~Excess() noexcept
                {
                }

                Excess(const Excess &other) noexcept :
                host(other.host),
                data(other.data)
                {
                }


                const Conservation &host;
                xreal               data;

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

                // hook monitors
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

                // first pass
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
                    return;
                



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


