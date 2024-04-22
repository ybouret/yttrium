
#include "y/woven/survey/integer.hpp"
#include "y/sort/merge.hpp"
#include "y/type/utils.hpp"

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

        IntegerSurvey:: IntegerSurvey(XMLog &xmlog) :
        Survey(xmlog),
        IntegerArray::List()
         {
        }

        void IntegerSurvey:: verify(const QVector &vec)
        {
            if(vec.ncoeff<=1) return;

            for(const IntegerArray *arr=head;arr;arr=arr->next)
            {
                if(*arr==vec)
                {
                    Y_XMLOG(xml, " (-) " << vec);
                    return;
                }
            }
            
            Y_XMLOG(xml, " (+) " << vec);
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

        size_t IntegerSurvey:: maxOrder() const noexcept
        {
            size_t res = 0;
            for(const IntegerArray *arr=head;arr;arr=arr->next)
            {
                res = Max(res,arr->order);
            }
            return res;
        }

    }

}


