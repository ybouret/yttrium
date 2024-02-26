//! \file

#ifndef Y_MKL_Strain_Filter_Included
#define Y_MKL_Strain_Filter_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            class FilterInfo
            {
            public:
                static const char * const CallSign;
                static const size_t       SIZE = 3;
                virtual ~FilterInfo() noexcept;
            protected:
                explicit FilterInfo() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(FilterInfo);
            };




            template <typename T>
            class Filter : public FilterInfo, public Readable<T>
            {
            public:
                explicit Filter();
                virtual ~Filter() noexcept;


                virtual const char * callSign() const noexcept;
                virtual size_t       size()     const noexcept;
                virtual const T & operator[](const size_t indx) const noexcept;

                void free();
                void eval(const T x0, const size_t degree);



            private:
                class Code;
                Code *code;
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };
        }
    }
}

#endif

