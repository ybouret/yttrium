//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/assembly.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef SuffixSet<String,Species::Ptr> SpeciesSet; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Library: database of species
        //
        //
        //______________________________________________________________________

        class Library : public Proxy<const SpeciesSet>, public Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign;              //!< "Chemical::Library"
            typedef SpeciesSet::ConstIterator ConstIterator; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            explicit Library();           //!< setup
            virtual ~Library() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Library);     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! record/recall species
            template <typename ID>
            const Species & operator()(const ID &name,
                                       const int z) {
                Species::Ptr sp = new Species(name,z,db.size()+1);
                return tryInsert(sp);
            }

            const Species *query(const String &     id) const noexcept; //!< query species by name
            const Species *query(const char * const id) const;          //!< query species by name

            

            //! display
            template <typename ARRAY> inline
            void  operator()(std::ostream &os,
            const char  *pfx,
            const ARRAY &arr,
            const char  *sfx) const
            {
                if(!pfx) pfx="";
                if(!sfx) sfx="";
                os << '{' << std::endl;
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    const Species &sp = **it;
                    const String  &id = (**it).name;
                    pad(os << pfx << id << sfx,sp) << " = " << double(arr[sp.indx[TopLevel]]) << std::endl;
                }
                os << '}' << std::endl;
            }
            
            //! transfer components-wise
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, const Level tgtLevel,
                          SOURCE &source, const Level srcLevel) const
            {
                size_t n=db.size();
                for(ConstIterator it=db.begin();n>0;--n,++it)
                {
                    const Species &sp = **it;
                    target[sp.indx[tgtLevel]] = source[sp.indx[srcLevel]];
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            SpeciesSet db;

            virtual ConstInterface & surrogate() const noexcept;
            const Species &          tryInsert(const Species::Ptr &sp);
        };
    }

}

#endif

