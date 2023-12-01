
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/ostream-proto.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        namespace Nucleus
        {
            class SIMD
            {
            public:
                virtual ~SIMD() noexcept {}

            protected:
                explicit SIMD(const SharedLoop &team) noexcept : loop(team) {}
                explicit SIMD(Loop             *team) noexcept : loop(team) {}

                SharedLoop loop;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            };
        }

        template <typename T>
        class SIMD : public Nucleus::SIMD, public CxxArray< const ForLoop<T> >
        {
        public:
            typedef CxxArray< const ForLoop<T> > Ranges;

            inline explicit SIMD(const SharedLoop &team) :
            Nucleus::SIMD(team), Ranges(loop->size())
            {
            }

            inline explicit SIMD(Loop *team) :
            Nucleus::SIMD(team), Ranges(loop->size())
            {
            }

            inline void dispatch(const T head, const T tail, const T step)
            {
                const Readable<const ThreadContext> &cntx = *loop;
                Writable<const ForLoop<T> >         &self = *this; assert(self.size()==cntx.size());
                for(size_t i=cntx.size();i>0;--i)
                {
                    ForLoop<T> trek = Split::For(cntx[i],head,tail,step);
                    Coerce(self[i]).swapWith(trek);
                }
            }

            inline void operator()(void) {
                const CallMe call = { *this };
                (*loop)(call);
            }





        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);

            struct CallMe
            {
                const Ranges &self;
                void operator()(const ThreadContext &ctx) const
                {
                    const ForLoop<T> &trek = self[ctx.indx];
                    Y_LOCK(ctx.sync);
                    std::cerr << "SIMD @" << trek << std::endl;
                }
            };


        };

#if 0

        //______________________________________________________________________
        //
        //
        //! Range of 1D indices to perform operations
        //
        //______________________________________________________________________
        class Range
        {
        public:
            explicit Range() noexcept; //!< setup zero
            virtual ~Range() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Range);

            const size_t length; //!< number of indices
            const size_t offset; //!< first index

            template <typename SEQUENCE> inline
            bool matches(const SEQUENCE &seq) const noexcept
            {
                return 1 == offset && seq.size() == length;
            }



        private:
            Y_DISABLE_ASSIGN(Range);
        };

        typedef Readable<const Range> Ranges;

        //______________________________________________________________________
        //
        //
        //
        //! SIMD for 1D operations
        //
        //
        //______________________________________________________________________
        class SIMD : public Ranges, public Range
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "SIMD"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SIMD(const SharedLoop &); //!< setup from shared loop
            explicit SIMD(Loop             *); //!< setup from new loop
            virtual ~SIMD()          noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! setup ranges
            /**
             \param dataLength global data length
             \param dataOffset global data offset
             */
            void dispatch(const size_t dataLength, const size_t dataOffset=1) noexcept;

            //! test call
            void operator()(void) noexcept;

            
            //! proc(range,args)
            template <typename PROC, typename ARGS> inline
            void operator()(PROC &proc, ARGS &args)
            {
                Call1<PROC,ARGS> call = { *this, proc, args };
                (*loop)(call);
            }

            //! proc(range,arg1,arg2)
            template <typename PROC, typename ARG1, typename ARG2> inline
            void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2)
            {
                Call2<PROC,ARG1,ARG2> call = { *this, proc, arg1, arg2 };
                (*loop)(call);
            }

            //! proc(range,arg1,arg2,arg3)
            template <typename PROC, typename ARG1, typename ARG2, typename ARG3> inline
            void operator()(PROC &proc, ARG1 &arg1, ARG2 &arg2, ARG3 &arg3)
            {
                Call3<PROC,ARG1,ARG2,ARG3> call = { *this, proc, arg1, arg2, arg3 };
                (*loop)(call);
            }




            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       size()                   const noexcept; //!< loop->size()
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual const Range &operator[](const size_t) const noexcept; //!< in [1..size()]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            class Code;
            SharedLoop  loop;
            Code       *code;

            struct CallMe {
                const Ranges &ranges;
                void operator()(const ThreadContext &ctx) const;
            };


            template <typename PROC, typename ARGS>
            struct Call1 {
                const Ranges &ranges;
                PROC         &proc;
                ARGS         &args;
                inline void operator()(const ThreadContext &ctx) { proc(ranges[ctx.indx],args); }
            };

            template <typename PROC, typename ARG1, typename ARG2>
            struct Call2 {
                const Ranges &ranges;
                PROC         &proc;
                ARG1         &arg1;
                ARG2         &arg2;
                inline void operator()(const ThreadContext &ctx) { proc(ranges[ctx.indx],arg1,arg2); }
            };


            template <typename PROC, typename ARG1, typename ARG2, typename ARG3>
            struct Call3 {
                const Ranges &ranges;
                PROC         &proc;
                ARG1         &arg1;
                ARG2         &arg2;
                ARG3         &arg3;
                inline void operator()(const ThreadContext &ctx) { proc(ranges[ctx.indx],arg1,arg2,arg3); }
            };

            
        };

#endif

    }

}

#endif
