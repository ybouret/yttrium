//! \file

#ifndef Y_Apex_Ortho_Coven_Wayfarer_Included
#define Y_Apex_Ortho_Coven_Wayfarer_Included 1


#include "y/apex/api/ortho/coven/tribes.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                //______________________________________________________________
                //
                //
                //
                //! resources to explore all tribes
                //
                //
                //______________________________________________________________
                struct Wayfarer
                {
                    template <typename MATRIX> static inline
                    void Explore(XMLog        &   xml,
                                 Callback     &   proc,
                                 const MATRIX &   data,
                                 uint64_t * const ell)
                    {
                        const Metrics params(data.cols);
                        Ortho::VCache vCache( new Ortho::Vector::Cache(params) );
                        Ortho::FCache fCache( new Ortho::Family::Cache(vCache) );
                        IBank         iCache;

                        Ortho::Coven::Tribes   tribes(xml,proc,data,iCache,fCache,ell);
                        while(tribes.size)
                        {
                            tribes.generate(xml,proc,data,Strategy::Optimize,ell);
                        }
                    }
                };
            }

        }

    }

}

#endif

