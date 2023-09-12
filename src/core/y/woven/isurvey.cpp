
#include "y/woven/isurvey.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{

    namespace WOVEn
    {
        IArray:: ~IArray() noexcept
        {
        }

        IArray:: IArray(const QVector &qvec) :
        Object(),
        IArrayType(CopyOf,qvec),
        norm1(qvec.norm1),
        next(0),
        prev(0)
        {
        }
    }


    namespace WOVEn
    {

        ISurvey:: ~ISurvey() noexcept
        {
        }

        ISurvey:: ISurvey() :
        IArray::List(),
        QSurvey(this,&ISurvey::tryGrow)
        {
        }

        void ISurvey:: tryGrow(const QVector &vec)
        {
            if(vec.norm1<=0) return;

            for(const IArray *arr=head;arr;arr=arr->next)
            {
                if(*arr==vec) return;
            }

            pushTail( new IArray(vec) );

        }

        static inline SignType CompareIArrays(const IArray *lhs, const IArray *rhs) noexcept
        {
            assert(0!=lhs);
            assert(0!=rhs);
            switch( Sign::Of(lhs->norm1,rhs->norm1) )
            {
                case Negative: return Negative;
                case __Zero__: break;
                case Positive: return Positive;
            }
            return Readable<const apz>::LexicographicCompare(*lhs,*rhs);
        }
        
        void ISurvey:: sort() noexcept
        {
            MergeSort::Call(*this,CompareIArrays);
        }


    }

}


