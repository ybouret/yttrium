
#include "y/field/idb.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Field
    {

        class MetaKey
        {
        public:
            virtual ~MetaKey() noexcept;

            virtual size_t maxi() const noexcept = 0;
            size_t         size() const noexcept { return built; }


        protected:
            explicit MetaKey(void * const data) noexcept :
            built(0),
            array( static_cast<SharedString *>(data) )
            {
            }

            inline void push(const SharedString &uuid) noexcept
            {
                assert( size() < maxi() );
                new ( & array[built++] ) SharedString(uuid);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKey);
            size_t               built;
            SharedString * const array;

        };

        MetaKey:: ~MetaKey() noexcept
        {
            while(built>0)
                Destruct( & array[--built] );
        }

        template <size_t NSUB> class MetaKeyData
        {
        public:
            static const size_t Maxi     = 1+NSUB;;
            static const size_t Required = Maxi * sizeof(SharedString);

            inline virtual ~MetaKeyData() noexcept { clr(); }

        protected:
            inline explicit MetaKeyData() noexcept : wksp()
            {
                clr();
            }

            inline virtual void *data() noexcept { return &wksp[0]; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyData);
            void *wksp[ Y_WORDS_GEQ(Required) ];
            inline void clr() noexcept { Y_STATIC_ZARR(wksp); }
        };

        template <size_t NSUB>
        class MetaKeyWith : public MetaKeyData<NSUB>, public MetaKey
        {
        public:
            typedef MetaKeyData<NSUB> DataType;

            inline virtual ~MetaKeyWith() noexcept {}
            inline explicit MetaKeyWith(const SharedString &uuid) :
            DataType(),
            MetaKey(this->data())
            {
                push(uuid);
            }

            inline virtual size_t maxi() const noexcept { return this->Maxi; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyWith);
        };




    }

}


Y_UTEST(fieldmk)
{
    const Field::SharedString uuid = new String("field");

    Field::MetaKeyWith<0> mk0(uuid);


}
Y_UDONE()
