

//! \file

#ifndef Y_Matrix_Included
#define Y_Matrix_Included 1

#include "y/container/matrix/metrics.hpp"
#include "y/container/matrix/row.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/embedded.hpp"
#include "y/container/implanted.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{


#define Y_MATRIX_EPILOG() RowsType(), row(0), base(0), code(0)
    //__________________________________________________________________________
    //
    //
    //
    //! Matrix of objects
    //
    //
    //__________________________________________________________________________
    template <class T, typename ALLOCATOR = Memory::Dyadic>
    class Matrix : public MatrixMetrics, public Writable< MatrixRow<T> >
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                //!< aliases
        typedef MatrixRow<T>      RowType;  //!< alias
        typedef Writable<RowType> RowsType; //!< alias
        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup empty
        inline explicit Matrix() noexcept :
        MatrixMetrics(0,0), Y_MATRIX_EPILOG() {}

        //! setup with (possible) data
        inline explicit Matrix(const size_t nr, const size_t nc) :
        MatrixMetrics(nr,nc), Y_MATRIX_EPILOG()
        {
            create();
        }

        inline Matrix(const Matrix &other) :
        MatrixMetrics(other), Y_MATRIX_EPILOG()
        {
            duplicate(other,Identity<T>);
        }

        template <typename U,typename ALLOC, typename TRANSFORM> inline
        Matrix(const Matrix<U,ALLOC> &other, TRANSFORM &transform) :
        MatrixMetrics(other), Y_MATRIX_EPILOG()
        {
            duplicate(other,transform);
        }

        template <typename U,typename ALLOC> inline
        Matrix(const Matrix<U,ALLOC> &other, const TransposeOf_ &_) :
        MatrixMetrics(other,_), Y_MATRIX_EPILOG()
        {
            duplicateTransposeOf(other,Identity<U>);
        }



        //! cleanup
        inline virtual ~Matrix() noexcept {
        }

        inline friend std::ostream & operator<<(std::ostream &os, const Matrix &M)
        {
            switch(M.rows)
            {
                case 0: os << "[]"; break;
                case 1:
                    if(1==M.cols) M.output_1x1(os);
                    else          M.output_1xn(os);
                    break;
                default:
                    if(1==M.cols) M.output_nx1(os);
                    else
                    {
                        os << '[';
                        os << M[1];
                        for(size_t r=2;r<=M.rows;++r)
                            os << ';' << M[r];
                        os << ']';
                    }
                    break;
            }
            return os;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t       size()     const noexcept { return rows;     } //!< rows
        inline virtual const char * callSign() const noexcept { return CallSign; } //!< CallSign

        //! access row
        inline RowType & operator[](const size_t r) noexcept {
            assert(r>=1); assert(r<=rows);
            return row[r];
        }

        //! access const row
        inline const RowType & operator[](const size_t r) const noexcept {
            assert(r>=1); assert(r<=rows);
            return row[r];
        }

        inline LightArray<T> asArray() const noexcept
        {
            return LightArray<T>(base,items);
        }


    private:
        Y_DISABLE_ASSIGN(Matrix);
        class Code;

        RowType      *row;
        MutableType  *base;
        AutoPtr<Code> code;

        void output_1x1(std::ostream &os) const
        {
            assert(1==rows); assert(1==cols); assert(0!=base);
            os << "hcat(" << base[0] << ")";
        }

        void output_1xn(std::ostream &os) const
        {
            assert(1==rows); assert(1<cols);  assert(0!=row);
            os << "[" << row[1] << "]";
        }

        void output_nx1(std::ostream &os) const
        {
            assert(1<rows); assert(1==cols); assert(0!=row);
            os << "hcat(" << row[1] << "')";
        }

        // Code for Matrix
        class Code : public Memory::Embedded
        {
        public:
            explicit Code(Memory::Embed emb[], const size_t nc) :
            Memory::Embedded(emb,NUM_FIELDS,ALLOCATOR::Instance()),
            dataOps(emb[DATA_INDEX]),
            rowInfo(emb[DATA_INDEX].address(),nc),
            rowsOps(emb[ROWS_INDEX],rowInfo)
            {
            }

            inline virtual ~Code() noexcept { }

            Implanted<T>             dataOps;
            Core::MatrixRow::Info    rowInfo;
            Implanted<RowType>       rowsOps;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        // create with default objects
        inline void create()
        {
            if(items<=0) return;
            Memory::Embed emb[] = {
                Memory::Embed(base,items),
                Memory::Embed(row,rows)
            };
            code = new Code(emb,cols);
            --row;
        }

        template <typename U, typename ALLOC, typename TRANSFORM>
        inline void duplicate(const Matrix<U,ALLOC> &M, TRANSFORM &transform)
        {
            assert(hasSameMetricsThan(M));
            create();
            const LightArray<U> source = M.asArray(); assert(source.size()==items);
            LightArray<T>       target = asArray();   assert(target.size()==items);
            for(size_t i=items;i>0;--i)
                target[i] = transform(source[i]);

        }
    };

}

#endif
