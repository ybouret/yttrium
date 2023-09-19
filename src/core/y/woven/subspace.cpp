
#include "y/woven/subspace.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        Subspace:: ~Subspace() noexcept {}

        const char * const Subspace::CallSign = "WOVEn::Subspace";

        size_t Subspace:: CheckDims(const MatrixMetrics &mu)
        {
            if(mu.cols<=0) throw Specific::Exception(CallSign,"Matrix #cols=0");
            return mu.cols;
        }


        size_t Subspace:: CheckDOFs(const MatrixMetrics &mu)
        {
            if(mu.rows<=0) throw Specific::Exception(CallSign,"Matrix #rows=0");
            return mu.rows;
        }

        void Subspace:: SingularFirstRowException()
        {
            throw Specific::Exception(CallSign,"Singular first row!");
        }

        void Subspace:: initializeWith(const size_t ir, const size_t nr)
        {
            qfamily.expand();
            indices += ir;
            for(size_t i=1;    i<ir;  ++i) staying += i;
            for(size_t i=ir+1; i<=nr; ++i) staying += i;
        }


        std::ostream & operator<<(std::ostream &os, const Subspace &sp)
        {
            os << sp.qfamily << "@{" << sp.indices << "} <== {" << sp.staying << "}";
            return os;
        }


        bool Subspace:: merged( AutoPtr<Subspace> &source )
        {
            assert(source.isValid());
            assert( this != & *source );

            assert(qfamily.size == source->qfamily.size );

            if( indices.contains(source->indices) || qfamily.contains(source->qfamily))
            {
                // different indices but includes sub-space
                indices |= source->indices; // merge indices
                staying |= source->staying; // merge staying
                staying ^= indices;         // update stayin
                source.dismiss();
                return true;
            }

            return false;
        }

        Subspace:: Subspace(const Subspace &other) :
        Object(),
        QMetrics(other),
        qfamily(other.qfamily),
        indices(other.indices),
        staying(other.staying),
        next(0),
        prev(0)
        {
        }


        void Subspace:: expand(List &L, const size_t ir, QSurvey *survey) 
        {
            AutoPtr<Subspace> sub = new Subspace(*this);
            const QVector &   vec = sub->qfamily.expandFrom( qfamily.remaining );
            sub->indices += ir;
            sub->staying -= ir;
            if(survey) (*survey)(vec);
            if(sub->staying.size()>0)
                L.pushTail( sub.yield() );
        }

        

    }

}
