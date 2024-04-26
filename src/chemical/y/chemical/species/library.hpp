//! \file

#ifndef Y_Chemical_Species_Library_Included
#define Y_Chemical_Species_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/type/proxy.hpp"
#include "y/chemical/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Libary of (topLevel) species
        //
        //
        //______________________________________________________________________
        class Library : public Proxy<const Species::Set>, public Entities, public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definions
            //
            //__________________________________________________________________
            static const char * const           CallSign;      //!< "Chemical::Library"
            typedef Species::Set::ConstIterator ConstIterator; //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();          //!< setup empty
            virtual ~Library() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Library);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< CallSign


            //__________________________________________________________________
            //
            //!  return new/existing species, checking matching chargess
            /**
             \param name name of the species
             \param z    charge of the species
             */
            //__________________________________________________________________
            template <typename NAME> inline
            const Species & operator()(const NAME &name,
                                       const int   z)
            {
                const size_t          sindex = sdb.size()+1;
                const Species::Handle handle = new Species(name,z,sindex);
                return manage(handle);
            }

            const Species & operator[](const String &id) const; //!< get species by name
            const Species & operator[](const char   *id) const; //!< get species by name

            //! display the full TopLevel array per species
            template <typename ARRAY> inline
            std::ostream & operator()(std::ostream &os, ARRAY &array) const
            {
                os << '{' << std::endl;
                for(ConstIterator it=sdb.begin();it!=sdb.end();++it)
                {
                    const Species &sp = **it;
                    pad(os << "    " << '[' << sp.name << ']',sp) << " = " << real_t(array[ sp.indx[TopLevel] ]) << std::endl;
                }
                return os << '}';
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            Species::Set sdb;
            virtual ConstInterface & surrogate() const noexcept;
            const Species &          manage(const Species::Handle &);

        };

    }

}

#endif

