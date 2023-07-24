
//! \file

#ifndef Y_Sequence_Vector_Included
#define Y_Sequence_Vector_Included 1

#include "y/sequence/interface.hpp"
#include "y/container/dynamic.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Vector
        {
        public: static const char * const CallSign; //!< "Vector"
        protected: explicit Vector() noexcept;
        public:    virtual ~Vector() noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Vector);
        };
    }

    template <typename T, typename ALLOCATOR>
    class Vector : public Dynamic, public Sequence<T>, public Core::Vector
    {
    public:
        virtual ~Vector() noexcept;
        
    private:
        
    };
}

#endif
