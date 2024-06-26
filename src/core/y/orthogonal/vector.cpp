
#include "y/orthogonal/vector.hpp"
#include "y/kemp/ops/univocal.hpp"

namespace Yttrium
{


    namespace Orthogonal
    {
        const char * const Vector:: DerivedCallSign = "Ortho::Vector";

        const char * Vector:: callSign() const noexcept { return DerivedCallSign; }

        Vector:: ~Vector() noexcept
        {
        }

        Vector:: Vector(const Vector &other) :
        Identifiable(), Collection(),
        ObjectType(),
        Metrics(other),
        VectorType(other),
        norm2(other.norm2),
        norm1(other.norm1),
        numPos(other.numPos),
        numNeg(other.numNeg),
        ncoeff(other.ncoeff),
        next(0),
        prev(0)
        {
        }



        Vector:: Vector(const size_t dims) :
        ObjectType(),
        Metrics(dims),
        VectorType(dimensions),
        norm2(0),
        norm1(0),
        numPos(0),
        numNeg(0),
        ncoeff(0),
        next(0),
        prev(0)
        {
            assert(dims>0);
        }

        Vector:: Vector(QArrayType &wksp) :
        ObjectType(),
        Metrics(wksp.size()),
        VectorType( dimensions ),
        norm2(0),
        norm1(0),
        numPos(0),
        numNeg(0),
        ncoeff(0),
        next(0),
        prev(0)
        {
            update(wksp);
        }



        void  Vector:: clear() noexcept
        {
            for(size_t i=dimensions;i>0;--i)
                Coerce( (*this)[i] ).ldz();
            Coerce(norm2).ldz();
        }



        void  Vector:: update(QArrayType &Q)
        {
            try
            {
                //----------------------------------------------------------
                // sanity check
                //----------------------------------------------------------
                assert(size()>0);
                assert(Q.size()==size());

                //----------------------------------------------------------
                // initialize norm2
                //----------------------------------------------------------
                apn    &s2 = ( Coerce(norm2) = 0 );

                //----------------------------------------------------------
                // initialize norm1
                //----------------------------------------------------------
                apn    &s1 = ( Coerce(norm1) = 0);

                size_t     &np = ( Coerce(numPos) = 0);
                size_t     &nn = ( Coerce(numNeg) = 0);
                size_t     &nt = ( Coerce(ncoeff) = 0);

                //----------------------------------------------------------
                // make univocal Q
                //----------------------------------------------------------
                // Mylar::Univocal(Q);
                Kemp::Univocal::Make(Q);

                //----------------------------------------------------------
                // transfer numerators
                //----------------------------------------------------------
                for(size_t i=dimensions;i>0;--i)
                {
                    assert(1==Q[i].denom);
                    const apz &z = (Coerce( (*this)[i] ) = Q[i].numer);
                    switch(z.s)
                    {
                        case Negative: ++nn; ++nt; break;
                        case Positive: ++np; ++nt; break;
                        case __Zero__: continue;
                    }
                    s1 += z.n;
                    s2 += z.n * z.n;
                }
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

        std::ostream & operator<<(std::ostream &os, const Orthogonal::Vector &v)
        {
            const Readable<const apz> &self = v;
            os << self << " #@" << v.norm2 << ":" << v.norm1;
            return os;
        }

        bool Vector:: computeOrtho(Writable<apq> &Q) const
        {
            const Readable<const apz> &B = *this; assert(B.size()==Q.size());
            const size_t n    = dimensions;
            apq          coef = B[1] * Q[1];
            for(size_t i=n;i>1;--i)
                coef += B[i] * Q[i];
            coef /= norm2;

            bool success = false;
            for(size_t i=n;i>0;--i)
            {
                const apq &q = (Q[i] -= coef * B[i]);
                if(q.numer.s!=__Zero__) success = true;
            }
            return success;
        }


        apz operator* (const Vector &lhs, const Vector &rhs)
        {
            apz sum = 0;
            assert(lhs.size()==rhs.size());
            for(size_t i=lhs.size();i>0;--i)
            {
                sum += lhs[i] * rhs[i];
            }
            return sum;
        }
    }



}


