#include "y/chemical/plexus/devices.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Devices:: ~Devices() noexcept
        {
        }

        static const char Name[] = "Chemical::Devices";

        Devices:: Devices(const Clusters &cls, const bool doTrace) : Device::Set()
        {
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                const Cluster     &mine = *cl;
                const Device::Ptr  dptr = new Device(mine,doTrace);
                if(!insert(dptr)) throw Specific::Exception(Name,"couldn't insert new Device");
            }
        }


        void Devices:: operator()(XWritable &       C,
                                  const Level       L,
                                  const XReadable & K,
                                  XMLog &           xml)
        {

            size_t nc = size();
            for(Iterator it=begin();nc>0;--nc,++it)
            {
                Device &dev = **it;
                dev(C,L,K,xml);
            }

        }
    }

}


