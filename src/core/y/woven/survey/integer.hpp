//! \file

#ifndef Y_WOVEn_Integer_Survey_Included
#define Y_WOVEn_Integer_Survey_Included 1

#include "y/woven/types.hpp"
#include "y/woven/survey.hpp"

namespace Yttrium
{

    namespace WOVEn
    {
        //______________________________________________________________________
        //
        //
        //! base type for IArray
        //
        //______________________________________________________________________
        typedef CxxArray<const apz,Memory::Pooled> IntegerArrayType;

        //______________________________________________________________________
        //
        //
        //
        //! Array (!=0) of Integers
        //
        //
        //______________________________________________________________________
        class IntegerArray : public Object, public IntegerArrayType
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<IntegerArray> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit IntegerArray(const QVector &); //!< copy from QVector
            virtual ~IntegerArray() noexcept;       //!< cleanup
            

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const apn       norm1; //!< |this|_1 > 0
            const size_t    order; //!< num coeff
            IntegerArray   *next;  //!< for list
            IntegerArray   *prev;  //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IntegerArray);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Survey for Integers
        //
        //
        //______________________________________________________________________
        class IntegerSurvey : public Survey, public IntegerArray::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit IntegerSurvey(XMLog &);    //!< setup empty
            virtual ~IntegerSurvey() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual size_t dimensions() const noexcept; //!< [Survey] from first array
            virtual size_t maxOrder()   const noexcept; //!< [Survey] maximum order

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void           sort()             noexcept; //!< sort by norm1 then lexicographic
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(IntegerSurvey);
            virtual void verify(const QVector &);
        };
    }

}


#endif

