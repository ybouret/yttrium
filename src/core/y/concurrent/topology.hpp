//! \file

#ifndef Y_Concurrent_Topology_Included
#define Y_Concurrent_Topology_Included 1

#include "y/data/small/heavy/list/bare.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        typedef Small::BareHeavyList<size_t> CPUs;
        typedef CPUs::NodeType               CPU;

        class Topology :  public CPUs
        {
        public:
            static const char * const      EnvName;  //!< Y_NUM_THREADS
            static const char * const      CallSign; //!< "Topology"
            
            virtual ~Topology() noexcept;
            explicit Topology();
            explicit Topology(const String &);
            explicit Topology(const char   *);
            explicit Topology(const size_t nproc,
                              const size_t start=0,
                              const size_t every=1);
            Topology(const Topology &);

            template <typename LIST> inline
            void assign(LIST &crew) const
            {
                assert(crew.size==size);
                typename LIST::NodeType *node = crew.head;
                for(const CPU *cpu=head;cpu;cpu=cpu->next,node=node->next)
                {
                    assert(0!=node);
                    assert(0!=cpu);
                    node->assign( **cpu );
                }
            }

        private:
            Y_DISABLE_ASSIGN(Topology);
            void parse(const String &);
            void create(const size_t nproc,
                        const size_t start,
                        size_t       every);
        };
        




    }
}

#endif

