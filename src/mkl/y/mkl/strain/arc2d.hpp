//! \file

#ifndef Y_MKL_Strain_Arc_Filter_Included
#define Y_MKL_Strain_Arc_Filter_Included 1

#include "y/container/readable.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            class Arc2DInfo
            {
            public:
                static const char * const CallSign;
                static const size_t       SIZE = 3;
                
                virtual ~Arc2DInfo() noexcept;
            protected:
                explicit Arc2DInfo() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc2DInfo);
            };

            template <typename T>
            class Arc2D : public Arc2DInfo, public Readable< V2D<T> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef V2D<T>           Vertex;
                typedef Readable<Vertex> ReadableType;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Arc2D();
                virtual ~Arc2D() noexcept;

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual const char *   callSign()                    const noexcept; //!< [Identifiable]  CallSign
                virtual size_t         size()                        const noexcept; //!< [Collection]    SIZE
                virtual const Vertex & operator[](const size_t indx) const noexcept; //!< [Readable] [1]: position, [2]: frist drvs, [3]: second drvs

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void free() noexcept;
                void add(const T t, const T x, const T y);
                void add(const T t, const Vertex v);
                void eval(const T t0, const size_t xdg, const size_t ydg);   //!< extract at value t0, with max degree for x and y
                
            private:
                class Code;
                Code *code;
                Y_DISABLE_COPY_AND_ASSIGN(Arc2D);
            };

        }
    }

}

#endif
