

#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"
#include "y/counting/permutation.hpp"

using namespace Yttrium;
using namespace Apex;

static inline void GenVec(Writable<int> &v, Random::Bits &ran, const unsigned nbit)
{
    for(size_t i=v.size();i>0;--i)
    {
        v[i] = ran.to<unsigned>(nbit);
        if(ran.choice()) v[i] = -v[i];
    }
}


static inline void GenVec(Writable<Integer> &v, Random::Bits &ran, const unsigned nbit)
{
    for(size_t i=v.size();i>0;--i) v[i] = Integer(ran,nbit);
}

Y_UTEST(apex_ortho)
{

    Random::ParkMiller ran;
    for(size_t dims=1;dims<=6;++dims)
    {
        std::cerr << std::endl << "dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::VCache         vcache = new Ortho::Vector::Cache(metrics);

        {
            Ortho::Family     F(metrics,vcache);
            Ortho::Family     sub(metrics,vcache);
            CxxArray<int>     V(dims);
            CxxArray<Integer> Z(dims);
            Ortho::Vector     Q(metrics);


            do {
                GenVec(V,ran,5);
                Y_ASSERT(F.includes(sub));
                if(F.wouldAccept(V))
                {
                    sub.recreate(F);

                    F.fetch(Z);
                    F.fetch(Q);
                    //F.fetch(V);

                    F.increase();
                    std::cerr << "F=" << F << std::endl;
                    Y_ASSERT(F.includes(sub));
                    Y_ASSERT(!sub.includes(F));
                }
            } while(F.quality != Ortho::Generating);
            sub.reset();
            std::cerr << "#vcache=" << (*vcache)->size << std::endl;
        }
        std::cerr << "#vcache=" << (*vcache)->size << std::endl;
    }


    // checking equivalence
    for(size_t dims=2;dims<=4;++dims)
    {
        std::cerr << std::endl  << "equivalences in dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::VCache         vcache = new Ortho::Vector::Cache(metrics);
        Ortho::Family         F(metrics,vcache);

        for(size_t sub=1;sub<dims;++sub)
        {
            Matrix<Integer>   M(sub,dims);
            CxxArray<Integer> V(dims);
            F.reset();
            while( F->size < sub )
            {
                GenVec(V,ran,5);
                if(F.wouldAccept(V))
                {
                    F.increase();
                    M[F->size].ld(V);
                }
            }
            std::cerr << "M=" << M << std::endl;

            Permutation perm(sub);

            perm.boot();
            do
            {
                std::cerr << "\t" << perm << std::endl;
                F.reset();
                for(size_t i=1;i<=sub;++i)
                {
                    Y_ASSERT(F.wouldAccept(M[perm[i]]));
                    F.increase();
                }
                std::cerr << "F=" << F << std::endl;


            } while(perm.next());


        }









    }




}
Y_UDONE()
