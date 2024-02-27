//! \file

#ifndef Y_MKL_Strain_Filter_Included
#define Y_MKL_Strain_Filter_Included 1

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
            class FilterInfo
            {
            public:
                static const char * const CallSign; //!< Filter
                static const size_t       SIZE = 3; //!< alias

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
                virtual const char * callSign()                 const noexcept; //!< [Identifiable] CallSign
                virtual size_t       size()                     const noexcept; //!< [Collection]    SIZE
                virtual const T & operator[](const size_t indx) const noexcept; //!< [Readable] [1]: position, [2]: frist drvs, [3]: second drvs

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void   free() noexcept;                         //!< cleanup data
                void   rem()  noexcept;                         //!< remove last data point
                void   add(const T x, const T y);               //!< add (x,y)
                void   eval(const T x0, const size_t degree);   //!< extract at value x0, with max degree
                size_t load() const noexcept;                   //!< number of points
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

