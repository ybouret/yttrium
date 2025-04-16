
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

            //__________________________________________________________________
            //
            //
            //
            //! walking and processing nodes
            //
            //
            //__________________________________________________________________
            class XWalk
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef TL2(XNode &,size_t)  Args; //!< alias
                typedef Functor<void,Args>   Proc; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit XWalk(const bool verbosity=false); //!< setup
                virtual ~XWalk() noexcept;                  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! walk down the tree
                void operator()(XNode &root) const;


                //! callback for node name
                template <typename NAME,
                typename HOST,
                typename METH> inline
                void on(const NAME &name, HOST &host, METH meth)
                {
                    const Proc _(&host,meth);
                    record(name,_);
                }

                //! callback for node name
                template <typename NAME> inline
                void on(const NAME &name, void (*proc)(XNode &,size_t))
                {
                    const Proc _(proc);
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
                
            public:
                bool &verbose; //!< verbosity

            };
        }
    }

}

#endif

