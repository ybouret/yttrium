
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
            order(    arr.order  ),
            next(0),
            prev(0)
            {
                BaseType &self = *this;
                for(size_t i=self.size();i>0;--i)
                    self[i] = arr[i].template cast<T>(which);

            }
            //! build a zeroed arrau with given dimensions
            explicit inline ArrayOf(const size_t dims) :
            Object(),
            BaseType(dims),
            order(0),
            next(0),
            prev(0)
            {
            }


            //! cleanup
            inline virtual ~ArrayOf() noexcept {}

            //! access for self printing
            inline const ArrayOf &operator*() const noexcept { return *this; }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! post-assign order
            inline size_t updateOrder() const noexcept
            {
                const BaseType &self = *this;
                Coerce(order) = 0;
                for(size_t i=self.size();i>0;--i)
                {
                    if( 0 != self[i] ) ++Coerce(order);
                }
                return order;
            }


            //! check if present in matrix
            template <typename MATRIX>
            inline bool isRedundantWith(const MATRIX &matrix) const noexcept
            {
                const ArrayOf &self = *this;
                for(size_t i=1;i<=matrix.rows;++i)
                {
                    if( self == matrix[i] ) return true;
                }
                return false;
            }

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

            //! create linear combination of matrix, check validity and not redundant
            template <typename MATRIX>
            inline explicit ArraysOf(const MATRIX   & matrix,
                                     const ArraysOf & weight) :
            Object(),
            ListType()
            {
                const size_t dims = matrix.cols;
                const size_t rows = matrix.rows;
                const MATRIX matneg(CopyOf,matrix,MakeOpposite<typename MATRIX::Type>);
                for(const ArrayType *node=weight.head;node;node=node->next)
                {
                    const ArrayType &w = *node; assert(rows==w.size());
                    ArrayType       &a = * this->pushTail( new ArrayType(dims) );

                    for(size_t j=dims;j>0;--j)
                    {
                        for(size_t i=rows;i>0;--i)
                        {
                            a[j] += matrix[i][j] * w[i];
                        }
                    }

                    if( a.updateOrder() <= 0 || a.isRedundantWith(matrix) || a.isRedundantWith(matneg) ||  alreadyComputed() )
                    {
                        delete this->popTail();
                        continue;
                    }
                }
            }

            //! cleanup
            inline virtual ~ArraysOf() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArraysOf);

            template <typename I>
            static inline I MakeOpposite(const I &i) { return -i; }

            inline bool alreadyComputed() const noexcept
            {
                assert(this->size>0);
                const ArrayType * const curr = this->tail;
                for(const ArrayType *node=curr->prev;node;node=node->prev)
                {
                    if( *node == *curr) return true;
                }
                return false;
            }



        };
    }

}

#endif
