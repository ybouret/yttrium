

#include "y/woven/usurvey.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{

    namespace WOVEn
    {
        UArray:: ~UArray() noexcept
        {
        }

        UArray:: UArray(const QVector &qvec) :
        Object(),
        UArrayType(qvec.size()),
        norm1(qvec.norm1),
        next(0),
        prev(0)
        {
            for(size_t i=size();i>0;--i)
            {
                const apz &z = qvec[i];
                assert(z>=0);
                Coerce((*this)[i]) = z.n;
            }
        }


        bool UArray:: equals(const QVector &v) const noexcept
        {
            assert(size()==v.size());
            for(size_t i=size();i>0;--i)
            {
                if(v[i] != (*this)[i] ) return false;
            }

            return true;
        }
    }


    namespace WOVEn
    {

        USurvey:: ~USurvey() noexcept
        {
        }

        USurvey:: USurvey() :
        UArray::List(),
        QSurvey(this,&USurvey::tryGrow)
        {
        }

        void USurvey:: tryGrow(const QVector &vec)
        {
            if(vec.norm1<=0) return;
            for(size_t i=vec.size();i>0;--i)
            {
                if(vec[i] < 0) return;
            }

            for(const UArray *arr=head;arr;arr=arr->next)
            {
                if(arr->equals(vec)) return;
            }

            pushTail( new UArray(vec) );

        }

        static inline SignType CompareUArrays(const UArray *lhs, const UArray *rhs) noexcept
        {
            assert(0!=lhs);
            assert(0!=rhs);
            switch( Sign::Of(lhs->norm1,rhs->norm1) )
            {
                case Negative: return Negative;
                case __Zero__: break;
                case Positive: return Positive;
            }
            return Readable<const apn>::LexicographicCompare(*lhs,*rhs);
        }

        void USurvey:: sort() noexcept
        {
            MergeSort::Call(*this,CompareUArrays);
        }


    }

}


