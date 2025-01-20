
//! \file

#ifndef Y_Apex_Block_Jig_API_Included
#define Y_Apex_Block_Jig_API_Included 1

#include "y/apex/types.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! API for Jigs
        //
        //
        //______________________________________________________________________
        class JigAPI
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned Plans = 4;          //!< number of plans
            static const unsigned Faded = Plans-1;    //!< number of faded plans
            static const Plan     Dull[Plans][Faded]; //!< list of faded plans

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit JigAPI(const size_t _count) noexcept; //!< setup with count

        public:
            virtual ~JigAPI() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(JigAPI);    //!< display

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void   setBits(const size_t bits)       noexcept = 0; //!< set words according to bits
            virtual size_t upgrade()                        noexcept = 0; //!< upgrade words from content
            virtual void   display(std::ostream &)    const          = 0; //!< display
            virtual bool   chkBits(const size_t bits) const noexcept = 0; //!< sanity check

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t words; //!< current words
            const size_t count; //!< maximum words (a power of two)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(JigAPI);
        };

    }

}

#endif

