#include "y/field/meta-key/with.hpp"
#include "y/field/meta-key/hasher.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_UTEST(fieldmk)
{
    const Field::SharedString uuid = new String("field");
    Field::MetaKeyWith<0> mk0(uuid);
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
Y_UDONE()
