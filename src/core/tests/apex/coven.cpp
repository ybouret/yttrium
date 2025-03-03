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
#include "y/string/env.hpp"

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


    static inline void ShowDot(const Ortho::Vector &)
    {
        (std::cerr << '.').flush();
    }

    template <typename MATRIX> static inline
    Digest Process(XMLog &            xml,
                   const MATRIX &     data,
                   const unsigned     flag,
                   Stats             &stats)
    {
        stats.ticks   = 0;

        Ortho::Coven::IBank    bank;
        Ortho::Metrics         qmtx(data.cols);
        Ortho::VCache          qvcc( new Ortho::Vector::Cache(qmtx) );
        Ortho::FCache          qfcc( new Ortho::Family::Cache(qvcc) );
        //Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );
        Ortho::Coven::Callback proc = cfunctor( ShowDot );
        std::cerr << "[";
        Ortho::Coven::Tribes   tribes(xml,proc,data,bank,qfcc,&stats.ticks);
        OutputFile             fp("coven.dat");

        Natural count = 0;
        while(tribes.size)
        {
            fp("%u %u %u\n", unsigned(tribes.iteration), unsigned(tribes.collected), unsigned(tribes.db.size) );
            count += tribes.size;
            tribes.generate(xml,proc,data,flag, &stats.ticks);
        }

        std::cerr << "]\ncount = " << count << " / " << Ortho::Coven::Tribes::MaxCount(data.rows) << std::endl << std::endl;

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

    size_t rows      = 4;    if(argc>1) rows      = ASCII::Convert::To<size_t>(argv[1],"rows");
    size_t cols      = rows; if(argc>2) cols      = ASCII::Convert::To<size_t>(argv[2],"cols");
    int    amplitude = 5;    if(argc>3) amplitude = ASCII::Convert::To<int>(argv[3],"amplitude");

    Matrix<int> data(rows,cols);
    for(size_t i=1;i<=rows;++i)
    {
        for(size_t j=1;j<=cols;++j)
        {
            data[i][j] = ran.in<int>(-amplitude,amplitude);
        }
    }

    //data[1].ld(0);
    //data[3].ld(data[2]);

    bool   verbose = Environment::Flag("VERBOSE");
    XMLog  xml(verbose);

    Stats s0; const Digest h0 = Process(xml,data,0,s0);
    Stats s1; const Digest h1 = Process(xml,data,Ortho::Coven::Strategy::RemoveFutile,s1);
    Stats s2; const Digest h2 = Process(xml,data,Ortho::Coven::Strategy::FindMultiple,s2);
    Stats s3; const Digest h3 = Process(xml,data,Ortho::Coven::Strategy::FindMatching,s3);
    Stats s4; const Digest h4 = Process(xml,data,Ortho::Coven::Strategy::ReplaceBasis,s4);
    Stats s5; const Digest h5 = Process(xml,data,Ortho::Coven::Strategy::HyperClosure,s5);
    Stats s6; const Digest h6 = Process(xml,data,Ortho::Coven::Strategy::Optimize,s6);

    std::cerr << "raw          h0=" << h0 << " " << s0 << std::endl;
    std::cerr << "RemoveFutile h1=" << h1 << " " << s1 << std::endl; Y_ASSERT(h1==h0);
    std::cerr << "FindMultiple h2=" << h2 << " " << s2 << std::endl; Y_ASSERT(h2==h0);
    std::cerr << "FindMatching h3=" << h3 << " " << s3 << std::endl; Y_ASSERT(h3==h0);
    std::cerr << "ReplaceBasis h4=" << h4 << " " << s4 << std::endl; Y_ASSERT(h4==h0);
    std::cerr << "HyperClosure h5=" << h5 << " " << s5 << std::endl; Y_ASSERT(h5==h0);
    std::cerr << "Optimize     h6=" << h6 << " " << s6 << std::endl; Y_ASSERT(h6==h0);




    Y_SIZEOF(Apex::Ortho::Vector);
    Ortho::Coven::VecDB vdb;
}
Y_UDONE()

