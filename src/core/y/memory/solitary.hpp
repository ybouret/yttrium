
//! \file

#ifndef Y_Memory_Solitary_Included
#define Y_Memory_Solitary_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Solitary item interface
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Solitary
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! erase content
            inline void erase() noexcept {
                if(0!=data)
                {
                    Memory::OutOfReach::Naught(data);
                    data = 0;
                }
            }

            //! build with default argument
            inline Type & build() {
                erase();
                return *( data = new ( get() ) T() );
            }

            //! build with one argument
            template <typename U>
            inline Type &build(U &args) {
                erase();
                return *( data = new ( get() ) T(args) );
            }

            //! dismiss content, assuming it was copied elsewhere
            inline void dismiss() noexcept {
                (void) Memory::OutOfReach::Zero(data,sizeof(T));
                data = 0;
            }


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Solitary() noexcept { erase(); }

        protected:
            inline explicit Solitary() noexcept :data(0) {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            MutableType *data; //!< (constructed) data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solitary);
            virtual void    *get() noexcept = 0;
        };

    }

}

#endif
