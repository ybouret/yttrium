
//! \file

#ifndef Y_Lingo_Syntax_XWalk_Included
#define Y_Lingo_Syntax_XWalk_Included 1

#include "y/lingo/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Syntax
        {



            class XWalk
            {
            public:
                enum Result
                {
                    Stop,
                    Dive
                };
                typedef TL2(XNode &,size_t)  Args;
                typedef Functor<Result,Args> Proc;

                explicit XWalk();
                virtual ~XWalk() noexcept;

                void operator()(XNode &root) const;

                template <typename NAME,
                typename HOST,
                typename METH> inline
                void on(const NAME &name, HOST &host, METH meth)
                {
                    const Proc _(&host,meth);
                    record(name,_);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(XWalk);
                class Code;
                Code *code;
                void walk(XNode * const node,
                          size_t        deep) const;

                void record(const String &, const Proc &);
                void record(const char * const, const Proc &);

            };
        }
    }

}

#endif

