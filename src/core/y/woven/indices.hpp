//! \file

#ifndef Y_WOVEn_Indices_Included
#define Y_WOVEn_Indices_Included 1

#include "y/ordered/vector.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //! base class for indices
        typedef OrderedVector<size_t,IncreasingComparator,Memory::Pooled> Indices_;

        //______________________________________________________________________
        //
        //
        //
        //! Set of indices to keep track of used vectors
        //
        //
        //______________________________________________________________________
        class Indices : public Indices_
        {
        public:
            explicit Indices(const size_t n); //!< setup
            virtual ~Indices() noexcept;      //!< cleanup
            Indices(const Indices &);         //!< copy

            //! record all indices corresponding to not-zero values of source
            template <typename SOURCE> inline
            void record(SOURCE &source) {
                Indices &self = *this;
                for(size_t j=source.size();j>0;--j)
                { if(0!=source[j]) self |= j; }
            }


        private:
            Y_DISABLE_ASSIGN(Indices);
        };

    }
}

#endif

