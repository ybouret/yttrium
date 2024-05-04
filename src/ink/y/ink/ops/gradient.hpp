//! \file

#ifndef Y_Ink_Gradient_Included
#define Y_Ink_Gradient_Included 1

#include "y/ink/ops/filter.hpp"

namespace Yttrium
{
    
    namespace Ink
    {

        namespace Crux
        {
            //__________________________________________________________________
            //
            //
            //
            //! Base class for Gradient
            //
            //
            //__________________________________________________________________
            class Gradient : public Object, public Counted
            {
            public:
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const String &key() const noexcept; //!< name

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Gradient() noexcept; //!< cleanuo
            protected:
                //! setup
                template <typename NAME> inline
                explicit Gradient(const NAME &id) : name(id) {}

            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const String name; //!< identifier

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Gradient interface
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Gradient : public Crux::Gradient
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Filter<T>                     FilterType; //!< alias
            typedef ArkPtr<String,const Gradient> Handle;     //!< alias

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const FilterType & dx() const noexcept = 0; //!< to compute dx
            virtual const FilterType & dy() const noexcept = 0; //!< to compute dy

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Gradient() noexcept {} //!< cleanup
       
        protected:
            //! setup
            template <typename NAME>
            inline explicit Gradient(const NAME &id) :
            Crux::Gradient(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradient);
        };
    }

}

#endif

