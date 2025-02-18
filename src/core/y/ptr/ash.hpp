
//! \file

#ifndef Y_Ash_Ptr_Included
#define Y_Ash_Ptr_Included 1

#include "y/ptr/ptr.hpp"
#include "y/type/copy.hpp"

namespace Yttrium
{


    namespace Core
    {
        class AshPtr
        {
        public:
            explicit AshPtr(const void *);
            virtual ~AshPtr() noexcept;

        protected:
            size_t * const nref;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AshPtr);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Authorizing SHared Pointer
    //
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class Policy = Immediate >
    class AshPtr : public Ptr<T,Policy>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Ptr<T,Policy> SelfType; //!< alias
        Y_ARGS_EXPOSE(T,Type);          //!< aliases
        using SelfType::handle;         //!< alias

        

    private:

    };

}

#endif

