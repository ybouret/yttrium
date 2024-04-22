

#include "y/woven/survey/natural.hpp"
#include "y/sort/merge.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace WOVEn
    {
        NaturalArray:: ~NaturalArray() noexcept
        {
        }

        NaturalArray:: NaturalArray(const QVector &qvec) :
        Object(),
        NaturalArrayType(qvec.size()),
        norm1(qvec.norm1),
        order(qvec.ncoeff),
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


        bool NaturalArray:: equals(const QVector &v) const noexcept
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

        NaturalSurvey:: ~NaturalSurvey() noexcept
        {
        }

        NaturalSurvey:: NaturalSurvey(XMLog &xmlog) :
        Survey(xmlog),
        NaturalArray::List()
        {
        }

        void NaturalSurvey:: verify(const QVector &vec)
        {

            if(vec.numNeg>0)  return;
            if(vec.numPos<=1) return;

            for(const NaturalArray *arr=head;arr;arr=arr->next)
            {
                if(arr->equals(vec))
                {
                    Y_XMLOG(xml, " (-) " << vec);
                    return;
                }
            }

            Y_XMLOG(xml, " (+) " << vec);
            pushTail( new NaturalArray(vec) );

        }

        static inline SignType CompareNaturalArrays(const NaturalArray *lhs, const NaturalArray *rhs) noexcept
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

        void NaturalSurvey:: sort() noexcept
        {
            MergeSort::Call(*this,CompareNaturalArrays);
        }

        size_t NaturalSurvey:: maxOrder() const noexcept
        {
            size_t res = 0;
            for(const NaturalArray *arr=head;arr;arr=arr->next)
            {
                res = Max(res,arr->order);
            }
            return res;
        }
    }

}


