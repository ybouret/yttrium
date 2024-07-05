
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Species::~Species() noexcept
        {
        }

        void Species:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=egg";
            Endl(fp<<']');
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << '[' << sp.name << ']';
            return os;
        }

        xreal_t Species:: Concentration(Random::Bits &ran)
        {
            const real_t p = PMIN + (PMAX-PMIN)*ran.to<real_t>();
            return pow(10.0,p);
        }


        SList:: SList() noexcept : SList_() {}
        SList:: ~SList() noexcept {}
        SList:: SList(const SList &other) : SList_(other) {}


        void SList:: transfer(XWritable       &target,
                              const Level      tgtlvl,
                              const XReadable &source,
                              const Level     &srclvl) const
        {
            for(const SNode *node=head;node;node=node->next)
            {
                const size_t * const indx = (**node).indx;
                target[ indx[tgtlvl] ] = source[ indx[srclvl] ];
            }
        }

    }
}
