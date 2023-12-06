
//! \file

#ifndef Y_Concurrent_Engine0D_Included
#define Y_Concurrent_Engine0D_Included 1

#include "y/concurrent/engine/interface.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! No-Mapping class
            //
            //
            //__________________________________________________________________
            class NoMapping
            {
            public:
                explicit NoMapping()                       noexcept; //!< setup false
                explicit NoMapping(const Int2Type<true> &) noexcept; //!< setup true
                virtual ~NoMapping()                       noexcept; //!< cleanup
                void xch(NoMapping &)                      noexcept; //!< no-throw swap
                friend std::ostream & operator<<(std::ostream &, const NoMapping &); //!< display

                const bool activated; //!< helper flag

            private:
                Y_DISABLE_COPY_AND_ASSIGN(NoMapping);
            };
        };

        //______________________________________________________________________
        //
        //
        //
        //! Engine to associate resource to a Thread in a Pipeline
        //
        //
        //______________________________________________________________________
        class Engine0D : public Engine<Nucleus::NoMapping>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Nucleus::NoMapping          Mapping;    //!< alias
            typedef Concurrent::Engine<Mapping> EngineType; //!< alias
            typedef int                         Type;       //!< alias, N/A
            typedef int                         Locus;      //!< alias, N/A

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Engine0D() noexcept; //!< setup deactivated
            virtual ~Engine0D() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! start engine: initiate/activate with no mapping
            void start(const ThreadContext &cntx);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine0D);
        };

    }

}

#endif
