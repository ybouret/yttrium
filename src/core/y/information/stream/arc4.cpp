
#include "y/information/stream/arc4.hpp"
#include "y/object.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Information
    {

        class ARC4:: Code : public Object
        {
        public:
            static const size_t Bytes = 2*256;
            explicit Code(const void    *key,
                          const size_t   keyLength) :
            Object(),
            shift( iLog2<Bytes>::Value ),
            S( static_cast<uint8_t *>( Memory::Dyadic::Instance().acquireBlock(shift)) ),
            Q( S+256 ),
            I(0),
            J(0)
            {
                KSA( static_cast<const uint8_t *>(key),keyLength);
            }

            virtual ~Code() noexcept
            {
                Memory::Dyadic::Location().releaseBlock(*(void **)&S,shift);
            }

            inline void restart() noexcept
            {
                for(size_t i=0;i<256;++i) S[i] = Q[i];
                I = 0;
                J = 0;
            }

            inline uint8_t nextByte() noexcept
            {
                I = (I+1)    & 0xff;
                J = (J+S[I]) & 0xff;
                Swap(S[I],S[J]);
                const unsigned Si = S[I], Sj = S[J];
                return S[ (Si+Sj) & 0xff ];
            }

            unsigned              shift;
            uint8_t *             S;
            const uint8_t * const Q;
            size_t                I;
            size_t                J;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);

            void KSA(const uint8_t *key,
                     size_t         keyLength)
            {
                assert(Good(key,keyLength));

                // prepare key
                static const uint8_t zero = 0;
                if(keyLength<=0)
                {
                    key       = &zero;
                    keyLength = 1;
                }

                // initialize S
                for(unsigned i=0;i<256;++i) S[i] = uint8_t(i);

                // warm-up S
                for(unsigned j=0,i=0;i<256;++i)
                {
                    j = (j + S[i] + key[i%keyLength]) & 0xff;
                    Swap(S[i],S[j]);
                }

                // save S into Q
                for(unsigned i=0;i<256;++i) S[i+256] = S[i];


            }
        };
    }

}


#include "y/text/ops.hpp"

namespace Yttrium
{
    namespace Information
    {
        ARC4:: Encoder:: Encoder(const void    *key,
                                 const size_t   keyLength) :
        StreamCipher(),
        code( new Code(key,keyLength) )
        {
        }

        ARC4:: Encoder:: Encoder(const char *key) :
        StreamCipher(),
        code( new Code(key,StringLength(key) ) )
        {
        }


        ARC4:: Encoder:: ~Encoder() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        void ARC4:: Encoder:: restart() noexcept
        {

        }

        uint8_t ARC4:: Encoder:: operator()(const uint8_t x) noexcept
        {
            assert(0!=code);
            return x ^ code->nextByte();
        }
    }

}
