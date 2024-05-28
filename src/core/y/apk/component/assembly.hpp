//! \file

#ifndef Y_APK_Component_Assembly_Included
#define Y_APK_Component_Assembly_Included 1

#include "y/apk/component/rework.hpp"
#include "y/tow/api.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/byte-count.hpp"

#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace APK
    {

        //______________________________________________________________________
        //
        //
        //
        //! inner assembly of unsigned types
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Assembly {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from user's data
            inline Assembly(void * const blockAddr,
                            const size_t numBlocks,
                            const size_t numActive=0) noexcept :
            space( numBlocks ),
            entry( static_cast<T *>(blockAddr) ),
            count( numActive )
            {
                assert(0!=entry);
                assert(space>0);
                assert(count<=space);
            }

            //! make a TEMPORARY assembly from a PERSISTENT uint64_t, used as workspace
            inline Assembly(uint64_t &qw) noexcept :
            space( sizeof(uint64_t)   ),
            entry( (T*)&qw ),
            count( Rework::To(entry,qw) )
            {
                
            }


            //! cleanup
            inline ~Assembly() noexcept { Coerce(count)=0; Coerce(space)=0; Coerce(entry)=0; }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Assembly &self)
            {
                os
                << '[' << std::setw(4) << self.count << '/' << std::setw(4) << self.space << ']'
                << '@' << (const void *)(self.entry);
                Hexadecimal::Display(os << '=',self.entry,self.count);
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! transmogrify
            template <typename U> inline
            void load(const Assembly<U> &source)
            {
                // TARGET = T, SOURCE = U
                const size_t cycles = TOW::API<T,U>::Cycles(count,source.count);
                TOW::Transmute(entry,source.entry,cycles);
            }



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t space; //!< maximum count
            T * const    entry; //!< data
            const size_t count; //!< valid entries

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };

    }

}

#endif

