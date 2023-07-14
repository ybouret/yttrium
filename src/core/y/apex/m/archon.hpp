
//! \file


#ifndef Y_Apex_Archon_Included
#define Y_Apex_Archon_Included 1

#include "y/singleton.hpp"
#include "y/calculus/base2.hpp"
#include "y/type/utils.hpp"

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


            template <typename WORD>
            class Block
            {
            public:
                static const size_t WordSize = sizeof(WORD);

                explicit Block(size_t numWords) :
                entry(0),
                words(0),
                bytes( Max(numWords*WordSize,WordSize) ),
                shift( ShiftFor(Coerce(bytes))         )
                {
                    static Archon &     mgr = Archon::Instance();
                    static const size_t one = 1;
                    try {
                        Coerce(entry) = static_cast<WORD *>( mgr.acquire( Coerce(shift)) );
                        Coerce(bytes) = one << shift;
                        Coerce(words) = bytes / WordSize;
                    }
                    catch(...) { hardReset(); throw; }
                }


                virtual ~Block() noexcept {
                    static Archon &mgr = Archon::Location();
                    assert(0!=entry);
                    assert(bytes>=words*WordSize);
                    assert( (size_t(1)<<shift) == bytes );
                    mgr.release(entry,shift);
                    hardReset();
                }

                WORD * const   entry;
                const size_t   words;
                const size_t   bytes;
                const unsigned shift;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Block);
                inline void hardReset() noexcept {
                    Coerce(entry) = 0;
                    Coerce(words) = 0;
                    Coerce(bytes) = 0;
                    Coerce(shift) = 0;
                }
            };


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Archon);
            explicit Archon() noexcept;
            virtual ~Archon() noexcept;
            friend class Singleton<Archon>;
            static void  CheckRequired(size_t       &bytes,
                                       unsigned     &shift);
            static unsigned ShiftFor(size_t       &bytes);

        };
    }

}

#endif
