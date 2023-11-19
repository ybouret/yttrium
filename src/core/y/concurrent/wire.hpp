
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

                //! PROC is a function, ARG1 is an object
                template <typename PROC, typename ARG1> inline
                explicit WireData(PROC &proc, ARG1 &args) noexcept :
                procAddr( (void*) proc), arg1Addr(&args), arg2Addr(0)
                {
                    assert(0!=procAddr);
                    assert(0!=arg1Addr);
                }

                //! PROC is a function, ARG1 and ARG2 are objects
                template <typename PROC, typename ARG1, typename ARG2> inline
                explicit WireData(PROC &proc, ARG1 &arg1, ARG2 &arg2) noexcept :
                procAddr( (void*) proc), arg1Addr(&arg1), arg2Addr(&arg2)
                {
                    assert(0!=procAddr);
                    assert(0!=arg1Addr);
                    assert(0!=arg2Addr);
                }

                //! provide this
                void *Args() noexcept;

                //! retrive PROC/ARG1 and call
                template <typename PROC, typename ARG1> static inline
                void Call1(void *addr)
                {
                    assert(0!=addr);
                    WireData &self = *static_cast<WireData*>(addr);
                    assert(0!=self.procAddr);
                    assert(0!=self.arg1Addr);

                    PROC &proc = *(PROC *)self.procAddr;
                    ARG1 &arg1 = *(ARG1 *)self.arg1Addr;
                    proc(arg1);
                }

                //! retrive PROC/ARG1/ARG2 and call
                template <typename PROC, typename ARG1, typename ARG2> static inline
                void Call2(void *addr)
                {
                    assert(0!=addr);
                    WireData &self = *static_cast<WireData*>(addr);
                    assert(0!=self.procAddr);
                    assert(0!=self.arg1Addr);
                    assert(0!=self.arg2Addr);

                    PROC &proc = *(PROC *)self.procAddr;
                    ARG1 &arg1 = *(ARG1 *)self.arg1Addr;
                    ARG2 &arg2 = *(ARG2 *)self.arg2Addr;
                    proc(arg1,arg2);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(WireData);
                void * const procAddr;
                void * const arg1Addr;
                void * const arg2Addr;
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! wrapper to execute threaded function on object
        //
        //
        //______________________________________________________________________
        class Wire : public Nucleus:: WireData, public Thread
        {
        public:

            //! will launch proc(arg1)
            template <typename PROC, typename ARG1> inline
            explicit Wire(PROC &proc, ARG1 &arg1) :
            Nucleus:: WireData(proc,arg1), Thread(Call1<PROC,ARG1>, Args() )
            {
            }

            //! will launch proc(arg1,arg2)
            template <typename PROC, typename ARG1, typename ARG2> inline
            explicit Wire(PROC &proc, ARG1 &arg1, ARG2 &arg2) :
            Nucleus:: WireData(proc,arg1,arg2), Thread(Call2<PROC,ARG1,ARG2>, Args() )
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
