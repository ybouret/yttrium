
//! \file

#ifndef Y_WOVEn_Unsigned_Survey_Included
#define Y_WOVEn_Unsigned_Survey_Included 1

#include "y/woven/types.hpp"

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
        typedef CxxArray<const apn,Memory::Pooled> UArrayType;

        //______________________________________________________________________
        //
        //
        //
        //! Array (!=0) of Unsigned
        //
        //
        //______________________________________________________________________
        class UArray : public Object, public UArrayType
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<UArray> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit UArray(const QVector &); //!< copy from QVector
            virtual ~UArray() noexcept;       //!< cleanup

            bool equals(const QVector &) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const apn norm1; //!< |this|_1 > 0
            UArray   *next;  //!< for list
            UArray   *prev;  //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(UArray);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Survey for Unsigned
        //
        //
        //______________________________________________________________________
        class USurvey : public UArray::List, public QSurvey
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit USurvey();           //!< setup empty
            virtual ~USurvey() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! sort by norm1 the lexicographic
            void sort() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(USurvey);
            void tryGrow(const QVector &);
            
        };
    }

}


#endif

