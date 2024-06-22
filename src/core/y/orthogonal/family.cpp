
#include "y/orthogonal/family.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{

    namespace Orthogonal
    {
        Y_SHALLOW_IMPL(Basis);

        Family:: ~Family() noexcept
        {
        }

        Family:: Family(const size_t dims) noexcept :
        Metrics(dims),
        Vectors(),
        remaining(dimensions)
        {

        }

        Family:: Family(const Family &other) :
        Metrics(other),
        Vectors(other),
        remaining(dimensions)
        {
        }

        bool Family:: wouldAccept( )
        {
            assert(size<dimensions);

            // checking remaining != 0
            for(size_t i=dimensions;i>0;--i)
            {
                switch(remaining[i].numer.s)
                {
                    case __Zero__: continue;
                    case Negative:
                    case Positive:
                        goto CHECK;
                }
            }
            return false;

        CHECK:
            for(const Vector *vec=head;vec;vec=vec->next)
            {
                if(vec->computeOrtho(remaining)) continue;
                return false;
            }

            return true;
        }

        const Vector & Family:: expand()
        {
            assert(size<dimensions);
            pushTail( new Vector(remaining) );

            assert(0!=tail);
            assert(tail->norm2>0);

            return *tail;
        }


        const Vector & Family:: expandFrom( QArrayType &arr )
        {
            assert(size<dimensions);
            assert( &arr != &remaining);

            assert(wouldAccept(arr));

            pushTail( new Vector(arr) );

            assert(0!=tail);
            assert(tail->norm2>0);


            return *tail;
        }


        std::ostream & operator<<(std::ostream &os, const Family &F)
        {
            if(F.size)
            {
                Matrix<apz> M(F.size,F.dimensions);
                size_t i=1;
                for(const Vector *v=F.head;v;v=v->next,++i)
                {
                    for(size_t j=F.dimensions;j>0;--j) M[i][j] = (*v)[j];
                }
                os << M;
            }
            else
            {
                const Matrix<int> none;
                os << none;
            }
            return os;
        }


        bool Family:: contains(const Vectors &vecs)
        {
            for(const Vector *vec=vecs.head;vec;vec=vec->next)
            {
                assert(dimensions==vec->dimensions);
                if(wouldAccept(*vec)) return false;
            }
            return true;
        }


    }
    

}
