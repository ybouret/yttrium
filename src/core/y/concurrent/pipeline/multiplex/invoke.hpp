
//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Invoke_Included
#define Y_Concurrent_Pipeline_Multiplex_Invoke_Included 1

#include "y/concurrent/pipeline/multiplex.hpp"
#include "y/concurrent/pipeline/runnable.hpp"
#include "y/type/binder.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <typename ENGINE, typename TLIST>
        struct Invoke
        {
            typedef Writable<ENGINE> Engines;
            Y_BINDER_ECHO(TLIST);

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth)
            {
                return plex->run( new Job<METHOD>(plex,meth) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1)
            {
                return plex->run( new Job<METHOD>(plex,meth,arg1) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2)
            {
                return plex->run( new Job<METHOD>(plex,meth,arg1,arg2) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3)
            {
                return plex->run( new Job<METHOD>(plex,meth,arg1,arg2,arg3) );
            }


            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3,
                        Param4             arg4)
            {
                return plex->run( new Job<METHOD>(plex,meth,arg1,arg2,arg3,arg4) );
            }


            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3,
                        Param4             arg4,
                        Param5             arg5)
            {
                return plex->run( new Job<METHOD>(plex,meth,arg1,arg2,arg3,arg4,arg5) );
            }

        private:
            template <typename METHOD>
            class Job : public Runnable, public Binder<TLIST>
            {
            public:
                Y_BINDER_ARGS(TLIST);
                inline virtual ~Job() noexcept {}

                inline explicit Job(Engines &eng,
                                    METHOD   mth) noexcept :
                Runnable(), Binder<TLIST>(),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1) noexcept :
                Runnable(), Binder<TLIST>(p1),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2) noexcept :
                Runnable(), Binder<TLIST>(p1,p2),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3,
                                    Param4   p4) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3,p4),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3,
                                    Param4   p4,
                                    Param5   p5) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3,p4,p5),
                plex(eng),
                meth(mth)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Job);
                Engines &plex;
                METHOD   meth;

                //! interface
                virtual void run(const ThreadContext &ctx)
                {
                    static const typename Binder<TLIST>::ArgsType params =  {};
                    perform(plex[ctx.indx],params);
                }

                inline void perform(ENGINE &host, const Int2Type<0> &) { (host.*meth)(); }
                inline void perform(ENGINE &host, const Int2Type<1> &) { (host.*meth)(arg1); }
                inline void perform(ENGINE &host, const Int2Type<2> &) { (host.*meth)(arg1,arg2); }
                inline void perform(ENGINE &host, const Int2Type<3> &) { (host.*meth)(arg1,arg2,arg3); }
                inline void perform(ENGINE &host, const Int2Type<4> &) { (host.*meth)(arg1,arg2,arg3,arg4); }
                inline void perform(ENGINE &host, const Int2Type<5> &) { (host.*meth)(arg1,arg2,arg3,arg3,arg5); }

            };
        };

    }

}

#endif

