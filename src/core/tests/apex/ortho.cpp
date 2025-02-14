

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
                if(F.welcomes(V))
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
    for(size_t dims=2;dims<=5;++dims)
    {
        std::cerr << std::endl  << "equivalences in dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::VCache         vcache = new Ortho::Vector::Cache(metrics);
        Ortho::Family         F(metrics,vcache);

        for(size_t sub=1;sub<dims;++sub)
        {
            Matrix<Integer>   M(sub,dims);
            CxxArray<Integer> V(dims),  first(dims), extra(dims);

            F.reset();
            while( F->size < sub )
            {
                GenVec(V,ran,5);
                if(F.welcomes(V))
                {
                    F.increase();
                    M[F->size].ld(V);
                }
            }
            std::cerr << "M=" << M << std::endl;
            do
            {
                GenVec(V,ran,10);
            } while( !F.welcomes(V) );
            F.fetch(first);
            std::cerr << "first=" << first << std::endl;

            Permutation perm(sub);

            perm.boot();
            do
            {
                // build a family from a permutation of trials
                std::cerr << "\t" << perm << std::endl;
                F.reset();
                for(size_t i=1;i<=sub;++i)
                {
                    Y_ASSERT(F.welcomes(M[perm[i]]));
                    F.increase();
                }
                std::cerr << "F=" << F << std::endl;

                // check that order doesn't matter for next projection
                Y_ASSERT(F.welcomes(V));
                F.fetch(extra);
                //std::cerr << "extra=" << extra << std::endl;
                Y_ASSERT(extra==first);

                if(Ortho::Hyperplane == F.quality)
                {
                    // check that any final vector is the same...
                    CxxArray<Integer> U(dims), curr(dims), next(dims);
                    do
                    {
                        GenVec(U,ran,10);
                    } while( !F.welcomes(U) );
                    F.fetch(curr);
                    std::cerr << "\t" << curr << " <-- " << V << std::endl;
                    for(size_t iter=0;iter<10;++iter)
                    {
                        do
                        {
                            GenVec(U,ran,10);
                        } while( !F.welcomes(U) );
                        F.fetch(next);
                        //std::cerr << "\t" << next << " <-- " << V << std::endl;
                        Y_ASSERT(curr==next);
                    }
                }

            } while(perm.next());


        }

    }

    

}
Y_UDONE()
