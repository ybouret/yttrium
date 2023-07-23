
//! \file

#ifndef Y_Container_Readable_Included
#define Y_Container_Readable_Included 1

#include "y/container/interface.hpp"
#include "y/type/args.hpp"
#include <iostream>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Readable container
    //
    //
    //__________________________________________________________________________
    template <typename T> class Readable : public Container
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T); //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Readable() noexcept : Container() {} //!< setup
    public:
        inline virtual ~Readable() noexcept {}               //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        virtual ConstType & operator[](const size_t) const noexcept = 0; //!< access items in [1..size()]

        inline friend std::ostream & operator<<(std::ostream &os, const Readable &self)
        {
            const size_t n = self.size();
            if(n<=0)
            {
                os << LBrack << RBrack;
            }
            else
            {
                os << LBrack;
                os << self[1];
                for(size_t i=2;i<=n;++i) os << SemiColon << self[i];
                os << RBrack;
                
            }
            return os;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Readable);
    };
}

#endif
