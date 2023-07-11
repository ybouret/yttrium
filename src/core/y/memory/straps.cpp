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
            while(size>0)
            {
                Strap         *strap = popTail(); assert(0!=strap);
                const unsigned shift = strap->myShift();
                assert(0==strap->next);
                assert(0==strap->prev);
                album[shift].store( Destructed(strap) );
            }
            cache = 0;
            empty = 0;
            eshft = 0;
        }


        const char * const Straps:: CallSign = "Memory::Straps";

        Straps:: Straps(Album &userAlbum) noexcept :
        ListOf<Strap>(),
        build( & Straps:: acquireIni ),
        cache( 0 ),
        empty( 0 ),
        eshft( 0 ),
        album(userAlbum)
        {
            
        }


        void *Straps:: acquire(size_t &blockSize)
        {
            assert(0!=build);
            return ((*this).*build)(blockSize);
        }

        //! get minimal shift to acquire blockSize in a new strap
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
            build   = & Straps:: acquireAny;                     // change state
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
            if(cache==empty)
            {
                std::cerr << "cleaning empty!" << std::endl;
                assert(eshft>0);
                empty = 0;
                eshft = 0;
            }
            return p;

        }

        void  SetupToRelease(Strap * &strap, unsigned  &shift,
                             Strap * &empty, unsigned  &eshft) noexcept
        {
            assert(0!=empty);
            assert(0!=strap);
            assert(strap!=empty);
            if(shift<eshft)
            {
                return;
            }
            else
            {
                if(eshft<shift)
                {
                    Swap(strap,empty);
                    Swap(shift,eshft);
                    return;
                }
                else
                {
                    assert(eshft==shift);
                    if(strap<empty)
                    {
                        return;
                    }
                    else
                    {
                        Swap(strap,empty);
                        Swap(shift,eshft);
                        return;
                    }
                }
            }

        }

        void  Straps:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            Strap *strap = Strap::Release(blockAddr);
            if(strap->isEmpty())
            {
                if(0!=empty)
                {
                    //----------------------------------------------------------
                    // detected a second empty strap
                    //----------------------------------------------------------
                    assert(eshft>0);           // previously computed
                    assert(empty->isEmpty());  // impossible
                    assert(strap!=empty);      // impossible

                    //----------------------------------------------------------
                    // select strap/shift to release
                    //----------------------------------------------------------
                    unsigned shift = strap->myShift();
                    SetupToRelease(strap,shift,empty,eshft);
                    assert(shift<eshft||strap<empty);
                    assert(empty->isEmpty());
                    assert(empty->myShift() == eshft);

                    //----------------------------------------------------------
                    // release strap. empty/eshft is already set
                    //----------------------------------------------------------
                    album[shift].store( Destructed( pop(strap) ) );

                }
                else
                {
                    //----------------------------------------------------------
                    // set as first empty strap
                    //----------------------------------------------------------
                    empty = strap;
                    eshft = strap->myShift();
                }
            }
        }

        void Straps:: displayInfo(const size_t indent) const
        {
            Core::Indent(std::cerr,indent) << "<" << CallSign << " straps='" << size << "'>" << std::endl;
            for(const Strap *strap = head; strap; strap=strap->next)
            {
                Core::Indent(std::cerr,indent+1) << "@" << strap << " : 2^" << strap->myShift() << std::endl;
            }
            Core::Indent(std::cerr,indent) << "<" << CallSign << "/>" << std::endl;
        }

    }
}
