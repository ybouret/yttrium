//! \file

#ifndef Y_Concurrent_Interface_Included
#define Y_Concurrent_Interface_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/container/readable.hpp"

#include "y/object.hpp"
#include "y/counted.hpp"

#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef uint32_t         JobID;

        class Runnable : public Object, public Counted
        {
        public:
            virtual ~Runnable() noexcept {}

            virtual void run(const ThreadContext &) = 0;

        protected:
            explicit Runnable() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Runnable);
        };

        //! function or functionoid, using copy/copy constructor
        template <typename FUNCTION>
        class Callback : public Runnable
        {
        public:
            inline explicit Callback(const FUNCTION &fn) : Runnable(), function(fn) {}
            inline virtual ~Callback() noexcept {}

            inline virtual void run(const ThreadContext &context) { function(context); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback);
            FUNCTION function;
        };

        template <typename OBJECT, typename METHOD>
        class Command : public Runnable
        {
        public:
            inline explicit Command(OBJECT &o, METHOD m) noexcept : Runnable(), host(o), meth(m) {}
            inline virtual ~Command() noexcept {}

            inline virtual void run(const ThreadContext &context) { (host.*meth)(context); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command);
            OBJECT &host;
            METHOD  meth;
        };

        class Task
        {
        public:
            Task(Runnable *userCode) noexcept : code( userCode ) {
                assert(0!=code); // do error
                code->withhold();
            }

            Task(const Task &task) noexcept : code( task.code )
            {
                assert(0!=code);
                code->withhold();
            }


            template <typename FUNCTION> inline
            Task(const FUNCTION &fn) : code( new Callback<FUNCTION>(fn) )
            {
                assert(0!=code);
                code->withhold();
            }

            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT &o, METHOD m) : code( new Command<OBJECT,METHOD>(o,m) )
            {
                assert(0!=code);
                code->withhold();
            }


            ~Task() noexcept {
                assert(0!=code);
                if(code->liberate())
                    delete code;
                code=0;
            }

            void process(const ThreadContext &ctx)
            {
                assert(0!=code);
                code->run(ctx);
            }

        private:
            Y_DISABLE_ASSIGN(Task);
            Runnable *code;
        };



        //______________________________________________________________________
        //
        //
        //
        //! Interface to a Pipeline to process jobs
        //
        //
        //______________________________________________________________________
        class Pipeline : public Readable<const ThreadContext>
        {
        public:
            virtual ~Pipeline() noexcept; //!< cleanup



        protected:
            explicit Pipeline() noexcept; //!< setup
            void     upgrade()  noexcept; //!< jobID++, controlled

            const JobID jobID;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);

            virtual void   suspend() noexcept          = 0; //!< suspend mechanism
            virtual void   restart() noexcept          = 0; //!< restart mechanism
            
        };

    }

}

#endif

