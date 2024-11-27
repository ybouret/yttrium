//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

    
        //______________________________________________________________________
        //
        //
        //
        //! database of species for Library
        //
        //
        //______________________________________________________________________
        class Ingredients : public Species::Set, public Assembly
        {
        public:
            explicit Ingredients();                   //!< setup empty
            virtual ~Ingredients() noexcept;          //!< cleanup
            void mustInsert(const Species::Handle &); //!< insert new species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ingredients);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Database of Species
        //
        //
        //______________________________________________________________________
        class Library : public Proxy<const Ingredients>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Ingredients::ConstIterator ConstIterator; //!< alias
            static const char * const          CallSign;      //!< "Chemical::Library"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();           //!< setup empty
            virtual ~Library() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Library);     //!< display info

            //__________________________________________________________________
            //
            //
            // Methdods
            //
            //__________________________________________________________________

            //! get/create species from its formula
            const Species & get(const Formula &);

            //! get/create species from string,text, formula...
            template <typename DATA> inline
            const Species & operator()(const DATA &data)
            {
                const Formula formula(data);
                return get(formula);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
            Ingredients db;
        };
    }

}

#endif

