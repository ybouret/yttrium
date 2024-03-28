//! \file


#ifndef Y_Concurrent_SIMT_Execute_Included
#define Y_Concurrent_SIMT_Execute_Included 1

#include "y/concurrent/loop/simt.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        

#define Y_CONCURRENT_EXEC_DECL(I)                                       \
typedef typename TL::SafeTypeAt<TLIST,I-1,EmptyType>::Result Type##I;   \
typedef typename TypeTraits<Type##I>::ParamType              Param##I;  \
typedef typename TypeTraits<Param##I>::ReferenceType         Ref##I

        template <typename ENGINE,typename TLIST>
        struct Execute
        {
            typedef Writable<ENGINE> Engines; //!< alias
            Y_CONCURRENT_EXEC_DECL(1);
            Y_CONCURRENT_EXEC_DECL(2);
            Y_CONCURRENT_EXEC_DECL(3);
            Y_CONCURRENT_EXEC_DECL(4);
            Y_CONCURRENT_EXEC_DECL(5);

            template <typename METHOD>
            class ModKernel : public Kernel
            {
            public:
                inline virtual ~ModKernel() noexcept {}

            protected:
                inline explicit ModKernel(Engines &eng, METHOD &mth) noexcept :
                method(mth),
                engines(eng)
                {
                }

                inline ENGINE & host(const Context &ctx) noexcept {
                    return engines[ctx.indx];
                }

                METHOD const method;
            private:
                Engines &engines;
                Y_DISABLE_COPY_AND_ASSIGN(ModKernel);
            };


            //__________________________________________________________________
            //
            //! ENGINE.meth()
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth)
            {
                class Call : public ModKernel<METHOD>  {
                public:
                    using ModKernel<METHOD>::host;
                    using ModKernel<METHOD>::method;

                    inline virtual ~Call() noexcept {}
                    inline explicit Call(Engines &eng, METHOD  &mth) noexcept : ModKernel<METHOD> (eng,mth) {}

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        (host(ctx).*method)();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                };

                Call me(simt,meth);
                simt->run1(me);
            }

            //__________________________________________________________________
            //
            //! ENGINE.meth(arg1)
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth,
                    Param1        p1)
            {
                class Call : public ModKernel<METHOD>  {
                public:
                    using ModKernel<METHOD>::host;
                    using ModKernel<METHOD>::method;

                    inline virtual ~Call() noexcept {}
                    inline explicit Call(Engines &eng, METHOD  &mth, Ref1 p1) noexcept :
                    ModKernel<METHOD> (eng,mth), arg1(p1) {}


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                    Ref1     arg1;
                    
                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        (host(ctx).*method)(arg1);
                    }
                };

                Call me(simt,meth,p1);
                simt->run1(me);
            }

            //__________________________________________________________________
            //
            //! ENGINE.meth(arg1,arg2)
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth,
                    Param1        p1,
                    Param2        p2)
            {
                class Call : public ModKernel<METHOD>  {
                public:
                    using ModKernel<METHOD>::host;
                    using ModKernel<METHOD>::method;

                    inline virtual ~Call() noexcept {}
                    inline explicit Call(Engines &eng, METHOD  &mth, Ref1 p1, Ref2 p2) noexcept :
                    ModKernel<METHOD> (eng,mth), arg1(p1), arg2(p2) {}


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                    Ref1     arg1;
                    Ref2     arg2;

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        (host(ctx).*method)(arg1,arg2);
                    }
                };

                Call me(simt,meth,p1,p2);
                simt->run1(me);
            }

            //__________________________________________________________________
            //
            //! ENGINE.meth(arg1,arg2,arg3)
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth,
                    Param1        p1,
                    Param2        p2,
                    Param3        p3)
            {
                class Call : public ModKernel<METHOD>  {
                public:
                    using ModKernel<METHOD>::host;
                    using ModKernel<METHOD>::method;

                    inline virtual ~Call() noexcept {}
                    inline explicit Call(Engines &eng, METHOD  &mth, Ref1 p1, Ref2 p2, Ref3 p3) noexcept :
                    ModKernel<METHOD> (eng,mth), arg1(p1), arg2(p2), arg3(p3){}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                    Ref1     arg1;
                    Ref2     arg2;
                    Ref3     arg3;

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        (host(ctx).*method)(arg1,arg2,arg3);
                    }
                };

                Call me(simt,meth,p1,p2,p3);
                simt->run1(me);
            }




        };

    }

}

#endif
