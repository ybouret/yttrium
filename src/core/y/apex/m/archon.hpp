
//! \file


#ifndef Y_Apex_Archon_Included
#define Y_Apex_Archon_Included 1

#include "y/singleton.hpp"

namespace Yttrium
{
    namespace Apex
    {


        //______________________________________________________________________
        //
        //
        //
        //! singleton for cache of memory blocks
        //
        //
        //______________________________________________________________________
        class Archon : public Singleton<Archon>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                //!< "Apex::Archon"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 5; //!< alias
            class Engine;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void * acquire(unsigned &shift);                            //!< fetch block 2^(MaxOf(shift,MinShift)), shift<=MaxShift
            void   release(void *entry, const unsigned shift) noexcept; //!< store previously acquire blocks

            template <typename WORD> inline
            WORD *acquire(size_t   &words,
                          size_t   &bytes,
                          unsigned &shift)
            {
                assert(0==bytes);
                assert(0==shift);
                CheckRequired( bytes = words * sizeof(WORD), shift);
                try {
                    void *block = acquire(shift);
                    words       = bytes/sizeof(WORD);
                    return block;
                }
                catch(...)
                {
                    words = 0;
                    bytes = 0;
                    shift = 0;
                    throw;
                }
            }

            template <typename WORD> inline
            void release(WORD *   &block,
                         size_t   &words,
                         size_t   &bytes,
                         unsigned &shift)
            {
                assert(0!=block);
                assert( (size_t(1)<<shift) == bytes);
                assert( bytes >= words * sizeof(WORD) );
                release(block,shift);
                block = 0;
                words = 0;
                bytes = 0;
                shift = 0;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Archon);
            explicit Archon() noexcept;
            virtual ~Archon() noexcept;
            friend class Singleton<Archon>;
            static void  CheckRequired(size_t       &bytes,
                                       unsigned     &shift);

        };
    }

}

#endif
