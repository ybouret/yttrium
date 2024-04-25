
//! \file

#ifndef Y_WOVEn_Survey_Included
#define Y_WOVEn_Survey_Included 1

#include "y/woven/types.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for a Survey
        //
        //
        //______________________________________________________________________
        class Survey
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual size_t dimensions() const noexcept = 0; //!< get dimension from first vector
            virtual void   verify(const QVector &)     = 0; //!< verify and insert vector
            virtual size_t maxOrder() const noexcept   = 0; //!< get maximum order

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Survey() noexcept;        //!< cleanup
        protected:
            explicit Survey(XMLog &) noexcept; //!< setup

        public:
            XMLog &xml; //!< output helper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Survey);
        };
    }

}

#endif
