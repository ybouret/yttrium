#include "y/memory/straps.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Memory
    {

        Straps:: ~Straps() noexcept
        {
            std::cerr << "~Straps()" << std::endl;
            while(size>0)
            {
                Strap         *strap = popTail();        assert(0!=strap);
                const unsigned shift = strap->shift__();
                assert(0==strap->next);
                assert(0==strap->prev);
                album[shift].store( Destructed(strap) );
            }
            cache = 0;
            empty = 0;
        }


        const char * const Straps:: CallSign = "Memory::Straps";

        Straps:: Straps(Album &userAlbum) noexcept :
        ListOf<Strap>(),
        hProc( & Straps:: acquireIni ),
        cache( 0 ),
        empty( 0 ),
        album(userAlbum)
        {
            
        }


        void *Straps:: acquire(size_t &blockSize)
        {
            assert(0!=hProc);
            return ((*this).*hProc)(blockSize);
        }

        static inline unsigned ShiftFor(const size_t blockSize)
        {
            return Max(Strap::ShiftToHold(blockSize),Page::DefaultShift);
        }


        static inline Strap *CreateStrapFor(const size_t blockSize, Album &album)
        {
            const unsigned shift = ShiftFor(blockSize);
            Pages         &pages = album[shift];
            void          *addr  = pages.acquire();
            return new (addr) Strap(addr,pages.bytes);
        }

        void *Straps:: acquireIni(size_t &blockSize)
        {
            assert(0==cache);
            assert(0==empty);
            cache   = pushTail(CreateStrapFor(blockSize,album)); // create first strap
            hProc   = & Straps:: acquireAny;                     // change state
            void *p = cache->acquire(blockSize);                 // get first block of a new Strap
            assert(0!=p);
            return p;
        }



        void *Straps:: acquireAny(size_t &blockSize)
        {
            assert(0!=cache);

            //------------------------------------------------------------------
            //
            // checking cache
            //
            //------------------------------------------------------------------
            void *p = cache->acquire(blockSize);
            if(0!=p)
                goto RETURN;


            //------------------------------------------------------------------
            //
            // search with locality
            //
            //------------------------------------------------------------------
            {
                assert(0==p);
                Strap *prev = cache->prev;
                Strap *next = cache->next;

                //--------------------------------------------------------------
                // interleaved search
                //--------------------------------------------------------------
                while(0!=prev && 0 !=next)
                {

                    //----------------------------------------------------------
                    // check prev
                    //----------------------------------------------------------
                    p = prev->acquire(blockSize);
                    if(0!=p)
                    {
                        cache = prev;
                        goto RETURN;
                    }
                    prev = prev->prev;

                    //----------------------------------------------------------
                    // check next
                    //----------------------------------------------------------
                    p = next->acquire(blockSize);
                    if(0!=p)
                    {
                        cache = next;
                        goto RETURN;
                    }
                    next = next->next;
                }

                //--------------------------------------------------------------
                // remaining prev
                //--------------------------------------------------------------
                while(0!=prev)
                {
                    p = prev->acquire(blockSize);
                    if(0!=p)
                    {
                        cache = prev;
                        goto RETURN;
                    }
                    prev = prev->prev;
                }

                //--------------------------------------------------------------
                // remaining next
                //--------------------------------------------------------------
                while(0!=next)
                {
                    p = next->acquire(blockSize);
                    if(0!=p)
                    {
                        cache = next;
                        goto RETURN;
                    }
                    next = next->next;
                }
            }

            //------------------------------------------------------------------
            //
            // need a new Strap!
            //
            //------------------------------------------------------------------
            assert(0==p);
            assert(0!=cache);
            {
                cache   = insertByIncreasingAddress(CreateStrapFor(blockSize,album)); // create new strap
                void *p = cache->acquire(blockSize);                                  // get first block of a new Strap
                assert(0!=p);
                assert(cache!=empty);
                return p;
            }

        RETURN:
            assert(0!=p);
            assert(0!=cache);
            if(cache==empty) empty = 0;
            return p;

        }

        void  Straps:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            Strap *strap = Strap::Release(blockAddr);
            
        }


    }
}
