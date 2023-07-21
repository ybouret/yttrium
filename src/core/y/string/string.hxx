
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

            //__________________________________________________________________
            //
            //! push a block at tail
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
            //! push a block at head
            //__________________________________________________________________
            bool pushHead(const CH *arr, const size_t num) noexcept
            {
                assert(Good(arr,num));

                if(num<=maxi-size)
                {
                    memmove(data+num,data,size*sizeof(CH));
                    memmove(data,arr,num);
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

        template <>
        String<CH> ::
        String(const CH *lhs, const size_t nl,
               const CH *rhs, const size_t nr) :
        Y_STRING_PROLOG(), code( new Code(lhs,nl,rhs,nr) )
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

    }

}



namespace Yttrium
{

    namespace Core
    {

        template <>
        void String<CH>:: pushTail(const CH *arr, const size_t num)
        {
            assert(Good(arr,num));
            if(! code->pushTail(arr,num) )
            {
                Code *temp = new Code(code->data,code->size,arr,num);
                delete code;
                code = temp;
            }
        }

        template <>
        String<CH> & String<CH>:: operator<<(const CH c)
        {
            pushTail(&c,1);
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const CH *text)
        {
            pushTail(text,StringLength(text));
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const String &other)
        {
            pushTail(other.code->data, other.code->size);
            return *this;
        }

    }
    
}

namespace Yttrium
{

    namespace Core
    {

        template <>
        void String<CH>:: pushHead(const CH *arr, const size_t num)
        {
            assert(Good(arr,num));
            if(! code->pushHead(arr,num) )
            {
                Code *temp = new Code(arr,num,code->data,code->size);
                delete code;
                code = temp;
            }
        }

        template <>
        String<CH> & String<CH>:: operator>>(const CH c)
        {
            pushHead(&c,1);
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator>>(const CH *text)
        {
            pushHead(text,StringLength(text));
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator>>(const String &other)
        {
            pushHead(other.code->data, other.code->size);
            return *this;
        }

    }

}

namespace Yttrium
{

    namespace Core
    {
        String<CH> operator+(const String<CH> &lhs, const String<CH> &rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              rhs.code->data,rhs.code->size);
        }


        String<CH> operator+(const String<CH> &lhs, const CH *rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              rhs,StringLength(rhs));
        }

        String<CH> operator+(const CH *lhs, const String<CH> &rhs)
        {
            return String<CH>(lhs,StringLength(lhs),
                              rhs.code->data,rhs.code->size);
        }

        String<CH> operator+(const String<CH> &lhs, const CH rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              &rhs,1);
        }

        String<CH> operator+(const CH lhs, const String<CH> &rhs)
        {
            return String<CH>(&lhs,1,
                              rhs.code->data,rhs.code->size);
        }

    }

}

namespace Yttrium
{

    namespace Core
    {
        template <>
        String<CH> & String<CH>:: operator+=(const String &other)
        {
            return (*this) << other;
        }

        template <>
        String<CH> & String<CH>:: operator+=(const CH *other)
        {
            return (*this) << other;
        }

        template <>
        String<CH> & String<CH>:: operator+=(const CH c)
        {
            return (*this) << c;
        }
    }
}
