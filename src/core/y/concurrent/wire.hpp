
//! \file

#ifndef Y_Concurrent_Wire_Included
#define Y_Concurrent_Wire_Included 1

#include "y/concurrent/thread.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {

            //__________________________________________________________________
            //
            //
            //
            //! formatting PROC/ARGS for Thread
            //
            //
            //__________________________________________________________________
            class WireData
            {
            public:
                virtual ~WireData() noexcept; //!< cleanup

            protected:

                //! PROC is a function, ARGS is an object
                template <typename PROC, typename ARGS> inline
                explicit WireData(PROC &proc, ARGS &args) noexcept :
                procAddr( (void*) proc), argsAddr(&args)
                {
                    assert(0!=procAddr);
                    assert(0!=argsAddr);
                }

                //! provide this
                void *Args() noexcept;

                //! retrive PROC/ARGS and call
                template <typename PROC, typename ARGS> static inline
                void Call(void *addr)
                {
                    assert(0!=addr);
                    WireData &self = *static_cast<WireData*>(addr);
                    assert(0!=self.procAddr);
                    assert(0!=self.argsAddr);

                    PROC &proc = *(PROC *)self.procAddr;
                    ARGS &args = *(ARGS *)self.argsAddr;
                    proc(args);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(WireData);
                void * const procAddr;
                void * const argsAddr;

            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! wrapper to execute function on object
        //
        //
        //______________________________________________________________________
        class Wire : public Nucleus:: WireData, public Thread
        {
        public:

            //! will launch proc(args)
            template <typename PROC, typename ARGS> inline
            explicit Wire(PROC &proc, ARGS &args) :
            Nucleus:: WireData(proc,args), Thread(Call<PROC,ARGS>, Args() )
            {
            }

            //! cleanup
            virtual ~Wire() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wire);


        };

    }

}

#endif
