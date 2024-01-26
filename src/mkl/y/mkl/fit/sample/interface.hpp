//! \file

#ifndef Y_Fit_Sample_Interface_Included
#define Y_Fit_Sample_Interface_Included 1

#include "y/mkl/fit/sample/abscissae.hpp"
#include "y/ptr/ark.hpp"

#include "y/system/rtti.hpp"
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


                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! access ordinates
                virtual const Ordinates & ordinates() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
             
                //! display basic info
                inline friend std::ostream & operator<<(std::ostream &os, const Sample &s)
                {
                    const String &aid = RTTI::Name<Abscissa>();
                    const String &oid = RTTI::Name<Ordinate>();
                    os << "['" << s.name << "': " << s.callSign() << "<" << aid << "," << oid << "> #" << std::setw(5) << s.dimension() << "]";
                    return os;
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

