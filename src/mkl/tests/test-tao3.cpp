
#include "y/mkl/tao/level3.hpp"

#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "../../core/tests/main.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(tao3)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;



    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Driver          seq(seqLoop);
    Tao::Driver          par(parLoop);
    Random::Rand         ran;
    Antelope::Caddy<int> xma;




    for(size_t iter=0;iter<2;++iter)
    {
        const size_t nr = 1+ran.leq(10);
        const size_t nc = 1+ran.leq(10);
        const size_t ns = 1+ran.leq(10);
        Matrix<int>  target(nr,nc);
        Matrix<int>  seqtgt(nr,nc);
        Matrix<int>  partgt(nr,nc);



        std::cerr << std::endl;
        std::cerr << "MatMul" << std::endl;
        Matrix<int>  lhs(nr,ns);
        {
            Matrix<int>  rhs(ns,nc);

            for(size_t i=1;i<=lhs.rows;++i)
                for(size_t j=1;j<=lhs.cols;++j)
                    lhs[i][j] = ran.in<int>(-10,10);

            for(size_t i=1;i<=rhs.rows;++i)
                for(size_t j=1;j<=rhs.cols;++j)
                    rhs[i][j] = ran.in<int>(-10,10);

            std::cerr << "lhs=" << lhs << std::endl;
            std::cerr << "rhs=" << rhs << std::endl;
            Tao::MatMul(target,lhs,rhs,xma);
            std::cerr << "target=" << target << std::endl;

            Tao::MatMul(seqtgt,lhs,rhs,xma,seq);
            Y_CHECK(target==seqtgt);
            std::cerr << "seqtgt=" << seqtgt << std::endl;

            Tao::MatMul(partgt,lhs,rhs,xma,par);
            std::cerr << "partgt=" << partgt << std::endl;
            Y_CHECK(target==partgt);

        }

        std::cerr << std::endl;
        std::cerr << "MatMul/Transpose" << std::endl;
        {
            Matrix<int> rhs(nc,ns);
            for(size_t i=1;i<=rhs.rows;++i)
                for(size_t j=1;j<=rhs.cols;++j)
                    rhs[i][j] = ran.in<int>(-10,10);
            std::cerr << "lhs=" << lhs << std::endl;
            std::cerr << "rhs=" << rhs << std::endl;

            target.ld(0);
            Tao::MatMul(target,lhs,TransposeOf,rhs,xma);
            std::cerr << "target=" << target << std::endl;

            seqtgt.ld(0);
            Tao::MatMul(seqtgt,lhs,TransposeOf,rhs,xma,seq);
            std::cerr << "seqtgt=" << seqtgt << std::endl;
            Y_CHECK(seqtgt==target);

            partgt.ld(0);
            Tao::MatMul(partgt,lhs,TransposeOf,rhs,xma,seq);
            std::cerr << "partgt=" << partgt << std::endl;
            Y_CHECK(partgt==target);
        }

#if 1
        std::cerr << std::endl;
        std::cerr << "DiagMatMul" << std::endl;
        {
            Matrix<int> rhs(nr,nc);
            for(size_t i=1;i<=rhs.rows;++i)
                for(size_t j=1;j<=rhs.cols;++j)
                    rhs[i][j] = ran.in<int>(-10,10);
            Vector<int> arr(nr,0);
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i] = ran.in<int>(-10,10);
            }

            target.ld(0);
            Tao::DiagMatMul(target,arr,rhs);
            std::cerr << "arr=diagm(" << arr << ")" << std::endl;
            std::cerr << "rhs=" << rhs << std::endl;
            std::cerr << "target=" << target << std::endl;

            seqtgt.ld(0);
            Tao::DiagMatMul(seqtgt,arr,rhs,seq);
            std::cerr << "seqtgt=" << seqtgt << std::endl;
            Y_CHECK(seqtgt==target);

            partgt.ld(0);
            Tao::DiagMatMul(partgt,arr,rhs,par);
            std::cerr << "partgt=" << partgt << std::endl;
            Y_CHECK(partgt==target);
        }
#endif
    }
}
Y_UDONE()

