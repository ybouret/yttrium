

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
#include "y/type/copy.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{

    //! constructor helper for Matrices
#define Y_MATRIX(TOPO) Identifiable(), Collection(), MatrixMetrics TOPO, RowsType(), row(0), base(0), code(0)

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
        Matrix(const Matrix<U,ALLOC> &other, const AsCopy_ &) :
        Y_MATRIX( (other) )
        {
            duplicate(other,Identity<U>);
        }

        //! copy transform
        template <typename U,typename ALLOC, typename TRANSFORM> inline
        Matrix(const Matrix<U,ALLOC> &other, const AsCopy_ &, TRANSFORM &transform) :
        Y_MATRIX( (other) )
        {
            duplicate(other,transform);
        }

        //! copy transposed, using identity
        template <typename U,typename ALLOC> inline
        Matrix(const Matrix<U,ALLOC> &other, const TransposeOf_ &_) :
        Y_MATRIX( (other,_) )
        {
            duplicateTransposeOf(other,Identity<U>);
        }

        //! copy transposed, using transform
        template <typename U,typename ALLOC,typename TRANSFORM> inline
        Matrix(const Matrix<U,ALLOC> &other, const TransposeOf_ &_, TRANSFORM &transform) :
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

        //! swap rows
        inline void swapRows(const size_t a, const size_t b) noexcept
        {
            Matrix &self = *this;
            Memory::OutOfReach::Swap(&self[a][1], &self[b][1], code->stride );
        }

        //! assign
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

        //! multiply
        template <typename LHS, typename RHS> inline
        void operator()(LHS &lhs, RHS &rhs) const
        {
            assert(lhs.size()>=rows);
            assert(rhs.size()>=cols);
            for(size_t i=rows;i>0;--i)
            {
                Type sum(0);
                const Readable<T> &r = row[i];
                for(size_t j=cols;j>0;--j) sum += r[j] * rhs[j];
                lhs[i] = sum;
            }
        }

        //! multiply with xadd
        template <typename LHS, typename RHS> inline
        void operator()(LHS &lhs, RHS &rhs, MKL::Antelope::Add<T> &xadd) const
        {
            assert(lhs.size()>=rows);
            assert(rhs.size()>=cols);
            for(size_t i=rows;i>0;--i)
            {
                xadd.free();
                const Readable<T> &r = row[i];
                for(size_t j=cols;j>0;--j)
                {
                    ConstType p = r[j] * rhs[j];
                    xadd << p;
                }
                lhs[i] = xadd.sum();;
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
    };

}

#endif
