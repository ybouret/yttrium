
//! \file


#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1


#include "y/chemical/reactive/components.hpp"
#include "y/chemical/library.hpp"
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char                        Prefix = '@'; //!< for parser
            static const char * const                Reac;         //!< "Reac";
            static const char * const                Prod;         //!< "Prod";
            static const char * const                Label;        //!< "Label"
            static const char * const                CallSign;     //!< "Equilibrium"
            typedef ArkPtr<const String,Equilibrium> Pointer;      //!< alias

            //! compiling from parsed XNode
            struct Compile
            {
                static const String * Name(const XNode * const);                    //!< extract name
                static void           Fill(Components &, Library &, XNode * const); //!< extract components and latch
                static const String * Data(const XNode * const);                    //!< extract string
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Equilibrium(Library &, XNode * const, const size_t ); //!< compile
            explicit Equilibrium(const String * const, const size_t);      //!< start manual
        public:
            virtual ~Equilibrium() noexcept;                               //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            xreal_t K(xreal_t); //!< checked getK


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal_t getK(xreal_t) = 0;
        };


        typedef Small::BareLightList<Equilibrium> EList;
        typedef EList::NodeType                   ENode;

      
    }


}


#endif
