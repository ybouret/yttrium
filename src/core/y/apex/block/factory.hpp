

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

        class Factory : public Singleton<Factory>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime       = AtExit::MaximumLongevity - 27;
            static const unsigned          PossibleShifts = Block::MaxShift-Block::MinShift+1;
            static const unsigned          BytesPerBlocks = sizeof(Blocks);
            static const unsigned          WorkspaceBytes = BytesPerBlocks * PossibleShifts;

            Block * acquire(const unsigned shift);
            Block * acquireBytes(size_t bytes);
            Block * acquire(Random::Bits &ran, const size_t bits);
            void    release(Block * const) noexcept;

            void gc()      noexcept;
            void display() const;



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

