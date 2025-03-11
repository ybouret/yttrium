//! \file


#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1


#include "y/chemical/type/xcode.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Species Formula, keep parsed Xnode
        //
        //
        //______________________________________________________________________
        class Formula : public Proxy<const XNode>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Formula"
            static const char * const Name;     //!< "Name"
            static const char * const Body;     //!< "Body"
            static const char * const Mult;     //!< "Mult"
            static const char * const Z;        //!< "Z"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Formula(const XNode * const) noexcept; //!< setup from raw node
            Formula(const XCode   &)     noexcept; //!< setup for shared code
            Formula(const Formula &)     noexcept; //!< shared copy
            Formula(const char * const description);
            Formula(const String &     description);
            virtual ~Formula()           noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String * toText(int &z) const;
            
        private:
            Y_DISABLE_ASSIGN(Formula);
            Y_PROXY_DECL();
            const XCode code;
        };
    }

}


#endif

