//! \file

#ifndef Y_MKL_Stats_Metrics_Included
#define Y_MKL_Stats_Metrics_Included 1

#include "y/mkl/tao/seq/level1.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Average of a range
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T Average(ITERATOR it, size_t n, Antelope::Add<T> &xadd)
            {
                switch(n)
                {
                    case 0: return T(0);
                    case 1: return T(*it);
                    default:
                        break;
                }
                const T den = n;
                xadd.free();
                while(n-- > 0)
                    xadd << *(it++);
                return xadd.sum()/den;
            }

            //__________________________________________________________________
            //
            //
            //! Average of a sequence
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T Average(SEQUENCE &seq, Antelope::Add<T> &xadd)
            {
                return Average(seq.begin(),seq.size(),xadd);
            }
        }

        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Variance of a range with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T Variance(ITERATOR it, size_t n, Antelope::Add<T> &xadd, const T &average)
            {
                switch(n)
                {
                    case 0:
                    case 1:
                        return T(0);
                    default:
                        break;
                }
                const T den(n-1);
                xadd.free();
                while(n-- > 0)
                {
                    const T delta = *(it++) - average;
                    xadd << delta*delta;
                }
                return xadd.sum()/den;
            }

            //__________________________________________________________________
            //
            //
            //! Variance of a sequence with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T Variance(SEQUENCE &seq, Antelope::Add<T> &xadd, const T &average)
            {
                return Variance( seq.begin(), seq.size(), xadd, average);
            }
        }

        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Standard Deviation of a range with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T StandardDeviation(ITERATOR it, size_t n, Antelope::Add<T> &xadd, const T &average)
            {
                const T _ = Variance(it,n,xadd,average);
                return Sqrt<T>::Of(_);
            }

            //__________________________________________________________________
            //
            //
            //! Standard Deviation of a sequence with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T StandardDeviation(SEQUENCE &seq, Antelope::Add<T> &xadd, const T &average)
            {
                return StandardDeviation( seq.begin(), seq.size(), xadd, average);
            }
        }

        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Absolute Deviation of a range with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T AbsoluteDeviation(ITERATOR it, size_t n, Antelope::Add<T> &xadd, const T &average)
            {
                switch(n)
                {
                    case 0:
                        return T(0);
                    default:
                        break;
                }
                const T den(n);
                xadd.free();
                while(n-- > 0)
                {
                    const T delta = *(it++) - average;
                    xadd << Fabs<T>::Of(delta);
                }
                return xadd.sum()/den;
            }

            //__________________________________________________________________
            //
            //
            //! Absolute Deviation of a sequence with pre-computed average
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T AbsoluteDeviation(SEQUENCE &seq, Antelope::Add<T> &xadd, const T &average)
            {
                return AbsoluteDeviation( seq.begin(), seq.size(), xadd, average);
            }
        }

        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Skewness of a range with pre-computed average and stddev
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T Skewness(ITERATOR          it,
                       size_t            n,
                       Antelope::Add<T> &xadd,
                       const T          &average,
                       const T          &standardDeviation)
            {
                switch(n)
                {
                    case 0:
                        return T(0);
                    default:
                        break;
                }
                const T den(n);
                xadd.free();
                while(n-- > 0)
                {
                    const T delta = (*(it++) - average)/standardDeviation;
                    const T delta2 = delta*delta;
                    xadd << delta*delta2;
                }
                return xadd.sum()/den;
            }

            //__________________________________________________________________
            //
            //
            //! Skewness of a sequence with pre-computed average and stddev
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T Skewness(SEQUENCE         &seq,
                       Antelope::Add<T> &xadd,
                       const T          &average,
                       const T          &standardDeviation)
            {
                return Skewness(seq.begin(), seq.size(), xadd, average, standardDeviation);
            }
        }

        namespace Statistics
        {
            //__________________________________________________________________
            //
            //
            //! Kurtosis of a range with pre-computed average and stddev
            //
            //__________________________________________________________________
            template <typename T,typename ITERATOR> inline
            T Kurtosis(ITERATOR          it,
                       size_t            n,
                       Antelope::Add<T> &xadd,
                       const T          &average,
                       const T          &standardDeviation)
            {
                switch(n)
                {
                    case 0:
                        return T(0);
                    default:
                        break;
                }
                const T three(3);
                const T den(n);
                xadd.free();
                while(n-- > 0)
                {
                    const T delta = (*(it++) - average)/standardDeviation;
                    const T delta2 = delta*delta;
                    xadd << delta2*delta2;
                }
                return xadd.sum()/den-three;
            }

            //__________________________________________________________________
            //
            //
            //! Kurtosis of a sequence with pre-computed average and stddev
            //
            //__________________________________________________________________
            template <typename T,typename SEQUENCE> inline
            T Kurtosis(SEQUENCE         &seq,
                       Antelope::Add<T> &xadd,
                       const T          &average,
                       const T          &standardDeviation)
            {
                return Kurtosis(seq.begin(), seq.size(), xadd, average, standardDeviation);
            }
        }
    }
}

#endif

