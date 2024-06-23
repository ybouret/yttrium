#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/kemp/modular.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

namespace Yttrium
{
    namespace RSA
    {

        class PublicKey : public Object
        {
        protected:
            explicit PublicKey(const apn &pubN, const apn &pubE) :
            n(pubN),
            e(pubE)
            {
            }

        public:
            virtual ~PublicKey() noexcept {}

            static PublicKey *Create(const apn &p, const apn &q, const apn &e)
            {
                const apn pm = p-1;
                const apn qm = q-1;
                const apn n  = p*q;
                //const apn phi = pm*qm;
                return new PublicKey(n,e);
            }

            apn pub(const apn &M)
            {
                return Kemp::Modular::Exp(M,e,n);
            }

            const apn n;
            const apn e;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PublicKey);
        };

        class PrivateKey : public PublicKey
        {
        protected:
            explicit PrivateKey(const apn &pubN, const apn &pubE, const apn &prvD) :
            PublicKey(pubN,pubE),
            d(prvD)
            {
            }

        public:
            virtual ~PrivateKey() noexcept {}

            static PrivateKey *Create(const apn &p, const apn &q, const apn &e)
            {
                const apn pm = p-1;
                const apn qm = q-1;
                const apn n  = p*q;
                const apn phi = pm*qm;
                const apn d   = Kemp::Modular::Inv(e,phi);
                return new PrivateKey(n,e,d);
            }

            apn prv(const apn &M)
            {
                return Kemp::Modular::Exp(M,d,n);
            }

            const apn d;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PrivateKey);
        };


    };
}

Y_UTEST(kemp_rsa)
{
    Random::ParkMiller ran;
    Y_SIZEOF(RSA::PublicKey);
    const apn p   = 67;
    const apn q   = 107;
    const apn e   = 7;
    AutoPtr<RSA::PublicKey>  pub = RSA::PublicKey::Create(p,q,e);
    AutoPtr<RSA::PrivateKey> prv = RSA::PrivateKey::Create(p,q,e);

    {
        const apn M = apn(pub->n.bits(),ran) % pub->n;
        const apn C = pub->pub(M);
        const apn D = prv->prv(C);
        std::cerr << M << "->" << C << "->" << D << std::endl;
    }

}
Y_UDONE()
