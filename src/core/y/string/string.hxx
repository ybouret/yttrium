
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

            //! copy with extra
            inline explicit Code(const Code &other, const size_t extra) :
            Y_STRING_CODE(other.maxi+extra)
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

            inline void free() noexcept
            {
                memset(data,0,size*sizeof(CH)); assert( Memory::OutOfReach::Are0(data+size, (maxBlocks-size)*sizeof(CH)));
                size = 0;
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

        template <>
        String<CH>:: String(const size_t minLength,
                            const AsCapacity_ &,
                            const bool  setLength) :
        Y_STRING_PROLOG(), code( new Code(minLength) )
        {
            if(setLength)
            {
                code->size = minLength;
            }
        }

        template <>
        String<CH>:: String(const String &other, const size_t extra) :
        Y_STRING_PROLOG(), code( new Code(*other.code,extra) )
        {

        }

        template <>
        void String<CH>:: free() noexcept
        {
            assert(0!=code);
            code->free();
        }
        
        template <>
        void String<CH>:: swapWith(String &s) noexcept
        {
            assert(code);
            assert(s.code);
            Swap(code,s.code);
        }

        template <>
        const String<CH> & String<CH>:: key() const noexcept
        {
            return *this;
        }


        template <>
        String<CH> & String<CH> :: operator=(const String<CH> &s)
        {
            String<CH> tmp(s);
            swapWith(tmp);
            return *this;
        }

#if 0
        template <>
        void String<CH>:: release() noexcept
        {
            assert(0!=code);
            code->free();
        }

        template <>
        void String<CH>:: reserve(const size_t n)
        {
            assert(0!=code);
            if(n>0)
            {
                String tmp(*this,n);
                swapWith(tmp);
            }
        }
#endif
        
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
        String<CH>::ConstType & String<CH>::operator[](const size_t index) const noexcept
        {
            assert(index>=1);
            assert(index<=size()+1);
            return code->item[index];
        }

        template <>
        String<CH>::Type & String<CH>::operator[](const size_t index)   noexcept
        {
            assert(0!=code);
            assert(index>=1);
            assert(index<=size());
            return code->item[index];
        }

        template <>
        String<CH>::ConstType * String<CH>:: operator()(void) const noexcept
        {
            assert(0!=code);
            return code->data;
        }

        template <>
        String<CH>::ConstType * String<CH>:: c_str() const noexcept
        {
            assert(0!=code);
            return code->data;
        }

        template <>
        void String<CH>:: reverse() noexcept
        {
            assert(0!=code);
            Algo::Reverse(code->data, code->size);
        }

    }

}
namespace Yttrium
{

    namespace Core
    {

        template <> size_t String<CH>:: measure() const noexcept
        {
            assert(0!=code);
            return code->size * sizeof(CH);
        }

        template <> const void * String<CH>:: ro_addr() const noexcept
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
        void String<CH>:: pushAtTail(const CH *arr, const size_t num)
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
            pushAtTail(&c,1);
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const CH *text)
        {
            pushAtTail(text,StringLength(text));
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator<<(const String &other)
        {
            pushAtTail(other.code->data, other.code->size);
            return *this;
        }

    }
    
}

namespace Yttrium
{

    namespace Core
    {

        template <>
        void String<CH>:: pushAtHead(const CH *arr, const size_t num)
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
            pushAtHead(&c,1);
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator>>(const CH *text)
        {
            pushAtHead(text,StringLength(text));
            return *this;
        }

        template <>
        String<CH> & String<CH>:: operator>>(const String &other)
        {
            pushAtHead(other.code->data, other.code->size);
            return *this;
        }

    }

}

namespace Yttrium
{

    namespace Core
    {
        template<>
        String<CH> operator+<CH>(const String<CH> &lhs, const String<CH> &rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              rhs.code->data,rhs.code->size);
        }

        template <>
        String<CH> operator+<CH>(const String<CH> &lhs, const CH *rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              rhs,StringLength(rhs));
        }

        template <>
        String<CH> operator+<CH>(const CH *lhs, const String<CH> &rhs)
        {
            return String<CH>(lhs,StringLength(lhs),
                              rhs.code->data,rhs.code->size);
        }

        template <>
        String<CH> operator+<CH>(const String<CH> &lhs, const CH rhs)
        {
            return String<CH>(lhs.code->data,lhs.code->size,
                              &rhs,1);
        }

        template <>
        String<CH> operator+<CH>(const CH lhs, const String<CH> &rhs)
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


namespace Yttrium
{
    namespace Core
    {

        template <>
        String<CH>:: String(const size_t len, Random::Bits &ran, const CH lower, const CH upper) :
        Y_STRING_PROLOG(), code( new Code(len) )
        {
            while(code->size<len)
            {
                code->data[code->size++] = ran.in<CH>(lower,upper);
            }
        }
    }

}

namespace Yttrium
{

    namespace Core
    {
        template <>
        const CH * String<CH>:: getBaseForward() const noexcept
        {
            assert(0!=code);
            return code->data;
        }

        template <>
        const CH * String<CH>:: getLastForward() const noexcept
        {
            assert(0!=code);
            return code->data+code->size;
        }

        template <>
        const CH * String<CH>:: getBaseReverse() const noexcept
        {
            assert(0!=code);
            return code->item+code->size;
        }

        template <>
        const CH * String<CH>:: getLastReverse() const noexcept
        {
            assert(0!=code);
            return code->item;
        }
    }

}

namespace Yttrium
{

    namespace Core
    {
        template <>
        void String<CH>:: popTail() noexcept
        {
            assert(0!=code);
            assert(code->size>0);
            assert(0!=code->data);
            code->data[--(code->size)] = 0;
        }

        template <>
        void String<CH>:: popHead() noexcept
        {
            assert(0!=code);
            assert(code->size>0);
            assert(0!=code->data);
            memmove(code->data,code->data+1,--(code->size)*sizeof(CH));
            code->data[code->size] = 0;
        }

        template <>
        void String<CH>:: pushTail(ParamType c)
        {
            pushAtTail(&c,1);
        }

        template <>
        void String<CH>:: pushHead(ParamType c)
        {
            pushAtHead(&c,1);
        }

        template <>
        const CH & String<CH>:: getHead() const noexcept
        {
            assert(0!=code);
            assert(0!=code->data);
            assert(code->size>0);
            return code->data[0];
        }

        template <>
        const CH & String<CH>:: getTail() const noexcept
        {
            assert(0!=code);
            assert(0!=code->data);
            assert(code->size>0);
            return code->item[code->size];
        }

    }

}


namespace Yttrium
{

    namespace Core
    {
        template <>
        size_t  String<CH>:: serialize(OutputStream &fp) const
        {
            assert(0!=code);
            assert(0!=code->data);
            size_t written = fp.emitVBR(code->size);
            for(size_t i=0;i<code->size;++i)
                written += fp.emitCBR(code->data[i]);
            return written;
        }

        template <>
        String<CH> String<CH>:: ReadFrom(InputStream &fp)
        {
            const size_t  nc = fp.readVBR<size_t>("String.size");
            String<CH>    res(nc,AsCapacity,false);
            for(size_t i=0;i<nc;++i)
            {
                res << fp.readCBR<CH>("String.char");
            }
            return res;
        }
    }

}


