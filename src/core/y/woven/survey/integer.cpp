
#include "y/woven/survey/integer.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{

    namespace WOVEn
    {
        IntegerArray:: ~IntegerArray() noexcept
        {
        }

        IntegerArray:: IntegerArray(const QVector &qvec) :
        Object(),
        IntegerArrayType(CopyOf,qvec),
        norm1(qvec.norm1),
        order(qvec.ncoeff),
        next(0),
        prev(0)
        {
        }
    }


    namespace WOVEn
    {

        IntegerSurvey:: ~IntegerSurvey() noexcept
        {
        }

        IntegerSurvey:: IntegerSurvey(const bool v) :
        Survey(v),
        IntegerArray::List(),
        QSurvey(this,&IntegerSurvey::tryGrow)
        {
        }

        void IntegerSurvey:: tryGrow(const QVector &vec)
        {
            if(vec.ncoeff<=1) return;

            for(const IntegerArray *arr=head;arr;arr=arr->next)
            {
                if(*arr==vec)
                {
                    if(verbose)
                    {
                        std:: cerr << "rejecting multiple " << vec << std::endl;
                    }
                    return;
                }
            }

            pushTail( new IntegerArray(vec) );

        }

        static inline SignType CompareIntegerArrays(const IntegerArray *lhs, const IntegerArray *rhs) noexcept
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
        
        void IntegerSurvey:: sort() noexcept
        {
            MergeSort::Call(*this,CompareIntegerArrays);
        }


    }

}


