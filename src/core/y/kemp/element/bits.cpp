#include "y/kemp/element/add.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        namespace
        {
            static const uint8_t vbit[8] = { 1,2,4,8,16,32,64,128 };


            static inline
            bool getBit(const uint8_t * const item, const size_t ibit) noexcept
            {
                const size_t q = ibit >> 3;
                const size_t r = ibit &  7;
                return 0 != (item[q] & vbit[r]);
            }

            static inline
            void  setBit(uint8_t * const item, const size_t ibit) noexcept
            {
                const size_t q = ibit >> 3;
                const size_t r = ibit &  7;
                item[q] |= vbit[r];
            }
        }


        Element * Element:: shr(const size_t nbit)
        {
            if(nbit>=bits)
            {
                return new Element(0,AsCapacity);
            }
            else
            {
                const size_t          outBits = bits-nbit;
                const size_t          outSize = Bytes::BitsToPositive(outBits);
                Element       *       out     = new Element( outSize, AsCapacity ); assert(AsBytes==out->state);
                uint8_t       * const target  = out->bytes.item;
                const uint8_t * const source  = get<uint8_t>().item;
                for(size_t i=0,j=nbit;i<outBits;++i,++j)
                {
                    if( getBit(source,j) ) setBit(target,i);
                }
                out->bits           = outBits;
                out->bytes.positive = outSize;
                return Shrink( out->revise() );
            }
        }

        Element * Element:: shl(const size_t nbit)
        {

            const size_t          outBits = bits+nbit;
            const size_t          outSize = Bytes::BitsToPositive(outBits);
            Element       *       out     = new Element( outSize, AsCapacity ); assert(AsBytes==out->state);
            uint8_t       * const target  = out->bytes.item;
            const uint8_t * const source  = get<uint8_t>().item;

            for(size_t i=0,j=bits;i<bits;++i,++j)
            {
                if( getBit(source,i) ) setBit(target,j);
            }

            out->bits           = outBits;
            out->bytes.positive = outSize;
            return  out->revise();
        }



    }
}

