//! \file

#ifndef Y_WOVEn_Integer_Survey_Included
#define Y_WOVEn_Integer_Survey_Included 1

#include "y/woven/types.hpp"

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
        typedef CxxArray<const apz,Memory::Pooled> IArrayType;

        //______________________________________________________________________
        //
        //
        //
        //! Array (!=0) of Integers
        //
        //
        //______________________________________________________________________
        class IArray : public Object, public IArrayType
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<IArray> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit IArray(const QVector &); //!< copy from QVector
            virtual ~IArray() noexcept;       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const apn norm1; //!< |this|_1 > 0
            IArray   *next;  //!< for list
            IArray   *prev;  //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IArray);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Survey for Integers
        //
        //
        //______________________________________________________________________
        class ISurvey : public IArray::List, public QSurvey
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ISurvey();           //!< setup empty
            virtual ~ISurvey() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! sort by norm1 the lexicographic
            void sort() noexcept;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ISurvey);
            void tryGrow(const QVector &);
        };
    }

}


#endif

