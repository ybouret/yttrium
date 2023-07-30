
//! \file

#ifndef Y_Matrix_Row_Included
#define Y_Matrix_Row_Included 1

#include "y/container/writable.hpp"
#include "y/container/iterator/writable-contiguous.hpp"

namespace Yttrium
{

    namespace Core
    {
        class MatrixRow
        {
        public:
            static const char * const CallSign;
            virtual ~MatrixRow() noexcept;

        protected:
            explicit MatrixRow(const size_t nc) noexcept; //!< nc>0
            const size_t cols;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(MatrixRow);

        public:
            class Info
            {
            public:
                Info(void *,const size_t nc) noexcept;
                ~Info() noexcept;

                void  move(const size_t blockSize) noexcept;

                void *       addr;
                const size_t cols;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Info);
            };
        };


    }


    template <typename T>
    class MatrixRow : public Core::MatrixRow, Writable<T>, public WritableContiguous<T>
    {
    public:
        Y_ARGS_DECL(T,Type);

        inline explicit MatrixRow(MutableType *entry, const size_t count) noexcept :
        Core::MatrixRow(count),
        item(entry-1),
        data(entry)
        {
        }

        inline explicit MatrixRow(Core::MatrixRow::Info &info) noexcept :
        Core::MatrixRow(info.cols),
        item(static_cast<MutableType *>(info.addr)-1),
        data(item+1)
        {
            assert(0!=data);
            info.move( sizeof(T) );
        }


        inline virtual ~MatrixRow() noexcept
        { Coerce(item) = 0; Coerce(data) = 0; }

        inline virtual const char * callSign() const noexcept { return CallSign; }
        inline virtual size_t       size()     const noexcept { return cols; }

        inline virtual Type & operator[](const size_t c) noexcept
        { assert(c>=1); assert(c<=cols); return item[c]; }

        inline virtual ConstType & operator[](const size_t c) const noexcept
        { assert(c>=1); assert(c<=cols); return item[c]; }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(MatrixRow);
        MutableType * const item;
        ConstType   * const data;

        virtual ConstType * getBaseForward() const noexcept { return data;      }
        virtual ConstType * getLastForward() const noexcept { return data+cols; }
        virtual ConstType * getBaseReverse() const noexcept { return item+cols; }
        virtual ConstType * getLastReverse() const noexcept { return item;      }


    };

}

#endif
