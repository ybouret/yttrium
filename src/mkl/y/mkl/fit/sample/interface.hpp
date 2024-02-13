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
                typedef Writable<ORDINATE>              Predicted;   //!< alias

                typedef ArkPtr<String,Sample>           Pointer;     //!< alias


                //! ORDINATE Dimension, based on ABSCISSA size
                static const size_t Dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual const Ordinates & ordinates() const noexcept = 0;  //!< access ordinates
                virtual const Predicted & fetchPred() const noexcept = 0;  //!< access predicted
                
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                inline Predicted        & predicted()        noexcept { return Coerce(fetchPred()); }
                inline const Ordinates  & predicted() const  noexcept { return fetchPred(); }

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
                    const Ordinates &c = this->predicted();

                    for(size_t i=1;i<=n;++i)
                    {
                        fp("%.15g",double(a[i]));
                        {
                            const Abscissa * const p = O2A( b[i] );
                            for(size_t j=0;j<Dimension;++j)
                                fp(" %.15g",double(p[j]));
                        }
                        {
                            const Abscissa * const p = O2A( c[i] );
                            for(size_t j=0;j<Dimension;++j)
                                fp(" %.15g",double(p[j]));
                        }

                        fp << '\n';
                    }
                }

                //! save using name.dat
                inline void saveDatFile(const char *sfx=0) const
                {
                    const String     fn = this->key() + sfx + ".dat";
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

