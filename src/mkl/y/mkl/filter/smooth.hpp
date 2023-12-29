
//! \file

#ifndef Y_MKL_Filter_Smooth_Included
#define Y_MKL_Filter_Smooth_Included 1


#include "y/mkl/api.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {

        
        //______________________________________________________________________
        //
        //
        //
        //! Smooth using low-degree polynomial on local excerpts
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Smooth : public Readable<T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Smooth();
            virtual ~Smooth() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! fit (t,z) at t0 with polynomial of max degree d
            /**
             on output: [1] = smoothed value
                        [2] = smoothed first  derivative
                        [3] = smoothed second derivative
             */
            //__________________________________________________________________
            void  operator()(const T           &t0,
                             const Readable<T> &t,
                             const Readable<T> &z,
                             const size_t       degree);

            //__________________________________________________________________
            //
            //! reserve internal memory to process from 0 to degree
            //__________________________________________________________________
            void reserveMaxDegree(const size_t degree);


            //__________________________________________________________________
            //
            //! reserve internal memory to process points
            //__________________________________________________________________
            void reserveMaxLength(const size_t points);


            //__________________________________________________________________
            //
            //
            // Readable interface
            //
            //__________________________________________________________________
            virtual const char * callSign()               const noexcept; //!< "Smooth"
            virtual size_t       size()                   const noexcept; //!< 3
            virtual const T &    operator[](const size_t) const noexcept; //!< [1:3]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Smooth);
            class Code;
            Code *code;
        };
    }

}

#endif
