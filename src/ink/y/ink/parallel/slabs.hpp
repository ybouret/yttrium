//! \file

#ifndef Y_Ink_Slabs_Included
#define Y_Ink_Slabs_Included 1

#include "y/concurrent/defs.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/ink/parallel/slab.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Slabs to setup tessellation for a given area
        /**
         - internal SIMT can be directly use
         - or use API to use with Pixmap as first argument
         */
        //
        //______________________________________________________________________
        class Slabs : public Writable<Slab>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Slabs(const Concurrent::SharedLoop &); //!< setup+memory
            Slabs(const Sequential_ &);            //!< use sequential loop
            Slabs(const InParallel_ &);            //!< use parallel loop
            virtual ~Slabs() noexcept;             //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual size_t       size()                   const noexcept; //!< simt.size()
            virtual Slab       & operator[](const size_t)       noexcept; //!< access
            virtual const Slab & operator[](const size_t) const noexcept; //!< access
            virtual const char * callSign()               const noexcept; //!< "Ink::Slabs"

            //__________________________________________________________________
            //
            //! computing slabs
            //__________________________________________________________________
            void split(const Area &area); //!< prepare slabs for a given area

            //__________________________________________________________________
            //
            //! proc(slab,target)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP> inline
            void operator()(PROC &proc, PIXMAP &target ) {
                split(target); simt(proc,target);
            }

            //__________________________________________________________________
            //
            //! proc(slab,target,arg1)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1) {
                split(target); simt(proc,target,arg1);
            }
            
            //__________________________________________________________________
            //
            //! proc(slab,target,arg1,arg2)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2) {
                split(target); simt(proc,target,arg1,arg2);
            }

            //__________________________________________________________________
            //
            //! proc(slab,target,arg1,arg2,arg3)
            //__________________________________________________________________
            template <typename PROC, typename PIXMAP, typename ARG1, typename ARG2, typename ARG3> inline
            void operator()(PROC &proc, PIXMAP &target, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3) {
                split(target); simt(proc,target,arg1,arg2,arg3);
            }

            //__________________________________________________________________
            //
            //! retrieve local min/max after local scanMinMax of each slab
            //__________________________________________________________________
            template <typename T> inline
            void getMinMax(T &tmin, T &tmax) const noexcept
            {
                tmin = tmax = 0;
                bool first = true;
                for(size_t i = simt.size();i>0;--i)
                {
                    const Slab &slab = simt[i];
                    if(slab.count()<=0) continue;;
                    const T * const data = slab.as<T>(2);
                    if(first)
                    {
                        tmin = data[0];
                        tmax = data[1];
                        first = false;
                    }
                    else
                    {
                        tmin = Min(tmin,data[0]);
                        tmax = Max(tmax,data[1]);
                    }
                }

            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        private:
            const Concurrent::SharedLoop mill;
        public:
            Concurrent::SIMT<Slab>       simt; //!< implementation of SIMT

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };
    }

}

#endif

