
#include "y/concurrent/topology.hpp"
#include "y/string/env.hpp"
#include "y/system/hw.hpp"
#include "y/system/exception.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/algo/crop.hpp"
#include "y/text/ascii/convert.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        Topology:: ~Topology() noexcept
        {
        }


        const char * const Topology:: EnvName  = "Y_NUM_THREADS";
        const char * const Topology:: CallSign = "Concurrent::Topology";

#define Y_TOPOLOGY_CTOR() CPUs()

        Topology:: Topology() : Y_TOPOLOGY_CTOR()
        {
            String topo;
            if( Environment::Get(topo,EnvName) )
            {
                parse(topo);
            }
            else
            {
                const size_t np = Hardware::NumProcs();
                for(size_t i=0;i<np;++i) (*this) << i;
            }
        }

        Topology:: Topology(const Topology &other) : CPUs(other)
        {
        }
        


        Topology:: Topology(const String &topo) :  Y_TOPOLOGY_CTOR()
        {
            parse(topo);
        }

        Topology:: Topology(const char *topo) :  Y_TOPOLOGY_CTOR()
        {
            const String _(topo);
            parse(_);
        }



        Topology:: Topology(const size_t nproc,
                            const size_t start,
                            const size_t every) :
        Y_TOPOLOGY_CTOR()
        {
            create(nproc,start,every);
        }

        void Topology:: create(const size_t nproc, const size_t start, size_t every)
        {
            const size_t np = Hardware::NumProcs();
            if(nproc<=0) throw Specific::Exception(CallSign,"zero CPU");
            if(every<=0) every = 1;
            for(size_t i=0,j=start;i<nproc;++i,j+=every)
            {
                (*this) << (j%np);
            }
        }

        static inline bool IsSep(const int c) noexcept
        {
            switch(c)
            {
                case ' ':
                case '\n':
                case '\r':
                case '\t':
                    return true;

                default:
                    break;
            }
            return false;
        }

        static inline
        size_t GetField(const char *name,
                        String     &field)
        {
            assert(0!=name);
            Algo::Crop(field,IsSep);
            if(field.size()<=0) return 0;
            return ASCII::Convert::To<size_t>(field,name);
        }

        void Topology:: parse(const String &topo)
        {
            Vector<String,Memory::Pooled> field(3,AsCapacity);
            Tokenizer::AppendTo(field,topo, ":", 1);
            if(field.size()<=0) throw Specific::Exception(CallSign,"no field in '%s'",        topo.c_str());
            if(field.size()>3)  throw Specific::Exception(CallSign,"too many fields in '%s'", topo.c_str());
            
            const size_t nproc = GetField("Topology::nproc",field[1]);
            size_t       start = 0;
            size_t       every = 0;
            if(field.size()>=2) start = GetField("Topology::start",field[2]);
            if(field.size()>=3) every = GetField("Topology::every",field[3]);
            create(nproc,start,every);
        }

    }
}

