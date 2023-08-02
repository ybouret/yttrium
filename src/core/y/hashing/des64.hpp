//! \file

#ifndef Y_Hashing_DES64_Included
#define Y_Hashing_DES64_Included 1

#include "y/hashing/mix64.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        //______________________________________________________________________
        //
        //
        //
        //! DES-like hashing
        //
        //
        //______________________________________________________________________
        class DES64 : public Mix64
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Hashing::DES64"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit DES64() noexcept; //!< setup
            virtual ~DES64() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()                         const noexcept; //!< CallSign
            virtual void         operator()(uint32_t &, uint32_t &) const noexcept; //!< compute
            bool                 test()                             const noexcept; //!< test challenge
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DES64);
        };

    }

}

#endif

