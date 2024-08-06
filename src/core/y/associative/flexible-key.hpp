//! \file

#ifndef Y_Flexible_Key_Included
#define Y_Flexible_Key_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/type/args.hpp"
#include "y/quantized.hpp"
#include "y/container/cxx/array.hpp"
#include "y/associative/little-endian.hpp"
#include "y/type/nullify.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Flexible Key of integral type(s)
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class FlexibleKey : public Memory::ReadOnlyBuffer
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitiosn
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                                    //!< aliases
        typedef CxxArray<MutableType,Memory::Dyadic> ArrayType; //!< alias
        typedef typename IntegerFor<T>::UInt::Type   WordType;  //!< alias

        //______________________________________________________________________
        //
        //
        //! internal code
        //
        //______________________________________________________________________
        class Code : public Quantized
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit Code(ConstType * const entry,
                                 const size_t      words) :
            Quantized(),
            data(words,0),
            blockAddr(words>0? & data[1] : 0),
            blockSize(words*sizeof(Type))
            {
                assert(Good(entry,words));
                for(size_t i=1,j=0;i<=words;++i,++j)
                    LittleEndian::Make( (uint8_t*) &data[i], entry[j]);
            }

            //! setup
            template <typename U>
            inline explicit Code(const Readable<U> &arr) :
            Quantized(),
            data( arr.size(), 0 ),
            blockAddr(data.size() > 0 ? & data[1] : 0),
            blockSize(data.size() *sizeof(Type))
            {
                for(size_t i=1;i<=data.size();++i)
                {
                    const WordType word( arr[i] );
                    LittleEndian::Make( (uint8_t*) &data[i], word );
                }
            }



            //! copy
            inline explicit Code(const Code &other) :
            Quantized(),
            data(other.data),
            blockAddr(data.size()>0? &data[1] : 0),
            blockSize(other.blockSize)
            {
                assert( blockSize == data.size() * sizeof(Type) );
            }

            //! cleanup
            inline virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const ArrayType    data;      //!< little-endian data
            const void * const blockAddr; //!< &data[1] or NULL
            const size_t       blockSize; //!< data.size() * sizeof(T)

        private:
            Y_DISABLE_ASSIGN(Code);
        };

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from array
        inline explicit FlexibleKey(ConstType * const entry,
                                    const size_t      words) :
        Memory::ReadOnlyBuffer(),
        code( new Code(entry,words) )
        {
        }

        //! setup from any compatible readable
        template <typename U>
        inline explicit FlexibleKey(const Readable<U> &arr) :
        Memory::ReadOnlyBuffer(),
        code( new Code(arr) )
        {
        }


        //! copy
        inline FlexibleKey(const FlexibleKey &key) :
        Memory::ReadOnlyBuffer(),
        code( new Code( *key.code ) )
        {
        }

        //! cleanup
        inline virtual ~FlexibleKey() noexcept {  assert(0!=code); Nullify(code); }


        //______________________________________________________________________
        //
        //
        // [Memory::ReadOnlyBuffer]
        //
        //______________________________________________________________________

        //! code->blockAddr
        inline virtual const void * ro_addr() const noexcept {
            assert(0!=code);
            return code->blockAddr;
        }

        //! code->blockSize
        inline virtual size_t measure() const noexcept
        {
            assert(0!=code);
            return code->blockSize;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! access
        const Readable<T> & operator*() const noexcept
        {
            return code->data;
        }

        //! access
        const Readable<T> * operator->() const noexcept
        {
            return & (code->data);
        }



    private:
        Y_DISABLE_ASSIGN(FlexibleKey);
        Code *             code;

    };
}

#endif

