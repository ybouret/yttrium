#include "y/field/meta-key/with.hpp"
#include "y/field/meta-key/hasher.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

#include "y/text/hexadecimal.hpp"

#include "y/ptr/ark.hpp"
#include "y/associative/hash/set.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Field
    {

        template <size_t NSUB>
        class Dummy : public Object, public Counted
        {
        public:
            typedef ArkPtr<MetaKey,Dummy> Handle;

            explicit Dummy(const char *name) :
            metaKey(name)
            {
            }

            explicit Dummy(const MetaKeyWith<NSUB-1> &rootKey,
                           const unit_t               subIndx) :
            metaKey(rootKey,subIndx)
            {
            }


            inline virtual ~Dummy() noexcept
            {
            }

            const MetaKeyWith<NSUB> metaKey;

            const MetaKey & key() const noexcept { return metaKey; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dummy);
        };

    }
}

Y_UTEST(fieldmk)
{

    {
        Field::MetaKeyWith<0> mk0("field");
        std::cerr << mk0 << std::endl;

        Field::MetaKeyWith<1> mk1(mk0,1);
        std::cerr << mk1 << std::endl;

        Field::MetaKeyWith<2> mk2(mk1,-3);
        std::cerr << mk2 << std::endl;

        Field::MetaKeyHasher H;
        const size_t h0 = H(mk0);
        const size_t h1 = H(mk1);
        const size_t h2 = H(mk2);

        std::cerr << "h0=" << Hexadecimal(h0) << std::endl;
        std::cerr << "h1=" << Hexadecimal(h1) << std::endl;
        std::cerr << "h2=" << Hexadecimal(h2) << std::endl;
    }

    {
        typedef Field::Dummy<0> Dummy1D;
        typedef Field::Dummy<1> Dummy2D;

        HashSet<Field::MetaKey,Dummy1D::Handle,Field::MetaKeyHasher> db1D;
        HashSet<Field::MetaKey,Dummy2D::Handle,Field::MetaKeyHasher> db2D;

        {
            const Dummy1D::Handle dummy1D = new Dummy1D("f1da");
            Y_CHECK(db1D.insert(dummy1D));
        }

        {
            const Dummy1D::Handle dummy1D = new Dummy1D("f1db");
            Y_CHECK(db1D.insert(dummy1D));
        }

        {
            const Field::MetaKeyWith<0> mk0("f1da");
            const Dummy1D::Handle *phd = db1D.search(mk0);
            Y_ASSERT(0!=phd);
            const Dummy1D &dummy1D = **phd;
            {
                const Dummy2D::Handle dummy2D = new Dummy2D(dummy1D.metaKey,1);
                Y_CHECK(db2D.insert(dummy2D));
            }
            {
                const Dummy2D::Handle dummy2D = new Dummy2D(dummy1D.metaKey,-3);
                Y_CHECK(db2D.insert(dummy2D));
            }
        }



    }
}
Y_UDONE()
