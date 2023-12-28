//! \file

#ifndef Y_Associative_POD_Key_Included
#define Y_Associative_POD_Key_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/args.hpp"
#include "y/type/copy.hpp"

#include <cstring>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! convert  a (quasi) Plain Old Datatype into a Memory::ReadOnlyBuffer
    //
    //
    //__________________________________________________________________________
    template <typename POD>
    class POD_Key : public Memory::ReadOnlyBuffer
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(POD,Type); //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! build with one arg
        template <typename ARG1>
        inline POD_Key(const CopyOf_ &,
                       const ARG1    &arg1) noexcept : data(arg1) {}

        //! build with two args
        template <typename ARG1, typename ARG2>
        inline POD_Key(const CopyOf_ &,
                       const ARG1    &arg1,
                       const ARG2    &arg2) noexcept : data(arg1,arg2) {}

        //! build with three args
        template <typename ARG1, typename ARG2, typename ARG3>
        inline POD_Key(const CopyOf_ &,
                       const ARG1 &arg1,
                       const ARG2 &arg2,
                       const ARG3 &arg3) noexcept : data(arg1,arg2,arg3) {}



        //! build relying on POD(const POD &) noexcept
        inline POD_Key(const ParamType user) noexcept : data(user) {}

        //! copy relying on POD(const POD &) noexcept
        inline POD_Key(const POD_Key &other) noexcept :
        Memory::ReadOnlyBuffer(),
        data(other.data)
        {
        }

        //! cleanup
        inline virtual ~POD_Key() noexcept {
        }

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        inline virtual const void * ro_addr() const noexcept { return &data; }          //!< &data
        inline virtual size_t       measure() const noexcept { return sizeof(data); }   //!< sizeof(data)

        //! byte-wise testing
        inline friend bool operator==(const POD_Key &lhs, const POD_Key &rhs) noexcept
        {
            return 0 == memcmp(&lhs.data, &rhs.data, sizeof(POD) );
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const POD data; //!< POD like data

    private:
        Y_DISABLE_ASSIGN(POD_Key);
    };
}

#endif

