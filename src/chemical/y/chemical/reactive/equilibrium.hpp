
//! \file


#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1


#include "y/chemical/reactive/components.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        


        //______________________________________________________________________
        //
        //
        //
        //! Interface for equilibrium
        //
        //
        //______________________________________________________________________
        class Equilibrium : public Oversized, public Components, public Counted
        {
        public:
            static const char         Prefix    = '@'; //!< for parser
            static const char * const Reac;            //!< "Reac";
            static const char * const Prod;            //!< "Prod";
            static const char * const Label;           //!< "Label"
            static const char * const CallSign;  //!< "Equilibrium"

            typedef ArkPtr<const String,Equilibrium> Pointer;

            struct Parser
            {
                static const String * Name(const XCode &);
            };


        protected:
            explicit Equilibrium(const XCode &xcode) :
            Components( Parser::Name(xcode) )
            {
            }

        public:
            virtual ~Equilibrium() noexcept;

            // Methods
            xreal_t K(xreal_t);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal_t getK(xreal_t) = 0;
        };


        class ConstEquilibrium : public Equilibrium
        {
        public:

            template <typename NAME>
            explicit ConstEquilibrium(const NAME &eid, const xreal_t value) :
            Equilibrium(eid),
            Konst(value)
            {
            }

            virtual ~ConstEquilibrium() noexcept;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            virtual xreal_t getK(xreal_t);
            const xreal_t Konst;
        };

    }


}


#endif
