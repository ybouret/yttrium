//! \file

#ifndef Y_Ink_Slabs_Included
#define Y_Ink_Slabs_Included 1

#include "y/concurrent/loop/simt.hpp"
#include "y/ink/slab.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Slabs to setup tessellation for a given area
        //
        //
        //______________________________________________________________________
        class Slabs
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Slabs(const Concurrent::SharedLoop &); //!< setup+memory
            virtual ~Slabs() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! computing slabs
            //__________________________________________________________________
            void split(const Area &area);

            //__________________________________________________________________
            //
            //! proc(slab,target)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP> inline
            void operator()(PROC &proc, PIXMAP &target )
            {
                split(target); simt(proc,target);
            }

            //__________________________________________________________________
            //
            //! proc(slab,target,arg1)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1)
            {
                split(target); simt(proc,target,arg1);
            }
            
            //__________________________________________________________________
            //
            //! proc(slab,target,arg1,arg2)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2)
            {
                split(target); simt(proc,target,arg1,arg2);
            }

            //__________________________________________________________________
            //
            //! proc(slab,target,arg1,arg2,arg3)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2, typename ARG3> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                split(target); simt(proc,target,arg1,arg2,arg3);
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Concurrent::SIMT<Slab> simt; //!< implementation of SIMT
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };
    }

}

#endif

