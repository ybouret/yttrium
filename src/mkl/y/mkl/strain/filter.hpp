//! \file

#ifndef Y_MKL_Strain_Filter_Included
#define Y_MKL_Strain_Filter_Included 1

#include "y/mkl/strain/interface.hpp"
#include "y/container/readable.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            //__________________________________________________________________
            //
            //
            //
            //! Info for Filter
            //
            //
            //__________________________________________________________________
            class FilterInfo : public Interface
            {
            public:
                static const char * const CallSign; //!< "Strain::Filter"

                virtual ~FilterInfo() noexcept;     //!< cleanup
            protected:
                explicit FilterInfo() noexcept;     //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(FilterInfo);
            };


            //__________________________________________________________________
            //
            //
            //
            //! Adaptive Savitsky-Golay filter
            //
            //
            //__________________________________________________________________
            template <typename T>
            class Filter : public FilterInfo, public Readable<T>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Filter();          //!< setup
                virtual ~Filter() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual const char * callSign()               const noexcept; //!< [Identifiable] CallSign
                virtual size_t       size()                   const noexcept; //!< [Collection]    SIZE
                virtual const T &    operator[](const size_t) const noexcept; //!< [Readable] [1]: position, [2]: frist drvs, [3]: second drvs
                virtual void         free()                         noexcept; //!< [Recyclable] cleanup data
                virtual size_t       load()                   const noexcept; //!< [Interface] number of points

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void   rem()  noexcept;                         //!< remove last data point
                void   add(const T x, const T y);               //!< add (x,y)
                const Readable<T> & eval(const T x0, const size_t degree);   //!< extract at value x0, with max degree
                void   save(const Core::String<char> &) const;  //!< save selection

            private:
                class Code;
                Code *code;
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };
        }
    }
}

#endif

