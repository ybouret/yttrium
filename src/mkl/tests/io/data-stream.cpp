#include "y/mkl/io/data-stream.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/stream/libc/input.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace Yttrium;

Y_UTEST(io_data_stream)
{
    Vector<double> X;
    List<float>    Y;
    Vector<long double,Memory::Dyadic> Z;

    MKL::DataStream::ColumnOf<double> CX("X",1,X);

    MKL::DataStream::Columns cols;

    cols.read( "X",1,X );
    cols.read( "Y",2,Y );
    cols.read( "Z",3,Z );

    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        cols.load(fp);
    }
    std::cerr << "X=" << X << std::endl;
    std::cerr << "Y=" << Y << std::endl;
    std::cerr << "Z=" << Z << std::endl;



}
Y_UDONE()

