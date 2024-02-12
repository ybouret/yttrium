
//! \file

#ifndef Y_Fit_Samples_Included
#define Y_Fit_Samples_Included 1

#include "y/mkl/fit/sample/interface.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/exception.hpp"

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
            //! Samples are a database of shared Sample::Pointer
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class Samples : public SuffixSet<String, typename Sample<ABSCISSA,ORDINATE>::Pointer>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Sample<ABSCISSA,ORDINATE>       SampleType;     //!< alias
                typedef typename SampleType::Pointer    SamplePointer;  //!< alias
                typedef SuffixSet<String,SamplePointer> SampleDB;       //!< alias
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
               
                //! setup
                explicit Samples() : SampleDB() {}

                //! cleanup
                virtual ~Samples() noexcept {}
                

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
              
                //! append [Light|Heavy|..]Sample
                template <typename SAMPLE>
                SAMPLE & operator()( SAMPLE *sample )
                {
                    assert(0!=sample);
                    const SamplePointer s(sample);
                    if(!this->insert(s))
                        throw Exception("Multiple Fit::Sample '%s'", sample->name.c_str());
                    return *sample;
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
            };
        }

    }

}

#endif

