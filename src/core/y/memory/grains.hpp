//! \file


#ifndef Y_Memory_Grains_Included
#define Y_Memory_Grains_Included 1

#include "y/singleton.hpp"



namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Allocator of Quanta
        //
        //
        //______________________________________________________________________
        class Grains : public Singleton<Grains,NucleusSingleton>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign; //!< "Memory::Grains"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 4; //!< longevity
            

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void *acquire(size_t blockSize);               //!< acquire blockSize
            void  release(void *, const size_t) noexcept;  //!< release previously acquired

        private:
            explicit  Grains();
            virtual  ~Grains() noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Grains);
            friend class Singleton<Grains,NucleusSingleton>;
        };

    }

}

#endif

