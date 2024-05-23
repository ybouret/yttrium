#include "y/utest/run.hpp"

#include "y/singleton.hpp"
#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/album.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/text/hexadecimal.hpp"

#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Archon);
            explicit Archon() noexcept;
            virtual ~Archon() noexcept;
            friend class Singleton<Archon>;


        };


        const char * const Archon:: CallSign = "Apex::Archon";

        namespace
        {
            class CoreEngine
            {
            public:
                inline   CoreEngine() : album(), corpus(album) { }
                virtual ~CoreEngine() noexcept {}

                Memory::Album  album;
                Memory::Corpus corpus;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(CoreEngine);
            };
        }

        class Archon:: Engine : public CoreEngine, public Memory::Quarry
        {
        public:
            explicit Engine() : CoreEngine(), Memory::Quarry(corpus)
            {
            }
            virtual ~Engine() noexcept {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };



        static Archon::Engine *engine = 0;
        static void           *engine_[ Y_WORDS_FOR(Archon::Engine) ];


        Archon:: ~Archon() noexcept
        {
            assert(0!=engine);
            Destruct(engine);
            engine = 0;
            Y_STATIC_ZARR(engine_);
        }

        Archon:: Archon() noexcept : Singleton<Archon>()
        {
            assert(0==engine);
            engine = new ( Y_STATIC_ZARR(engine_) ) Engine();
        }

        void * Archon:: acquire(unsigned &shift)
        {
            assert(0!=engine);
            Y_LOCK(access);
            return engine->acquire(shift);
        }

        void Archon:: release(void *entry, const unsigned shift) noexcept
        {
            assert(0!=engine);
            assert(0!=entry);
            Y_LOCK(access);
            engine->release(entry,shift);
        }

        static inline void * AcquireLinear(unsigned &shift)
        {
            static Archon &archon = Archon::Instance();
            return archon.acquire(shift);
        }

        class Grades
        {
        public:
            static const size_t MinBytes = sizeof(uint64_t);

        protected:
            explicit Grades(const size_t required)   :
            bits(0),
            bytes(0),
            maxBytes( MaxBytesFor(required) ),
            byte(0),
            shift(0),
            entry(0)
            {
            }

        public:
            virtual ~Grades() noexcept {
                static Archon &archon = Archon::Location();
                archon.release(entry,shift);
            }

            static inline size_t MaxBytesFor(const size_t required)
            {
                if(required>=Base2<size_t>::MaxPowerOfTwo-1) throw Specific::Exception(Archon::CallSign, "required overflow");
                return NextPowerOfTwo( Max(required,MinBytes) );
            }

            size_t          bits;
            size_t          bytes;
            const size_t    maxBytes;
            uint8_t * const byte;

        protected:
            const unsigned  shift;
            void * const    entry;

        private:
            Y_DISABLE_ASSIGN(Grades);
        };


        template <typename T>
        static inline Hexadecimal ToHex(const T &x)
        {
            return Hexadecimal(x);
        }

        template <typename WORD_TYPE>
        class Element : public Grades
        {
        public:
            typedef WORD_TYPE WordType;


            explicit Element(const size_t required) : 
            Grades(required),
            words(0),
            maxWords(maxBytes/sizeof(WordType)),
            word(0)
            {
                assert( IsPowerOfTwo(maxBytes) );
                assert( IsPowerOfTwo(maxWords) );
                assert( sizeof(WordType) * maxWords == maxBytes );
                assert(maxBytes< Base2<size_t>::MaxPowerOfTwo );
                Coerce(entry) = AcquireLinear( Coerce(shift) = Base2<size_t>::Log(maxWords) + 1 );
                Coerce(byte)  = static_cast<uint8_t *>(entry);
                Coerce(word)  = (WordType *)(byte+maxBytes);
            }

            virtual ~Element() noexcept 
            {

            }

            size_t           words;
            const size_t     maxWords;
            WordType * const word;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Element);
        };

        template <>
        class Element<uint8_t> : public Grades
        {
        public:
            typedef uint8_t WordType;

            explicit Element(const size_t required) :
            Grades(required),
            words(bytes),
            maxWords(maxBytes),
            word(byte)
            {
                assert( IsPowerOfTwo(maxBytes) );
                assert( IsPowerOfTwo(maxWords) );
                assert( maxBytes == maxWords   );
                Coerce(entry) = AcquireLinear( Coerce(shift) = Base2<size_t>::Log(maxWords) );
                Coerce(byte)  = static_cast<uint8_t *>(entry);

            }

            virtual ~Element() noexcept {}

            const size_t      &words;
            const size_t      &maxWords;
            WordType * const  &word;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Element);
        };




    }
}

Y_UTEST(apk_n)
{

    for(size_t i=0;i<=20;++i)
    {
        std::cerr << i << std::endl;

        { APK::Element<uint8_t>  el(i); std::cerr << "maxBytes=" << el.maxBytes << ", maxWords=" << el.maxWords << std::endl; }
        { APK::Element<uint16_t> el(i); std::cerr << "maxBytes=" << el.maxBytes << ", maxWords=" << el.maxWords << std::endl; }
        { APK::Element<uint32_t> el(i); std::cerr << "maxBytes=" << el.maxBytes << ", maxWords=" << el.maxWords << std::endl; }


    }

}
Y_UDONE()

