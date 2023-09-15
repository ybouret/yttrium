

#include "y/woven/survey/natural.hpp"
#include "y/sort/merge.hpp"

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

        NaturalSurvey:: NaturalSurvey(const bool v) :
        Survey(v),
        NaturalArray::List(),
        QSurvey(this,&NaturalSurvey::tryGrow)
        {
        }

        void NaturalSurvey:: tryGrow(const QVector &vec)
        {

            if(vec.numNeg>0)  return;
            if(vec.numPos<=1) return;

            for(const NaturalArray *arr=head;arr;arr=arr->next)
            {
                if(arr->equals(vec))
                {
                    if(verbose)
                    {
                        std::cerr << "rejecting multiple " << *arr << std::endl;
                    }
                    return;
                }
            }

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


    }

}


