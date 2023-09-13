#include "y/mkl/io/data-stream.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;

Y_UTEST(io_data_stream)
{
    Vector<double> X;
    List<double>   Y;

    MKL::DataStream::ColumnOf<double> CX("X",1,X);

    MKL::DataStream::Columns cols;

    cols.read( "X",1,X );
    cols.read( "Y",2,Y );

    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        cols.load(fp);
    }


}
Y_UDONE()

