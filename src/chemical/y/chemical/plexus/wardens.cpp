
#include "y/chemical/plexus/wardens.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Wardens:: ~Wardens() noexcept
        {
        }

        static const char Name[] = "Chemical::Wardens";

        Wardens:: Wardens(const Clusters &cls) :
        Warden::Set()
        {
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                const Cluster     &mine = *cl;
                const Warden::Ptr  wptr = new Warden(mine);
                if(!insert(wptr)) throw Specific::Exception(Name,"couldn't insert new Warden");
            }
        }


        void Wardens:: operator()(const Clusters &cls,
                                  XWritable      &C,
                                  XWritable      &dC,
                                  const Level     L,
                                  XMLog          &xml)
        {

            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                const Cluster       &     mine = *cl;
                const LittleEndianAddress clk(mine);
                Warden::Ptr *             ppw  = search(clk);
                if(!ppw) throw Specific::Exception(Name,"foreing cluster provided");
                Warden &w = **ppw;

                w(C,dC,L,xml);
            }

        }

    }

}
