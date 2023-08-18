
//! \file

#ifndef Y_Information_Entropy_Included
#define Y_Information_Entropy_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Information
    {
        //______________________________________________________________________
        //
        //
        //
        //! Information Entropy
        //
        //
        //______________________________________________________________________
        class Entropy
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Entropy();          //!< setup
            virtual ~Entropy() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void      restart()                                  noexcept; //!< restart all
            Entropy & operator<<(const uint8_t)                  noexcept; //!< count occurence
            Entropy & operator<<(const Memory::ReadOnlyBuffer &) noexcept; //!< count occurences

            //! current value
            long double operator()(void) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entropy);
            unsigned  shift;
            uint64_t *count;
        };
    }

}

#endif

