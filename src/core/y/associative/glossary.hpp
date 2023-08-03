
//! \file

#ifndef Y_Associative_Glossary_Included
#define Y_Associative_Glossary_Included 1

#include "y/associative/interface.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Associative for distinct (key,type)
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class Glossary : public Associative<KEY,T>
    {
    public:
        Y_ARGS_DECL(T,Type);  //!< aliases
        Y_ARGS_DECL(KEY,Key); //!< aliases
    protected:
        inline explicit Glossary() noexcept {} //!< setup

    public:
        inline virtual ~Glossary() noexcept {} //!< cleanup

        virtual bool insert(ParamKey,ParamType) = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Glossary);
    };

}

#endif
