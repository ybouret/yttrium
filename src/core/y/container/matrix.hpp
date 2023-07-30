

//! \file

#ifndef Y_Matrix_Included
#define Y_Matrix_Included 1

#include "y/container/matrix/metrics.hpp"
#include "y/container/matrix/row.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/embedded.hpp"
#include "y/container/implanted.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

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
        typedef Writable<RowType> RowsType; //!< aluas
        

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup empty
        inline explicit Matrix() noexcept :
        MatrixMetrics(0,0), RowsType(), row(0), base(0), code(0) {}

        //! setup with (possible) data
        inline explicit Matrix(const size_t nr, const size_t nc) :
        MatrixMetrics(nr,nc), RowsType(), row(0), base(0), code(0)
        {
            create();
        }

        //! cleanup
        inline virtual ~Matrix() noexcept {
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

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Matrix);
        class Code;

        RowType      *row;
        MutableType  *base;
        AutoPtr<Code> code;


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
    };

}

#endif
