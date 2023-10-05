//! \file

#ifndef Y_Associative_Registry_Included
#define Y_Associative_Registry_Included 1

#include "y/associative/interface.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Associative for keyed object with key() method
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class Registry : public Associative<KEY,T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);  //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Registry() noexcept {} //!< setup

    public:
        inline virtual ~Registry() noexcept {} //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        
        virtual bool insert(ParamType) = 0; //!< insert keyed object


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Registry);
    };

}

#endif
