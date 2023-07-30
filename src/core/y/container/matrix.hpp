

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

    template <class T, typename ALLOCATOR = Memory::Dyadic>
    class Matrix : public MatrixMetrics
    {
    public:
        Y_ARGS_DECL(T,Type);
        typedef MatrixRow<T>     RowType;




        inline explicit Matrix() noexcept : MatrixMetrics(0,0), code(0) {}

        inline explicit Matrix(const size_t nr, const size_t nc) :
        MatrixMetrics(nr,nc),
        code(0)
        {
            create();
        }

        inline virtual ~Matrix() noexcept {
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
            Memory::Embedded(emb,2,ALLOCATOR::Instance()),
            dataOps(emb[0]),
            rowInfo(emb[0].address(),nc) //,rowsOps(emb[1],rowInfo)
            {
            }

            inline virtual ~Code() noexcept {}

            Implanted<T>             dataOps;
            Core::MatrixRow::Info    rowInfo;
            //Implanted<RowType>       rowsOps;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        inline void create()
        {
            if(items<=0) return;
            Memory::Embed emb[] = {
                Memory::Embed(row,rows),
                Memory::Embed(base,items)
            };
            code = new Code(emb,cols);
        }
    };

}

#endif
