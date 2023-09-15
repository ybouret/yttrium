#include "y/associative/suffix/set.hpp"
#include "y/associative/little-endian-address.hpp"
#include "y/graphviz/vizible.hpp"
#include "y/utest/run.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;

template <typename T>
class IKey
{
public:
    inline IKey(const T k) noexcept : val_(k), key_(val_) {}
    ~IKey() noexcept {}

    const LittleEndianKey & key() const noexcept { return key_; }

    friend inline std::ostream & operator<<(std::ostream &os, const IKey &self)
    {
        os << self.val_;
        return os;
    }

private:
    const T         val_;
    LittleEndianKey key_;
};

template <typename T>
class IKeySet : public SuffixSet<LittleEndianKey,IKey<T> >
{
public:
    IKeySet() {}
    virtual ~IKeySet() noexcept {}

    bool add(const T k)   {
        const IKey<T> _(k);
        return this->insert(_);
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(IKeySet);
};


Y_UTEST(associative_key_set)
{

    IKeySet<size_t> kset;

    kset.add(1);
    kset.add(2);
    kset.add(3);
    Vizible::GraphViz("kset.dot", kset, true);

    std::cerr << kset << std::endl;

    Iterating::DisplayAll::Of(kset);


}
Y_UDONE()


