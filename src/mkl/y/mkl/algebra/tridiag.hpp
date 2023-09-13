//! \file

#ifndef Y_MKL_Algebra_TriDiag_Included
#define Y_MKL_Algebra_TriDiag_Included 1

#include "y/container/writable.hpp"


namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class TriDiag
        {
        public:
            explicit TriDiag(const size_t n);
            virtual ~TriDiag() noexcept;

            size_t size() const noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(TriDiag);
            class Code;
            Code *code;
        public:
            Writable<T> &a;
            Writable<T> &b;
            Writable<T> &c;

            T operator()(const size_t i, const size_t j) const;

        };
    }
}

#endif

