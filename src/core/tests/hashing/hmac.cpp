
#include "y/hashing/mac.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"

#include "y/hashing/md.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(hashing_mac)
{
    {
        Hashing::HMAC<Hashing::MD5> HMAC_MD5("key");
        const Digest D = Hashing::MD::Of(HMAC_MD5,"The quick brown fox jumps over the lazy dog");
        const Digest R = "80070713463e7749b90c2dc24911e275";
        std::cerr << D << "/" << R << std::endl;
        Y_CHECK(D==R);
    }

    {
        Hashing::HMAC<Hashing::SHA1> HMAC_SHA1("key");
        const Digest D = Hashing::MD::Of(HMAC_SHA1,"The quick brown fox jumps over the lazy dog");
        const Digest R = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
        std::cerr << D << "/" << R << std::endl;
        Y_CHECK(D==R);
    }

    {
        Hashing::HMAC<Hashing::SHA256> HMAC_SHA256("key");
        const Digest D = Hashing::MD::Of(HMAC_SHA256,"The quick brown fox jumps over the lazy dog");
        const Digest R = "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8";
        std::cerr << D << "/" << R << std::endl;
        Y_CHECK(D==R);
    }

    {
        Hashing::HMAC<Hashing::SHA512> HMAC_SHA512("key");
        const Digest D = Hashing::MD::Of(HMAC_SHA512,"The quick brown fox jumps over the lazy dog");
        const Digest R = "b42af09057bac1e2d41708e48a902e09b5ff7f12ab428a4fe86653c73dd248fb82f948a549f7b791a5b41915ee4d1ec3935357e4e2317250d0372afa2ebeeb3a";
        std::cerr << D << "/" << R << std::endl;
        Y_CHECK(D==R);
    }

}
Y_UDONE()

