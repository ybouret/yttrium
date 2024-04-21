//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/reactive/component.hpp"
#include "y/type/proxy.hpp"
#include "y/associative/address-book.hpp"

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
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
         
            //! append a new component
            void operator()(const int nu, const Species &sp);

            //! sum nu * z
            int  chargeBalance()                            const noexcept;
            bool contains(const Species &sp)                const noexcept; //!< look for species
            bool sharesSpeciesWith(const Components &other) const noexcept; //!< look for shared species
            void recordSpeciesInto(AddressBook &)           const;          //!< store all species address

            //! fill an array of coefficients
            template <typename T> inline
            void fill(Writable<T> &nu, const Level level) const
            {
                for(Component::Set::ConstIterator it=cdb.begin();it!=cdb.end();++it)
                {
                    const Component &component = *it;
                    nu[ component.sp.indx[level] ] = component.nu;
                }
            }


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
            //__________________________________________________________________
            //
            //
            //! Formatting helper
            //
            //__________________________________________________________________
            class Formatting
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Formatting() noexcept; //!< setup
                virtual ~Formatting() noexcept; //!< cleanup

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________
                void          modernizeWith(const Components &) noexcept;      //!< update
                std::ostream &print(std::ostream &, const Components &) const; //!< print with current values

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const size_t rmax; //!< max length of rstr
                const size_t pmax; //!< max length of pstr

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Formatting);
            };
        };

    }

}

#endif
