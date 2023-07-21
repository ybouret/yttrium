
namespace Yttrium
{
    
    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! String Code
        //
        //
        //______________________________________________________________________
        template <>
        class String<CH>:: Code :
        public Object,
        public Memory::Wad<CH,Memory::Pooled>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Memory::Wad<CH,Memory::Pooled> WadType;
            using   WadType:: workspace;
            using   WadType:: maxBlocks;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! default
            inline explicit Code(const size_t numChars) :
            Y_STRING_CODE(numChars)
            {
                assert(Memory::OutOfReach::Are0(data,maxBlocks*sizeof(CH)));
            }

            //! copy
            inline explicit Code(const Code &other) :
            Y_STRING_CODE(other.size)
            {
                memcpy(data,other.data,(size=other.size)*sizeof(CH));
            }

            inline explicit Code(const CH *arr, const size_t num) :
            Y_STRING_CODE(num)
            {
                assert(Good(arr,num));
                memcpy(data,arr,(size=num)*sizeof(CH));
            }

            inline explicit Code(const CH *cl, const size_t nl,
                                 const CH *cr, const size_t nr) :
            Y_STRING_CODE(nl+nr)
            {
                assert(Good(cl,nl));
                assert(Good(cr,nr));
                memcpy(data,     cl,nl*sizeof(CH)); size += nl;
                memcpy(data+size,cr,nr*sizeof(CH)); size += nr;
            }


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool pushTail(const CH *arr, const size_t num) noexcept
            {
                assert(Good(arr,num));

                if(num<=maxi-size)
                {
                    memmove(data+size,arr,num*sizeof(CH));
                    size += num;
                    return true;
                }
                else
                {
                    return false;
                }
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            CH          *data; //!< workspace
            CH          *item; //!< data-1
            size_t       size; //! in 0..maxi
            const size_t maxi; //!< maxBlocks-1

        private:
            Y_DISABLE_ASSIGN(Code);
        };

    }

}

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        // setup
        //
        //______________________________________________________________________
        template <> String<CH>:: String() :
        Y_STRING_PROLOG(), code( new Code(0) )
        {
            std::cerr << "sizeof(String::Impl) = " << sizeof(Code) << std::endl;
        }

        template <> String<CH>:: ~String() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        template <> String<CH>:: String(const String &other) :
        Y_STRING_PROLOG(), code( new Code(*other.code) )
        {
        }

        template <> String<CH>:: String(const CH ch) :
        Y_STRING_PROLOG(), code( new Code(&ch,1) )
        {
        }

        template <> String<CH>:: String(const CH *arr, const size_t num) :
        Y_STRING_PROLOG(), code( new Code(arr,num) )
        {
        }

        template <> String<CH>:: String(const CH *arr) :
        Y_STRING_PROLOG(), code( new Code(arr,StringLength(arr) ) )
        {
        }

        


    }

}

namespace Yttrium
{

    namespace Core
    {

        template <> size_t String<CH>:: size() const noexcept
        {
            assert(0!=code);
            return code->size;
        }

        template <> size_t String<CH>:: capacity() const noexcept
        {
            assert(0!=code);
            return code->maxi;
        }

        template <>
        typename String<CH>::ConstType & String<CH>::operator[](const size_t index) const noexcept
        {
            assert(index>=1);
            assert(index<=size());
            return code->item[index];
        }

        template <>
        typename String<CH>::Type & String<CH>::operator[](const size_t index)   noexcept
        {
            assert(0!=code);
            assert(index>=1);
            assert(index<=size());
            return code->item[index];
        }

        template <>
        typename String<CH>::ConstType * String<CH>:: operator()(void) const noexcept
        {
            assert(0!=code);
            return code->data;
        }


#if 0



        template <>
        void String<CH>:: append(const CH *arr, const size_t num)
        {
            assert(Good(arr,num));
            if(!impl->accepted(arr,num) )
            {
                Impl *temp = new Impl(*impl,num);
                temp->catenate(arr,num);
                Swap(impl,temp);
                delete temp;
            }
        }

        template <>
        String<CH> & String<CH>:: operator<<(const CH c)
        {
            append(&c,1);
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const CH *text)
        {
            append(text,StringLength(text));
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const String &other)
        {
            append(other.impl->data, other.impl->size);
            return *this;
        }

        
#endif


    }
    
}
