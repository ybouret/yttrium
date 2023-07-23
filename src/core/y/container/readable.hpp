
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
        Y_ARGS_EXPOSE(T,Type); //!< aliases

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

        //! default display
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

        //! object-wise comparison
        inline friend bool operator==(const Readable<T> &lhs, const Readable<T> &rhs) noexcept
        {
            const size_t n = lhs.size();
            if(n!=rhs.size()) return false;
            for(size_t i=n;i>0;--i)
            {
                if(rhs[i]!=lhs[i]) return false;
            }
            return true;
        }

        //! object-wise comparison
        inline friend bool operator!=(const Readable<T> &lhs, const Readable<T> &rhs) noexcept
        {
            const size_t n = lhs.size();
            if(n!=rhs.size()) return true;
            for(size_t i=n;i>0;--i)
            {
                if(rhs[i]!=lhs[i]) return true;
            }
            return false;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Readable);
    };
}

#endif
