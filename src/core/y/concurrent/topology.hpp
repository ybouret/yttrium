//! \file

#ifndef Y_Concurrent_Topology_Included
#define Y_Concurrent_Topology_Included 1

#include "y/data/small/heavy/list/bare.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        typedef Small::BareHeavyList<size_t> CPUs; //!< alias
        typedef CPUs::NodeType               CPU;  //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Topology to bind CPUs
        //
        //
        //______________________________________________________________________
        class Topology :  public CPUs
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      EnvName;  //!< Y_NUM_THREADS
            static const char * const      CallSign; //!< "Topology"
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Topology(const size_t nproc,
                              const size_t start=0,
                              const size_t every=1); //!< setup with parameters
            explicit Topology();                     //!< setup to Hardware::NumProcs() or Y_NUM_THREADS
            explicit Topology(const String &);       //!< parse string
            explicit Topology(const char   *);       //!< parse text
            virtual ~Topology() noexcept;            //!< cleanup
            Topology(const Topology &);              //!< hard copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! assign to a list of 
            template <typename LIST> inline
            bool assign(LIST &crew) const noexcept
            {
                assert(crew.size==size);
                typename LIST::NodeType *node = crew.head;
                bool                     flag = true;
                for(const CPU *cpu=head;cpu;cpu=cpu->next,node=node->next)
                {
                    assert(0!=node);
                    assert(0!=cpu);
                    if(!node->assign( **cpu )) flag = false;
                }
                return flag;
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

