
//! \file

#ifndef Y_Hashing_IBJ64_Included
#define Y_Hashing_IBJ64_Included 1

#include "y/hashing/mix64.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        //! integer hashing of 32 bits words
        uint32_t IBJ32(uint32_t) noexcept;

        //______________________________________________________________________
        //
        //
        //
        //! Bob Jenkins' integer hash
        //
        //
        //______________________________________________________________________
        class IBJ64 : public Mix64
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Hashing::IBJ64"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit IBJ64() noexcept; //!< setup
            virtual ~IBJ64() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()                         const noexcept; //!< CallSign
            virtual void         operator()(uint32_t &, uint32_t &) const noexcept; //!< compute

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IBJ64);
        };

    }

}

#endif

