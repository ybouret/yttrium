
//! \file

#ifndef Y_WOVEn_Unsigned_Survey_Included
#define Y_WOVEn_Unsigned_Survey_Included 1

#include "y/woven/types.hpp"
#include "y/woven/survey.hpp"

namespace Yttrium
{
    namespace WOVEn
    {
        //______________________________________________________________________
        //
        //
        //! base type for UArray
        //
        //______________________________________________________________________
        typedef CxxArray<const apn,Memory::Pooled> NaturalArrayType;

        //______________________________________________________________________
        //
        //
        //
        //! Array (!=0) of Natural
        //
        //
        //______________________________________________________________________
        class NaturalArray : public Object, public NaturalArrayType
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<NaturalArray> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit NaturalArray(const QVector &); //!< copy from QVector
            virtual ~NaturalArray() noexcept;       //!< cleanup

            //! check equality of heterogeneous vectors
            bool equals(const QVector &) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const apn     norm1; //!< |this|_1 > 0
            const size_t  order; //!< number of coefficient
            NaturalArray *next;  //!< for list
            NaturalArray *prev;  //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(NaturalArray);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Survey for Unsigned
        //
        //
        //______________________________________________________________________
        class NaturalSurvey : public Survey, public NaturalArray::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit NaturalSurvey(XMLog &);   //!< setup empty
            virtual ~NaturalSurvey() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual size_t dimensions() const noexcept; //!< from first array
            void           sort()             noexcept; //!< sort by norm1 then lexicographic
            virtual size_t maxOrder()   const noexcept; //!< maximum order

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NaturalSurvey);
            virtual void verify(const QVector &);
            
        };
    }

}


#endif

