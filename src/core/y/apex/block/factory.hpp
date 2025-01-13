

//! \file

#ifndef Y_Apex_Block_Factory_Included
#define Y_Apex_Block_Factory_Included 1

#include "y/apex/block/blocks.hpp"
#include "y/singleton.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Apex
    {
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
            Block * acquire(const unsigned shift);                 //!< acquire for a given shift
            Block * acquireBytes(size_t bytes);                    //!< acquire for given bytes
            Block * acquire(Random::Bits &ran, const size_t bits); //!< exactly bits
            void    release(Block * const) noexcept;               //!< release in cache
            void gc()      noexcept; //!< gc on each shift
            void display() const;    //!< display status



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Factory);
            friend class Singleton<Factory>;
            explicit Factory() noexcept;
            virtual ~Factory() noexcept;

            Blocks * const blocks;
            void   *       wksp[Y_WORDS_GEQ(WorkspaceBytes)];


        };

    }

}

#endif

