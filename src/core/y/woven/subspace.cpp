
#include "y/woven/subspace.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        SubSpace:: ~SubSpace() noexcept {}

        const char * const SubSpace::CallSign = "WOVEn::SubSpace";

        size_t SubSpace:: CheckDims(const MatrixMetrics &mu)
        {
            if(mu.cols<=0) throw Specific::Exception(CallSign,"Matrix #cols=0");
            return mu.cols;
        }


        size_t SubSpace:: CheckDOFs(const MatrixMetrics &mu)
        {
            if(mu.rows<=0) throw Specific::Exception(CallSign,"Matrix #rows=0");
            return mu.rows;
        }

        void SubSpace:: SingularFirstRowException()
        {
            throw Specific::Exception(CallSign,"Singular first row!");
        }

        void SubSpace:: initializeWith(const size_t ir, const size_t nr)
        {
            expand();
            indices += ir;
            for(size_t i=1;    i<ir;  ++i) staying += i;
            for(size_t i=ir+1; i<=nr; ++i) staying += i;
        }


        std::ostream & operator<<(std::ostream &os, const SubSpace &sp)
        {
            const QFamily &f = sp;
            os << f << "@{" << sp.indices << "} <== {" << sp.staying << "}";
            return os;
        }


        bool SubSpace:: merged( AutoPtr<SubSpace> &source )
        {
            assert(source.isValid());
            assert( this != & *source );

            assert(size == source->size );

            if( indices.contains(source->indices) ||  contains(*source) )
            {
                // different indices but includes sub-space
                indices |= source->indices; // merge  indices
                staying |= source->staying; // merge  staying
                staying ^= indices;         // update staying
                source.dismiss();
                return true;
            }

            return false;
        }

        SubSpace:: SubSpace(const SubSpace &other) :
        Object(),
        QFamily(other),
        indices(other.indices),
        staying(other.staying),
        quality(other.quality),
        next(0),
        prev(0)
        {
        }


        void SubSpace:: doExpand(List &        L,
                                 const size_t  ir,
                                 QSurvey      *survey)
        {
            AutoPtr<SubSpace> sub = new SubSpace(*this);           // duplicate in sub
            const QVector &   vec = sub->expandFrom( remaining );  // expand sub with this->remainng
            sub->indices += ir;                                    // update indices
            sub->staying -= ir;                                    // update staying
            if(survey) (*survey)(vec);                             // take survey if any
            if(sub->staying.size()<=0) return;                     // it was the final trial
            switch( (Coerce(sub->quality) = sub->getQuality(sub->size)) )
            {
                case Apex::Ortho::Generating: return; // won't accept any more
                case Apex::Ortho::Fragmental:
                case Apex::Ortho::Hyperplane:
                    break;
            }
            L.pushTail( sub.yield() );
        }

        void SubSpace:: fullfill()
        {
            assert(size==dimensions);
            assert(Apex::Ortho::Generating==quality);
            indices |= staying;
            staying.free();
        }

    }

}
