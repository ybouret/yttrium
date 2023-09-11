
#include "y/type/moniker.hpp"
#include "y/utest/run.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    class XNode
    {
    public:
        inline  XNode(const T x ) : next(0), prev(0), value(x) {}
        inline ~XNode() noexcept {}


        XNode *next, *prev;
        Moniker<T> value;
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XNode);
    };

    template <typename T> static inline
    void testXNode(Random::Bits &ran)
    {
        static const size_t bs = sizeof( XNode<T> );
        void *obj =  Object::operator new(bs);

        Random::Fill::Block(obj,bs,ran,0x01,0xff);
        const T x = 1000;
        XNode<T> *node = new (obj) XNode<T>( x );

        std::cerr << node->value << std::endl;
        Memory::OutOfReach::Naught(node);

        Object::operator delete(obj,bs);
    }

}

Y_UTEST(type_moniker)
{
    Random::Rand ran;

    testXNode<float>(ran);


}
Y_UDONE()
