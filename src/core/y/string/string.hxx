
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

            }

            virtual ~Impl() noexcept { StringRelease(data, Coerce(items), Coerce(bytes) ); }

            

            const size_t items;//!< allocated items
            const size_t bytes;//!< allocated bytes
            CH          *data; //!< flat memory
            size_t       size; //!< in data[0..maxi]
            const size_t maxi; //!< capacity

        };

        template <> String<CH>:: String() :
        impl( new Impl(0) )
        {
            std::cerr << "sizeof(String::Impl) = " << sizeof(Impl) << std::endl;
        }

        template <> String<CH>:: ~String() noexcept
        {
            delete impl;
            impl = 0;
        }

        template <> size_t String<CH>:: size() const noexcept
        {
            return impl->size;
        }

        template <> size_t String<CH>:: capacity() const noexcept
        {
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
            assert(index>=1);
            assert(index<=size());
            return impl->data[index];
        }

    }
    
}
