
//! \file

#ifndef Y_Data_List_Raw_Included
#define Y_Data_List_Raw_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! List of raw nodes, managed by user
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class RawListOf : public ListOf<NODE>
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit RawListOf() noexcept : ListOf<NODE>() {}
        inline virtual ~RawListOf() noexcept { reset(); }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! (hard) reset
        inline void reset() noexcept {
            this->tail         = 0;
            this->head         = 0;
            Coerce(this->size) = 0;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(RawListOf);
    };

}

#endif
