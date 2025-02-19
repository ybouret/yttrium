
#include "y/utest/run.hpp"

#include "y/osprey/tribes.hpp"

#include "y/random/shuffle.hpp"
#include "y/random/park-miller.hpp"


#include "y/text/ascii/convert.hpp"
#include "y/stream/hash/output.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"

#include "y/container/matrix.hpp"
#include "y/system/exception.hpp"

#include "y/stream/xmlog.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace Osprey
    {



       
    }

}

Y_UTEST(osprey)
{
    Random::ParkMiller ran;

    bool verbose = true;
    XMLog xml(verbose);

    size_t  rows = 5; if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t  dims = 6; if(argc>2) dims = ASCII::Convert::To<size_t>(argv[2],"dims");


    Matrix<int>      data(rows,dims);
    for (size_t i = 1; i <= data.rows; ++i)
    {
        for (size_t j = 1; j <= data.cols; ++j)
        {
            data[i][j] = ran.in<int>(-5,5);
        }
    }

    if(false)
    {
        if(rows>=3)
        {
            data[3].ld(0);
        }
        if(rows>=4)
        {
            data[4].ld(data[1]);
            for(size_t i=data.cols;i>0;--i) data[4][i] *= -3;
        }
        if(rows>=5)
        {
            for(size_t i=data.cols;i>0;--i) data[5][i] = 2 * data[1][i] - 3 * data[2][i];
        }
    }

    std::cerr << "data=" << data << std::endl;

    Osprey::IBank    bank;
    Osprey::QMetrics metrics(data.cols);
    Osprey::QVCache  vcache = new Apex::Ortho::Vector::Cache(metrics);
    Osprey::QFCache  fcache = new Apex::Ortho::Family::Cache(vcache);
    void (*proc_)(const Osprey::QVector &) = Osprey::Tribes::Display;
    Osprey::Callback proc(proc_);
    Osprey::Tribes   tribes(xml,proc,data,bank,fcache);

    size_t count = 0;
    while(tribes->size)
    {
        count += tribes->size;
        std::cerr << tribes << std::endl;
        tribes.generate(xml,proc,data);
    }
    std::cerr << "count=" << count << "/" << Osprey::Tribes::MaxCount(data.rows) << std::endl;
    std::cerr << "found=" << tribes.db.size << std::endl;


    for(size_t n=1;n<=16;++n)
    {
        const Apex::Natural nmax = Osprey::Tribes::MaxCount(n);
        std::cerr << n << " -> " << nmax.toDec(3) << std::endl;
    }


}
Y_UDONE()



