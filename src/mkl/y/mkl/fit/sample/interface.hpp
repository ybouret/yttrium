//! \file

#ifndef Y_Fit_Sample_Interface_Included
#define Y_Fit_Sample_Interface_Included 1

#include "y/mkl/fit/sample/abscissae.hpp"
#include "y/ptr/ark.hpp"
#include "y/system/rtti.hpp"
#include "y/stream/libc/output.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            //__________________________________________________________________
            //
            //
            //
            //! Sample description
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class Sample : public Identifiable, public SampleAbscissae<ABSCISSA>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef SampleAbscissae<ABSCISSA>       MyAbscissae; //!< alias

                typedef typename MyAbscissae::Abscissa  Abscissa;    //!< alias
                typedef ORDINATE                        Ordinate;    //!< alias

                typedef typename MyAbscissae::Abscissae Abscissae;   //!< alias
                typedef Readable<ORDINATE>              Ordinates;   //!< alias

                typedef ArkPtr<String,Sample>           Pointer;     //!< alias


                //! ORDINATE Dimension, based on ABSCISSA size
                static const size_t Dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! access ordinates
                virtual const Ordinates & ordinates() const noexcept = 0;
                
                //! access predicted ordinates
                virtual const Ordinates & predicted() const noexcept = 0;


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! Ordinate to Abscissa pointer
                static inline const Abscissa * O2A(const Ordinate &o) noexcept
                {
                    return Memory::OutOfReach::Cast<const Abscissa, const Ordinate>( &o );
                }

                //! display basic info
                inline friend std::ostream & operator<<(std::ostream &os, const Sample &s)
                {
                    const String &aid = RTTI::Name<Abscissa>();
                    const String &oid = RTTI::Name<Ordinate>();
                    os << "['" << s.name << "': " << s.callSign() << "<" << aid << "," << oid << "> #" << std::setw(5) << s.numPoints() << "]";
                    return os;
                }

                //! save to output stream
                inline void save( OutputStream &fp ) const
                {
                    const size_t     n = this->numPoints();
                    const Abscissae &a = this->abscissae();
                    const Ordinates &b = this->ordinates();
                    for(size_t i=1;i<=n;++i)
                    {
                        fp("%.15g",double(a[i]));
                        const Abscissa * const p = O2A( b[i] );
                        for(size_t j=0;j<Dimension;++j)
                            fp(" %.15g",double(p[j]));
                        fp << '\n';
                    }
                }

                //! save using name.dat
                inline void saveDatFile() const
                {
                    const String     fn = this->key() + ".dat";
                    Libc::OutputFile fp(fn);
                    save(fp);
                }

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Sample() noexcept {} //!< cleanp
            protected:
                template <typename ID>
                explicit Sample(const ID &id) noexcept : MyAbscissae(id) {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sample);
            };

        }

    }
}

#endif

