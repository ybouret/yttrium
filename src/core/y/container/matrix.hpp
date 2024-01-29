

//! \file

#ifndef Y_Matrix_Included
#define Y_Matrix_Included 1

#include "y/container/matrix/metrics.hpp"
#include "y/container/matrix/row.hpp"
#include "y/memory/embedded.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/releasable.hpp"
#include "y/container/implanted.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"
#include "y/type/copy.hpp"

namespace Yttrium
{

    //! constructor helper for Matrices
#define Y_MATRIX(TOPO) Identifiable(), Collection(), MatrixMetrics TOPO, RowsType(), Releasable(), row(0), base(0), code(0)

    //__________________________________________________________________________
    //
    //
    //
    //! Matrix of objects
    //
    //
    //__________________________________________________________________________
    template <class T, typename ALLOCATOR = Memory::Dyadic>
    class Matrix : public MatrixMetrics, public Writable< MatrixRow<T> >, public Releasable
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
        Y_MATRIX( (0,0) ) {}

        //! setup with (possible) data
        inline explicit Matrix(const size_t nr, const size_t nc) :
        Y_MATRIX( (nr,nc) )
        {
            create();
        }

        //! copy
        inline Matrix(const Matrix &other) :
        Y_MATRIX( (other) )
        {
            duplicate(other,Identity<T>);
        }

        //! copy another
        template <typename U,typename ALLOC> inline
        Matrix(const CopyOf_ &, const Matrix<U,ALLOC> &other) :
        Y_MATRIX( (other) )
        {
            duplicate(other,Identity<U>);
        }

        //! copy transform
        template <typename U,typename ALLOC, typename TRANSFORM> inline
        Matrix(const CopyOf_ &, const Matrix<U,ALLOC> &other, TRANSFORM &transform) :
        Y_MATRIX( (other) )
        {
            duplicate(other,transform);
        }

        //! copy transposed, using identity
        template <typename U,typename ALLOC> inline
        Matrix( const TransposeOf_ &_, const Matrix<U,ALLOC> &other) :
        Y_MATRIX( (other,_) )
        {
            duplicateTransposeOf(other,Identity<U>);
        }

        //! copy transposed, using transform
        template <typename U,typename ALLOC,typename TRANSFORM> inline
        Matrix(const TransposeOf_ &_, const Matrix<U,ALLOC> &other,  TRANSFORM &transform) :
        Y_MATRIX( (other,_) )
        {
            duplicateTransposeOf(other,transform);
        }



        //! cleanup
        inline virtual ~Matrix() noexcept {
        }


        //! display Julia style
        inline friend std::ostream & operator<<(std::ostream &os, const Matrix &M)
        {
            switch(M.rows)
            {
                case 0: os << "[]"; break;
                case 1:
                    if(1==M.cols) M.julia_1x1(os);
                    else          M.julia_1xn(os);
                    break;
                default:
                    if(1==M.cols) M.julia_nx1(os);
                    else
                    {
                        M[1].Julia(os << '[');
                        for(size_t r=2;r<=M.rows;++r)
                            M[r].Julia(os << ';');
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

        //! convert to linear array
        inline LightArray<T> asArray() const noexcept
        { return LightArray<T>(base,items); }

        //! load all items with args
        inline void ld(ParamType args)
        {
            for(size_t i=0;i<items;++i) base[i] = args;
        }

        inline void ldRow(const size_t r, ParamType args)
        {
            Writable<T> &arr = (*this)[r];
            for(size_t i=cols;i>0;--i) arr[i] = args;
        }

        inline void ldCol(const size_t c, ParamType args)
        {
            Matrix &self = *this;
            for(size_t r=rows;r>0;--r) self[r][c] = args;
        }



        //! swap rows
        inline void swapRows(const size_t a, const size_t b) noexcept
        {
            Matrix &self = *this;
            Memory::OutOfReach::Swap(&self[a][1], &self[b][1], code->stride );
        }

        //! swap cols
        inline void swapCols(const size_t a, const size_t b) noexcept
        {
            Matrix &self = *this;
            for(size_t i=rows;i>0;--i)
            {
                Memory::OutOfReach::Swap(&self[i][a], &self[i][b], sizeof(T) );
            }
        }

        //! swap both columns and rows
        inline void swapBoth(size_t i,size_t j) throw()
        {
            swapRows(i,j);
            swapCols(i,j);
        }


        //! assign matrices with same metrics
        template <typename U> inline
        Matrix<T> & assign(const Matrix<U> &other)
        {
            assert(hasSameMetricsThan(other));
            for(size_t i=rows;i>0;--i)
            {
                Writable<T      > &tgt = (*this)[i];
                const Readable<U> &src = other[i];
                for(size_t j=cols;j>0;--j) tgt[j] = src[j];
            }
            return *this;
        }


        //______________________________________________________________________
        //
        //! multiply all terms by rhs
        //______________________________________________________________________
        template <typename RHS>
        Matrix<T> & operator*=(RHS &rhs)
        {
            for(size_t i=0;i<items;++i) base[i] *= rhs;
            return *this;
        }

        //______________________________________________________________________
        //
        //! divide all terms by rhs
        //______________________________________________________________________
        template <typename RHS>
        Matrix<T> & operator/=(RHS &rhs)
        {
            for(size_t i=0;i<items;++i) base[i] /= rhs;
            return *this;
        }

        //______________________________________________________________________
        //
        //! build minor(I,J)
        //______________________________________________________________________
        template <typename U>
        inline void buildMinor(Matrix<U> &m, const size_t I, const size_t J) const
        {
            assert(I>=1); assert(I<=rows);
            assert(J>=1); assert(J<=cols);
            assert(m.rows==rows-1);
            assert(m.cols==cols-1);
            size_t ii=1;
            for(size_t i=1;i<=rows;++i)
            {
                if(i==I) continue;
                Writable<U>       &tgt = m[ii++];
                const Readable<T> &src = (*this)[i];
                size_t jj=1;
                for(size_t j=1;j<=cols;++j)
                {
                    if(j==J) continue;
                    tgt[jj++] = src[j];
                }
            }
            
        }

        //______________________________________________________________________
        //
        //! exchange, no throw
        //______________________________________________________________________
        inline void xch(Matrix &other) noexcept
        {
            CoerceSwap(cols,other.cols);
            CoerceSwap(rows,other.rows);
            CoerceSwap(items,other.items);
            CoerceSwap(row,other.row);
            CoerceSwap(base,other.base);
            code.xch(other.code);
        }

        //______________________________________________________________________
        //
        //! release content, exchange with empty matrix
        //______________________________________________________________________
        virtual void release() noexcept
        {
            Matrix _;
            xch(_);
        }

        //______________________________________________________________________
        //
        //! exchange with a new matrix if metrics is different
        //______________________________________________________________________
        void make(const size_t nr, const size_t nc)
        {
            if(rows!=nr||cols!=nc)
            {
                Matrix _(nr,nc);
                xch(_);
            }
        }

        //______________________________________________________________________
        //
        //! assign of copy-swap
        //______________________________________________________________________
        template <typename U> inline
        void make(const Matrix<U> &other)
        {
            if( hasSameMetricsThan(other))
                assign(other);
            else
            {
                Matrix _(CopyOf,other);
                xch(_);
            }
        }
        
        


    private:
        Y_DISABLE_ASSIGN(Matrix);
        class Code;

        RowType      *row;
        MutableType  *base;
        AutoPtr<Code> code;

        inline void julia_1x1(std::ostream &os) const
        {
            assert(1==rows); assert(1==cols); assert(0!=base);
            os << "hcat(" << base[0] << ")";
        }

        inline void julia_1xn(std::ostream &os) const
        {
            assert(1==rows); assert(1<cols);  assert(0!=row);
            row[1].Julia(os << "[") << "]";
        }

        inline void julia_nx1(std::ostream &os) const
        {
            assert(1<rows); assert(1==cols); assert(0!=row);
            os << "hcat([" << row[1][1];
            for(size_t r=2;r<=rows;++r)
                os << ' ' << row[r][1];
            os << "]')";
        }

        //______________________________________________________________________
        //
        //
        //! Code for Matrix
        //
        //______________________________________________________________________
        class Code : public Memory::Embedded
        {
        public:
            explicit Code(Memory::Embed emb[], const size_t nc) :
            Memory::Embedded(emb,NUM_FIELDS,ALLOCATOR::Instance()),
            stride(nc*sizeof(T)),
            dataOps(emb[DATA_INDEX]),
            rowInfo(emb[DATA_INDEX].address(),nc),
            rowsOps(emb[ROWS_INDEX],rowInfo)
            {
            }

            inline virtual ~Code() noexcept { }

            const size_t             stride;
        private:
            Implanted<T>             dataOps;
            Core::MatrixRow::Info    rowInfo;
            Implanted<RowType>       rowsOps;
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

        // duplicate
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

        // duplicateTranspse
        template <typename U, typename ALLOC, typename TRANSFORM>
        inline void duplicateTransposeOf(const Matrix<U,ALLOC> &M, TRANSFORM &transform)
        {
            assert(hasSameMetricsThanTransposeOf(M));
            create();
            Matrix &A = *this;
            for(size_t i=rows;i>0;--i)
            {
                for(size_t j=cols;j>0;--j)
                {
                    A[i][j] = transform(M[j][i]);
                }
            }
        }



    };

}

#endif
