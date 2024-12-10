
//! \file

#ifndef Y_WOVEn_Survey_Included
#define Y_WOVEn_Survey_Included 1

#include "y/woven/types.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for a Survey
        //
        //
        //______________________________________________________________________
        class Survey
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual size_t dimensions() const noexcept = 0; //!< get dimension from first vector
            virtual void   verify(const QVector &)     = 0; //!< verify and insert vector
            virtual size_t maxOrder() const noexcept   = 0; //!< get maximum order

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Survey() noexcept;        //!< cleanup
        protected:
            explicit Survey(XMLog &) noexcept; //!< setup

        public:
            XMLog &xml; //!< output helper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Survey);
        };

        //______________________________________________________________________
        //
        //
        //
        //! helper to convert array to integral types
        //
        //
        //______________________________________________________________________
        template <typename T>
        class ArrayOf : public Object, public CxxArray<T,Memory::Pooled>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<T,Memory::Pooled> BaseType; //!< alias
            typedef CxxListOf<ArrayOf>         List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! get memory and convert
            template <typename ARRAY> inline
            explicit ArrayOf(ARRAY &arr, const char * const which) :
            Object(),
            BaseType( arr.size() ),
            order(    arr.order  )
            {
                BaseType &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i] = arr[i].template cast<T>(which);

            }

            //! cleanup
            inline virtual ~ArrayOf() noexcept {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t order; //!< number of coefficients
            ArrayOf *    next;  //!< for list
            ArrayOf *    prev;  //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArrayOf);
        };


        //______________________________________________________________________
        //
        //
        //
        //! helper to convert a survey to integral type
        //
        //
        //______________________________________________________________________
        template <typename T>
        class ArraysOf : public Object, public ArrayOf<T>::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArrayOf<T>               ArrayType; //!< alias
            typedef typename ArrayType::List ListType;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! duplicate and convert survey, keeping only above given minOrder
            template <typename SURVEY>
            inline explicit ArraysOf(const SURVEY       &survey,
                                     const char * const  which,
                                     const size_t        minOrder) :
            Object(),
            ListType()
            {
                for(const typename SURVEY::NodeType *arr=survey.head;arr;arr=arr->next)
                {
                    if(arr->order<minOrder) continue;
                    this->pushTail( new ArrayType(*arr,which) );
                }
            }

            //! cleanup
            inline virtual ~ArraysOf() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArraysOf);
        };
    }

}

#endif
