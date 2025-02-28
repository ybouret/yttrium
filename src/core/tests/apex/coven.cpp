#include "y/apex/api/ortho/coven/tribes.hpp"

#include "y/utest/run.hpp"


#include "y/associative/hash/set.hpp"
#include "y/stream/hash/srzh.hpp"
#include "y/hashing/sha1.hpp"
#include "y/ptr/ark.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

                typedef ArkPtr<Vector,Vector>                VPointer;
                typedef SerializableKeyHasher<Hashing::SHA1> VHasher;

#if 1
                class VecDB : public HashSet<Vector,VPointer,VHasher>
                {
                public:
                    explicit VecDB()
                    {
                    }

                    virtual ~VecDB() noexcept
                    {
                    }


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(VecDB);
                };
#endif


            }

        }

    }

}

#include "y/text/ascii/convert.hpp"
#include "y/container/matrix.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/system/wtime.hpp"

namespace
{

    struct Stats
    {
        size_t   vc;
        size_t   fc;
        uint64_t ticks;

        inline friend
        std::ostream & operator<<(std::ostream &os, const Stats &s)
        {
            WallTime chrono;
            os << "#v = " << std::setw(6) << s.vc << " | #f = " << std::setw(6) << s.fc << " | time= " << std::setw(6) << chrono(s.ticks);
            return os;
        }
    };

    template <typename MATRIX> static inline
    Digest Process(XMLog &            xml,
                   const MATRIX &     data,
                   const unsigned     flag,
                   Stats             &stats)
    {

        Ortho::Coven::IBank    bank;
        Ortho::Metrics         qmtx(data.cols);
        Ortho::VCache          qvcc( new Ortho::Vector::Cache(qmtx) );
        Ortho::FCache          qfcc( new Ortho::Family::Cache(qvcc) );
        Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );

        Ortho::Coven::Tribes   tribes(xml,proc,data,bank,qfcc);
        OutputFile             fp("coven.dat");

        stats.ticks = 0;
        Natural count = 0;
        while(tribes.size)
        {
            fp("%u %u %u\n", unsigned(tribes.iteration), unsigned(tribes.collected), unsigned(tribes.db.size) );
            count += tribes.size;
            //std::cerr << tribes << std::endl;
            const uint64_t ini = WallTime::Ticks();
            tribes.generate(xml,proc,data,flag);
            stats.ticks += WallTime::Ticks() - ini;
        }

        std::cerr << "count = " << count << " / " << Ortho::Coven::Tribes::MaxCount(data.rows) << std::endl;

        stats.vc = (*qvcc)->size;
        stats.fc = (*qfcc)->size;


        Hashing::SHA1 H;
        return tribes.signature(H);
    }
}

Y_UTEST(apex_coven)
{

    Random::ParkMiller ran;
    Y_SIZEOF(Ortho::Coven::Tribe);

    size_t rows = 4;    if(argc>1) rows = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t cols = rows; if(argc>2) cols = ASCII::Convert::To<size_t>(argv[2],"cols");

    Matrix<int> data(rows,cols);
    for(size_t i=1;i<=rows;++i)
    {
        for(size_t j=1;j<=cols;++j)
        {
            data[i][j] = ran.in<int>(-2,2);
        }
    }

    //data[1].ld(0);
    //data[3].ld(data[2]);

    bool   verbose = true;
    XMLog  xml(verbose);

    Stats s0; const Digest h0 = Process(xml,data,0,s0);
    Stats s1; const Digest h1 = Process(xml,data,Ortho::Coven::Tribes::RemoveFutile,s1);
    Stats s2; const Digest h2 = Process(xml,data,Ortho::Coven::Tribes::FindMultiple,s2);
    Stats s3; const Digest h3 = Process(xml,data,Ortho::Coven::Tribes::RemoveFutile|Ortho::Coven::Tribes::FindMatching,s3);

    std::cerr << "raw          h0=" << h0 << " " << s0 << std::endl;
    std::cerr << "RemoveFutile h1=" << h1 << " " << s1 << std::endl; Y_ASSERT(h1==h0);
    std::cerr << "FindMultiple h2=" << h2 << " " << s2 << std::endl; Y_ASSERT(h2==h0);




    Y_SIZEOF(Apex::Ortho::Vector);
    Ortho::Coven::VecDB vdb;
}
Y_UDONE()

