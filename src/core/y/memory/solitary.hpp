
//! \file

#ifndef Y_Memory_Solitary_Included
#define Y_Memory_Solitary_Included 1

#include "y/memory/out-of-reach.hpp"
#include "y/ptr/ptr.hpp"

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
        template <typename T, template <typename> class Policy = Immediate >
        class Solitary : public Ptr<T,Policy>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            //typedef Ptr<T,Immediate> PtrType; //!< alias
            Y_ARGS_EXPOSE(T,Type);            //!< aliases

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! erase content
            inline void erase() noexcept {
                if(0!=this->handle)
                {
                    Memory::OutOfReach::Naught(this->handle);
                    this->handle = 0;
                }
            }

            //! build with default argument
            inline Type & build() {
                erase();
                return *( this->handle = new ( get() ) T() );
            }

            //! build with one argument
            template <typename U>
            inline Type &build(U &args) {
                erase();
                return *( this->handle = new ( get() ) T(args) );
            }

            //! build with two arguments
            template <typename U, typename V>
            inline Type &build(U &argu, V &argv) {
                erase();
                return *( this->handle = new ( get() ) T(argu,argv) );
            }

            //! build with thhree arguments
            template <typename U, typename V, typename W>
            inline Type &build(U &argu, V &argv, W &argw) {
                erase();
                return *( this->handle = new ( get() ) T(argu,argv,argw) );
            }

            //! dismiss content, assuming it was copied elsewhere
            inline void dismiss() noexcept {
                (void) Memory::OutOfReach::Zero(this->handle,sizeof(T));
                this->handle = 0;
            }


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            
            //! cleanup, handle must be taken care of beforehand
            inline virtual ~Solitary() noexcept { assert(0 == this->handle); }

        protected:
            //! setup
            inline explicit Solitary() noexcept : Ptr<T,Policy>(0) {}



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solitary);
            virtual void    *get() noexcept = 0;
        };

    }

}

#endif
