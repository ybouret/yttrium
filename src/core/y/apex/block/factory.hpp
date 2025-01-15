

//! \file

#ifndef Y_Apex_Block_Factory_Included
#define Y_Apex_Block_Factory_Included 1

#include "y/apex/block/blocks.hpp"
#include "y/singleton.hpp"
#include "y/random/bits.hpp"
#include "y/concurrent/mutex/qpool.hpp"

namespace Yttrium
{
    namespace Apex
    {
        typedef Concurrent::Mutex Mutex;

        //______________________________________________________________________
        //
        //
        //
        //! Global cache of blocks
        //
        //
        //______________________________________________________________________
        class Factory : public Singleton<Factory>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                           //!< "Apex::Factory"
            static const AtExit::Longevity LifeTime       = AtExit::MaximumLongevity - 27;     //!< alias
            static const unsigned          PossibleShifts = Block::MaxShift-Block::MinShift+1; //!< alias
            static const unsigned          BytesPerBlocks = sizeof(Blocks);                    //!< alias
            static const unsigned          WorkspaceBytes = BytesPerBlocks * PossibleShifts;   //!< linear memory

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Block * query(const unsigned shift);                 //!< query for a given shift
            Block * queryBytes(size_t bytes);                    //!< acquire for given bytes
            Block * query(Random::Bits &ran, const size_t bits); //!< exactly bits
            Block * duplicate(const Block * const);                //!< duplicate block
            void    store(Block * const)  noexcept;              //!< release in cache
            void    gc(const size_t cycles) noexcept;              //!< gc on each shift + mutexes
            void    display() const;                               //!< display status




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Factory);
            friend class Singleton<Factory>;
            explicit Factory() noexcept;
            virtual ~Factory() noexcept;

            Blocks * const         blocks;
            Concurrent::QMutexPool mutexes;
            
            void   *       wksp[Y_WORDS_GEQ(WorkspaceBytes)];


        };

    }

}

#endif

