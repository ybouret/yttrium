

#include "y/memory/guild.hpp"
#include "y/memory/pages.hpp"
#include "y/memory/album.hpp"

#include "y/utest/run.hpp"
#include "../alea.hpp"

#include <cstring>

using namespace Yttrium;

namespace
{
    template <typename T>
    class XNode
    {
    public:
        XNode *next;
        XNode *prev;
        T      data;

        inline const T & operator*() const noexcept
        {
            return data;
        }

        inline  XNode() : next(0), prev(0), data(0) {}
        inline ~XNode() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XNode);
    };
}

Y_UTEST(memory_guild)
{
    alea_seed();

    typedef XNode<short>  iNode;
    typedef XNode<double> dNode;

    typedef ListOf<iNode> iList;
    typedef ListOf<dNode> dList;

    Memory::Album        album;
    Memory::Guild<iNode> iG(album);
    Memory::Guild<dNode> dG(album);

    iG.zstore( iG.zombie() );
    dG.zstore( dG.zombie() );

    iList il;
    dList dl;
    for(size_t i=1000;i>0;--i)
    {
        if(alea()>0.5) il.pushTail( iG.construct() ); else il.pushHead( iG.construct() );
        if(alea()>0.5) dl.pushTail( dG.construct() ); else dl.pushHead( dG.construct() );
    }

    while( il.size ) iG.eradicate( il.popTail() );
    while( dl.size ) dG.eradicate( dl.popTail() );




    
}
Y_UDONE()

