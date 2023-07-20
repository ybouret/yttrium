
namespace Yttrium
{
    
    namespace Core
    {

        template <>
        class String<CH>:: Impl : public Object
        {
        public:
            explicit Impl(size_t n) :
            Object(),
            items(Max(n,StringMinChars)+1),
            bytes(0),
            data( StringAcquire<CH>(Coerce(items), Coerce(bytes))),
            size(0),
            maxi(items-1)
            {
                assert( Check("String()") );
            }

            virtual ~Impl() noexcept { StringRelease(data, Coerce(items), Coerce(bytes) ); }

            explicit Impl(const Impl &impl, const size_t extra) :
            Object(),
            items(impl.items+extra),
            bytes(0),
            data( StringAcquire<CH>(Coerce(items), Coerce(bytes))),
            size(0),
            maxi(items-1)
            {
                assert(items>=impl.items);
                memcpy(data,impl.data, (size=impl.size) * sizeof(CH) );
                assert( Check("String(copy)") );
            }


            inline bool Check(const char *when) const noexcept
            {
                if(bytes<items*sizeof(CH)) { std::cerr << when << " bad bytes/items" << std::endl; }
                if(0==data)                { std::cerr << when << " missing data!"   << std::endl; }
                if(items-1!=maxi)          { std::cerr << when << " bad maxi/items"  << std::endl; }
                if(size>maxi)              { std::cerr << when << " bad size/maxi"   << std::endl; }
                if( !Memory::OutOfReach::Are0(data+size,(items-size)*sizeof(CH)) )
                {
                    std::cerr << when << " dirty string!" << std::endl;
                }
                return true;
            }

            const size_t items;//!< allocated items
            const size_t bytes;//!< allocated bytes
            CH          *data; //!< flat memory data[0..items-1]
            size_t       size; //!< in data[0..maxi-1]
            const size_t maxi; //!< capacity

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Impl);

        };

        template <> String<CH>:: String() :
        Y_STRING_PROLOG(), impl( new Impl(0) )
        {
            std::cerr << "sizeof(String::Impl) = " << sizeof(Impl) << std::endl;
        }

        template <> String<CH>:: String(const String &other) :
        Y_STRING_PROLOG(), impl( new Impl(*other.impl,0) )
        {
        }

        template <> String<CH>:: ~String() noexcept
        {
            assert(0!=impl);
            delete impl;
            impl = 0;
        }

        template <> size_t String<CH>:: size() const noexcept
        {
            assert(0!=impl);
            return impl->size;
        }

        template <> size_t String<CH>:: capacity() const noexcept
        {
            assert(0!=impl);
            return impl->maxi;
        }

        template <> typename String<CH>::ConstType & String<CH>::operator[](const size_t index) const noexcept
        {
            assert(index>=1);
            assert(index<=size());
            return impl->data[index];
        }

        template <> typename String<CH>::Type & String<CH>::operator[](const size_t index)   noexcept
        {
            assert(0!=impl);
            assert(index>=1);
            assert(index<=size());
            return impl->data[index];
        }

        template <>
        typename String<CH>::ConstType * String<CH>:: operator()(void) const noexcept
        {
            assert(0!=impl);
            return impl->data;
        }


    }
    
}
