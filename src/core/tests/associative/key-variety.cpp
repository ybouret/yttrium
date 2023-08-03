#include "y/associative/key-variety.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/memory/digest.hpp"

using namespace Yttrium;

static const char *answer(const bool flag) noexcept
{
    return flag ? "YES" : "NO";
}

template <typename T>
static inline void analyze(const char *name)
{
    std::cerr << std::endl;
    std::cerr << name << " : " << std::endl;
    std::cerr << "\tIsMemoryBuffer ? " << answer(KeyVariety::Cull<T>::_IsMemoryBuffer) << std::endl;
    std::cerr << "\tIsLegacyString ? " << answer(KeyVariety::Cull<T>::_IsLegacyString) << std::endl;
    std::cerr << "\tIsIntegralType ? " << answer(KeyVariety::Cull<T>::_IsIntegralType) << std::endl;

}

#define Y_KV(TYPE) analyze<TYPE>( #TYPE )

Y_UTEST(associative_key_variety)
{
    std::cerr << KeyVariety::Type2Text(KeyVariety::IsNotAvailable) << std::endl;
    std::cerr << KeyVariety::Type2Text(KeyVariety::IsMemoryBuffer) << std::endl;
    std::cerr << KeyVariety::Type2Text(KeyVariety::IsIntegralType) << std::endl;
    std::cerr << KeyVariety::Type2Text(KeyVariety::IsLegacyString) << std::endl;

    Y_KV(Memory::ReadOnlyBuffer);
    Y_KV(Memory::ReadWriteBuffer);
    Y_KV(String);
    Y_KV(Digest);
    Y_KV(const String);
    Y_KV(char);
    Y_KV(char *);
    Y_KV(char []);
    Y_KV(const char *);
    Y_KV(const char []);

    


}
Y_UDONE()
