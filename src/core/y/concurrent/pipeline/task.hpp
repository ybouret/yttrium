//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1


#include "y/concurrent/pipeline/callback/0.hpp"
#include "y/concurrent/pipeline/callback/1.hpp"
#include "y/concurrent/pipeline/callback/2.hpp"
#include "y/concurrent/pipeline/callback/3.hpp"

#include "y/concurrent/pipeline/command/0.hpp"
#include "y/concurrent/pipeline/command/1.hpp"
#include "y/concurrent/pipeline/command/2.hpp"
#include "y/concurrent/pipeline/command/3.hpp"

#include "y/type/ints.hpp"
#include "y/config/shallow.hpp"


namespace Yttrium
{
    Y_SHALLOW_DECL(Functionoid);
    Y_SHALLOW_DECL(CxxMethodOf);

    namespace Concurrent
    {



        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate all different Runnable for Pipeline
        //
        //
        //______________________________________________________________________
        class Task
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef UnsignedInt<sizeof(void*)>::Type ID; //!< identifier
           
            //! status of the task
            enum Status
            {
                Success, //!< done
                Pending, //!< pending
                Running  //!< running
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Task(Runnable *) noexcept; //!< setup from user's runnable code
            Task(const Task &) noexcept; //!< shared copy
            ~Task()            noexcept; //!< cleanup

            // no args

            //! create from function/functionoid (full copy)
            template <typename FUNCTION> inline
            Task( const FUNCTION     &fn) :
            code( new Callback0<FUNCTION>(fn) )
            { initialize(); }

            //! create from object+method
            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT             &o,
                 METHOD              m) :
            code( new Command0<OBJECT,METHOD>(o,m) )
            { initialize(); }


            //! create for functionoid(context,arg1)
            template <typename FUNCTION, typename ARG1> inline
            Task(const Functionoid_ &, const FUNCTION &fn, ARG1 &a1) :
            code( new Callback1<FUNCTION,ARG1>(fn,a1) )
            {

            }

            //! create for functionoid(context,arg1,arg2)
            template <typename FUNCTION, typename ARG1, typename ARG2>   inline
            Task( const Functionoid_ &, const FUNCTION &fn, ARG1 &a1, ARG2 &a2) :
            code( new Callback2<FUNCTION,ARG1,ARG2>(fn,a1,a2) )
            {

            }

            //! create for functionoid(context,arg1,arg2,arg3)
            template <typename FUNCTION, typename ARG1, typename ARG2, typename ARG3> inline
            Task( const Functionoid_ &, const FUNCTION &fn, ARG1 &a1, ARG2 &a2, ARG3 &a3) :
            code( new Callback3<FUNCTION,ARG1,ARG2,ARG3>(fn,a1,a2,a3) )
            {

            }

            //! create for host.meth(context,arg1)
            template <typename OBJECT, typename METHOD, typename ARG1> inline
            Task(const CxxMethodOf_ &,
                 OBJECT             &host,
                 METHOD              meth,
                 ARG1               &arg1) :
            code( new Command1<OBJECT,METHOD,ARG1>(host,meth,arg1) )
            {

            }

            //! create for host.meth(context,arg1,arg2)
            template <typename OBJECT, typename METHOD, typename ARG1, typename ARG2> inline
            Task(const CxxMethodOf_ &,
                 OBJECT             &host,
                 METHOD              meth,
                 ARG1               &arg1,
                 ARG2               &arg2) :
            code( new Command2<OBJECT,METHOD,ARG1,ARG2>(host,meth,arg1,arg2) )
            {

            }

            //! create for host.meth(context,arg1,arg2,arg3)
            template <typename OBJECT, typename METHOD, typename ARG1, typename ARG2, typename ARG3> inline
            Task(const CxxMethodOf_ &,
                 OBJECT             &host,
                 METHOD              meth,
                 ARG1               &arg1,
                 ARG2               &arg2,
                 ARG3               &arg3) :
            code( new Command3<OBJECT,METHOD,ARG1,ARG2,ARG2>(host,meth,arg1,arg2,arg3) )
            {

            }



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

            
        private:
            Y_DISABLE_ASSIGN(Task);
            void initialize() noexcept;
            Runnable *code;
        };

    }

}

#endif
