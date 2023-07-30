
//! \file

#ifndef Y_Matrix_Row_Included
#define Y_Matrix_Row_Included 1

#include "y/container/light-array.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! base class for MatrixRow
        //
        //______________________________________________________________________
        class MatrixRow
        {
        public:
            static const char * const CallSign; //!< "MatrixRow"
            explicit MatrixRow() noexcept;      //!< setup
            virtual ~MatrixRow() noexcept;      //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MatrixRow);

        public:
            //__________________________________________________________________
            //
            //
            //! Info to setup multiple rows
            //
            //__________________________________________________________________
            class Info
            {
            public:
                Info(void *,const size_t nc) noexcept; //!< initialize row/cols
                ~Info() noexcept;                      //!< cleanup
                void  move(const size_t bs) noexcept;  //!< move address blockSize*cols

                void *       addr; //!< current row entry
                const size_t cols; //!< columns

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Info);
            };
        };

    }


    //__________________________________________________________________________
    //
    //
    //
    //! Matrix Row
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class MatrixRow : public Core::MatrixRow, public LightArray<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);       //!< aliases
        using LightArray<T>::room;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup as a LighArray
        inline explicit MatrixRow(MutableType *entry, const size_t count) noexcept :
        Core::MatrixRow(),
        LightArray<T>(entry,count)
        {
        }

        //! setup from Info, updated
        inline explicit MatrixRow(Core::MatrixRow::Info &info) noexcept :
        Core::MatrixRow(),
        LightArray<T>(static_cast<MutableType*>(info.addr),info.cols)
        {
            assert(0!=this->data);
            info.move( sizeof(T) );
        }

        //! cleanup
        inline virtual ~MatrixRow() noexcept {}

        //! helper for Julia
        inline std::ostream &Julia(std::ostream &os) const
        {
            assert(room>0);
            os << (*this)[1];
            for(size_t c=2;c<=room;++c) os << ' ' << (*this)[c];
            return os;
        }


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! overrides callSign
        inline virtual const char * callSign() const noexcept { return Core::MatrixRow::CallSign; }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(MatrixRow);


    };

}

#endif
