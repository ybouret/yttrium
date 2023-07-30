//! \file

#ifndef Y_Container_Implanted_Included
#define Y_Container_Implanted_Included 1

#include "y/container/operating.hpp"
#include "y/memory/embed.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //
    //! combine embedded memory and operating
    //
    //
    //__________________________________________________________________________
    template <typename T> class Implanted : public Operating<T>
    {
    public:
        //__________________________________________________________________
        //
        //
        // C++
        //
        //__________________________________________________________________

        //! setup objects into embedded memory, default constructor
        inline explicit Implanted(Memory::Embed &emb) : Operating<T>(emb.address(), emb.blocks)
        {
        }


        //! setup objects into embedded memory, one argument constructor
        template <typename U>
        inline explicit Implanted(Memory::Embed &emb,
                                  U             &arguments) :
        Operating<T>(emb.address(), emb.blocks, arguments)
        {
        }

        //! cleanup
        inline virtual ~Implanted() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Implanted);
    };
}

#endif
