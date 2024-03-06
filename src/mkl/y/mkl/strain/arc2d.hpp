//! \file

#ifndef Y_MKL_Strain_Arc_Filter_Included
#define Y_MKL_Strain_Arc_Filter_Included 1

#include "y/mkl/strain/interface.hpp"
#include "y/container/readable.hpp"
#include "y/mkl/v2d.hpp"

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
            //! Info for Arc2D
            //
            //
            //__________________________________________________________________
            class Arc2DInfo : public Interface
            {
            public:
                static const char * const CallSign; //!< "Strain::Arc2D"
                
                virtual ~Arc2DInfo() noexcept; //!< cleanup
            protected:
                explicit Arc2DInfo() noexcept; //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc2DInfo);
            };

            //__________________________________________________________________
            //
            //
            //
            //! Arc2D filter
            //
            //
            //__________________________________________________________________
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
                typedef V2D<T>           Vertex;        //!< alias
                typedef Readable<Vertex> ReadableType;  //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Arc2D();          //!< setup
                virtual ~Arc2D() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual const char *   callSign()                    const noexcept; //!< [Identifiable] CallSign
                virtual size_t         size()                        const noexcept; //!< [Collection]   SIZE
                virtual const Vertex & operator[](const size_t indx) const noexcept; //!< [Readable]     [1]: position, [2]: frist drvs, [3]: second drvs
                virtual void           free()                              noexcept; //!< [Recyclable]   cleanup data
                virtual size_t         load()                        const noexcept; //!< [Interface]    number of points

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                template <typename U> inline
                void                 add(const T t, const V2D<U> &v) { add(t,v.x,v.y); }   //!< add any vertex
                void                 add(const T t, const T x, const T y);                 //!< add point
                const ReadableType & eval(const T t0, const size_t xdg, const size_t ydg); //!< extract at value t0, with max degrees for x and y

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc2D);
                class Code;
                Code *code;

            };

        }
    }

}

#endif
