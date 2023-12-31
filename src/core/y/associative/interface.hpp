//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"
#include <iostream>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Associative interface
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class Associative : public Dynamic
    {
    public:
        Y_ARGS_DECL(T,Type);  //!< aliases
        Y_ARGS_DECL(KEY,Key); //!< aliases

    protected:
        inline explicit Associative() noexcept {}
    public:
        inline virtual ~Associative() noexcept {}

        virtual ConstType *search(ParamKey) const noexcept = 0; //!< search object by its key
        virtual Type *     search(ParamKey)       noexcept = 0; //!< search object by its key, may optimize for next search
        virtual bool       remove(ParamKey)       noexcept = 0; //!< remove object by its key

        //! helper to display one NODE
        template <typename NODE>
        static inline void ShowNode(std::ostream &os, const NODE *node)
        {
            assert(0!=node);
            os << ' ' << node->key << ':' << **node;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Associative);
    };

}

#endif

