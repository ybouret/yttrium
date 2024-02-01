
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

            virtual unsigned maxi() const noexcept = 0;
            unsigned         size() const noexcept { return built; }

            inline friend std::ostream & operator<<(std::ostream &os, const MetaKey &mk)
            {
                String ans;
                for(unsigned i=0;i<mk.built;++i) ans += mk[i];
                os << ans;
                return os;
            }

            inline const String & operator[](const unsigned dim) const noexcept
            {
                assert(dim<built);
                return *array[dim];
            }

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

            inline void push(const String &s) noexcept
            {
                assert(s.quantity()>0);
                const SharedString uuid( & Coerce(s) );
                push(uuid);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKey);
            unsigned             built;
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
            inline explicit MetaKeyWith(const SharedString &uuid) noexcept:
            DataType(),
            MetaKey(this->data())
            {
                push(uuid);
            }





            inline explicit MetaKeyWith(const MetaKeyWith<NSUB-1> &root,
                                        const unit_t               indx) noexcept:
            DataType(),
            MetaKey(this->data())
            {
                static IndexDataBase &idb = IndexDataBase::Instance();
                const unsigned        num = root.size();
                for(unsigned i=0;i<num;++i) push( root[i] );
                push( idb[indx] );
            }


            inline virtual unsigned maxi() const noexcept { return this->Maxi; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyWith);
        };




    }

}


Y_UTEST(fieldmk)
{
    const Field::SharedString uuid = new String("field");
    Field::MetaKeyWith<0> mk0(uuid);
    std::cerr << mk0 << std::endl;

    Field::MetaKeyWith<1> mk1(mk0,1);
    std::cerr << mk1 << std::endl;

    Field::MetaKeyWith<2> mk2(mk1,-3);
    std::cerr << mk2 << std::endl;


}
Y_UDONE()
