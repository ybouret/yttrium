
#include "y/chemical/plexus/cluster/listing.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Listing:: Listing() noexcept : neutral(), charged() {}
        Listing:: ~Listing() noexcept {}

        size_t  Listing:: size() const noexcept
        { return neutral.size+charged.size; }


        void Listing::  sort()
        {
            MergeSort::Call( Coerce(neutral), MetaList<SList>::Compare);
            MergeSort::Call( Coerce(charged), MetaList<SList>::Compare);
        }

    }

}


