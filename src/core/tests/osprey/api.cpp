
#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/light/list/coop.hpp"



namespace Yttrium
{
    namespace Osprey
    {

        typedef Small::CoopHeavyList<size_t> IndexList;
        typedef IndexList::NodeType          IndexNode;
        typedef Small::CoopLightList<size_t> AliasList;
        typedef AliasList::NodeType          AliasNode;


        typedef IndexList::CacheType         IndexBank;
        typedef AliasList::CacheType         AliasBank;

    }
}

using namespace Yttrium;

Y_UTEST(osprey)
{

    Osprey::IndexBank ibank;
    Osprey::AliasBank abank;

    Y_SIZEOF(Osprey::IndexNode);
    Y_SIZEOF(Osprey::AliasNode);

}
Y_UDONE()
