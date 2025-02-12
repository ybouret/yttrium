
//! \file

#ifndef Y_Container_Readable_Included
#define Y_Container_Readable_Included 1

#include "y/container/collection.hpp"
#include "y/type/args.hpp"
#include "y/type/signs.hpp"
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
    template <typename T> class Readable : public virtual Collection
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Readable() noexcept : Collection() {} //!< setup
    public:
        inline virtual ~Readable() noexcept {}                //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        virtual ConstType & operator[](const size_t) const noexcept = 0; //!< access items in [1..size()]

        //______________________________________________________________________
        //
        //! default display
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //
        // Comparisons
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! object-wise comparison
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //! single object comparison
        //______________________________________________________________________
        inline friend bool operator==(const Readable<T> &lhs, ParamType rhs) noexcept
        {
            return 1 == lhs.size() && rhs == lhs[1];
        }

        //______________________________________________________________________
        //
        //! single object comparision
        //______________________________________________________________________
        inline friend bool operator==(ParamType lhs, const Readable<T> &rhs) noexcept
        {
            return 1 == rhs.size() && lhs == rhs[1];
        }

        //______________________________________________________________________
        //
        //! object-wise comparison
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        //! single object difference
        //______________________________________________________________________
        inline friend bool operator!=(const Readable<T> &lhs, ParamType rhs) noexcept
        {
            return 1 != lhs.size() || rhs != lhs[1];
        }

        //______________________________________________________________________
        //
        //! single object difference
        //______________________________________________________________________
        inline friend bool operator!=(ParamType lhs, const Readable<T> &rhs) noexcept
        {
            return 1 != rhs.size() || lhs != rhs[1];
        }

        //______________________________________________________________________
        //
        //! lexicographic comparison
        //______________________________________________________________________
        inline static SignType LexicographicCompare(const Readable<T> &lhs,
                                                    const Readable<T> &rhs)
        {
            const size_t ls = lhs.size();
            const size_t rs = rhs.size();


            switch( Sign::Of(ls,rs) )
            {
                case Negative: return lxcmp(lhs,rhs);
                case __Zero__: break;
                case Positive: return Sign::Opposite(lxcmp(rhs,lhs));
            }
            for(size_t i=1;i<=ls;++i)
            {
                switch( Sign::Of(lhs[i],rhs[i]))
                {
                    case Negative: return Negative;
                    case __Zero__: continue;
                    case Positive: return Positive;
                }
            }
            return __Zero__; // all the same, same length
        }


        //! apply the same method to all data
        template <typename METHOD_POINTER> inline
        const Readable &forEachConst(METHOD_POINTER meth) const
        {
            Readable<T> &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                (self[i].*(meth))();
            }
            return self;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Readable);
        static inline SignType lxcmp(const Readable<T> &lhs, const Readable<T> &rhs)
        {
            assert( lhs.size() < rhs.size() );
            const size_t n = lhs.size();
            for(size_t i=1;i<=n;++i)
            {
                switch( Sign::Of(lhs[i],rhs[i]))
                {
                    case Negative: return Negative;
                    case __Zero__: continue;
                    case Positive: return Positive;
                }
            }
            return Negative; // lhs is prefix of rhs or greater than rhs
        }
    };
}

#endif
