//! \file

#ifndef Y_Container_Implanted_Included
#define Y_Container_Implanted_Included 1

#include "y/container/operating.hpp"
#include "y/memory/embed.hpp"

namespace Yttrium
{

    template <typename T> class Implanted : public Operating<T>
    {
    public:
        inline explicit Implanted(Memory::Embed &emb) :
        Operating<T>(emb.address(), emb.blocks)
        {
        }

        template <typename U>
        inline explicit Implanted(Memory::Embed &emb,
                                  U             &arguments) :
        Operating<T>(emb.address(), emb.blocks, arguments)
        {
        }
        
        inline virtual ~Implanted() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Implanted);
    };
}

#endif
