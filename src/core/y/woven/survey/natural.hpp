
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
        class NaturalSurvey : public Survey, public NaturalArray::List, public QSurvey
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

            //! sort by norm1 the lexicographic
            void sort() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(NaturalSurvey);
            void tryGrow(const QVector &);
            
        };
    }

}


#endif

