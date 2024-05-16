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
            typedef Component::Set::ConstIterator ConstIterator; //!< alias
            
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
         

            void operator()(const int nu, const Species &sp);               //!< append a new component
            void operator()(const Component &);                             //!< append a new component
            int  chargeBalance()                            const noexcept; //!< sum nu * z
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


            bool isTheSameThan(const Components &) const noexcept; //!< same reac and prod
            bool isFlippedWith(const Components &) const noexcept; //!< mirror reac and prod
            bool isAnalogousTo(const Components &) const noexcept; //!< isTheSameThan or isFlippedWith
            bool isConcernedBy(const AddressBook&) const noexcept; //!< test if one of species in database matches component

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
