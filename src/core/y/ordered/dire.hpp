//! \file

#ifndef Y_Ordered_Dire_Included
#define Y_Ordered_Dire_Included 1

#include "y/type/args.hpp"
#include "y/ordered/core/dire.hpp"
#include "y/type/capacity.hpp"

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //
    //
    //! Parametrized Dire access, mimicking Heap
    //
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename RAW_BUFFER>
    class Dire : public RAW_BUFFER, public Core::Dire
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);              //!< aliases
        typedef RAW_BUFFER RawBufferType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup empty
        inline explicit Dire() noexcept : RawBufferType() {}

        //! setup with capacity
        inline explicit Dire(const size_t n, const AsCapacity_ &) noexcept : RawBufferType(n) {}

        //! cleanup
        inline virtual ~Dire() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! insert object
        inline void      insert(ParamType args)
        { this->insertAtTail(args);   }


        //! insert mutliple objects
        inline void      insert(ParamType args, size_t n)
        { while(n-- > 0) this->insertAtTail(args); }


        //! remove top object
        inline void      remove() noexcept
        { this->removeAtTail(); }

        //! pull top object
        inline ConstType pull()
        { return this->uprootAtTail(); }

        //! [Identifiable] CallSign
        virtual const char * callSign() const noexcept { return CallSign; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dire);
    };




}


#endif
