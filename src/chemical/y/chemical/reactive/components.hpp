//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/reactive/component.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Reactants and Products
        //
        //
        //______________________________________________________________________
        class Components : public Proxy<const Component::Set>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char LeftRightArrow[]; //!< "<=>"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Components();           //!< setup empty
            Components(const Components &);  //!< copy
            virtual ~Components() noexcept;  //!< cleanup
            friend std::ostream & operator<<(std::ostream &, const Components &); //!< default display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
         
            //! append a new component
            void operator()(const int nu, const Species &sp);

            
            //! display with padding
            std::ostream & showComponents(std::ostream &os, const size_t rmax=0, const size_t pmax=0) const;
          
            //! sum nu * z
            int charge() const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors reac;   //!< reactants list
            const Actors prod;   //!< products  list
            const String rstr;   //!< reactants string
            const String pstr;   //!< products  string

        private:
            Y_DISABLE_ASSIGN(Components);
            Component::Set cdb;
            virtual ConstInterface & surrogate() const noexcept;

        public:
            class Formatting
            {
            public:
                explicit Formatting() noexcept;
                virtual ~Formatting() noexcept;
                void     modernizeWith(const Components &) noexcept;




                const size_t rmax;
                const size_t pmax;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Formatting);
            };
        };

    }

}

#endif
