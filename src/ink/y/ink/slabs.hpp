
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
            explicit Slabs(const Concurrent::SharedLoop &);
            virtual ~Slabs() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! computing slabs
            void split(const Area &area);

            template <typename PROC, typename PIXMAP> inline
            void operator()(PROC &proc, PIXMAP &target )
            {
                split(target); simt(proc,target);
            }

            template <typename PROC, typename PIXMAP, typename ARG1> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1)
            {
                split(target); simt(proc,target,arg1);
            }
            
            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2)
            {
                split(target); simt(proc,target,arg1,arg2);
            }

            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2, typename ARG3> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                split(target); simt(proc,target,arg1,arg2,arg3);
            }




            Concurrent::SIMT<Slab> simt;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };
    }

}

#endif

