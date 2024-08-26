
#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        const char * const Library:: CallSign = "Chemical::Library";
        
        Library:: ~Library() noexcept {}

        Library:: Library() : Proxy<const SpeciesSet>(), db()
        {
        }

        Library:: ConstInterface & Library:: surrogate() const noexcept
        {
            return db;
        }


        void Library:: NoSpecies(const char * const id)
        {
            assert(0!=id);
            throw Specific:: Exception(CallSign, "no species '%s'", id);
        }
        const Species & Library:: tryInsert(const Species::Ptr &sp)
        {
            const Species::Ptr *ppS = db.search(sp->name);
            if(0==ppS)
            {
                // new species
                if(!db.insert(sp))
                    throw  Specific::Exception(CallSign, "unexpected insert failure for '%s'", sp->name.c_str());
                enroll(*sp);
                return *sp;
            }
            else
            {
                // multiple species ?
                const Species &mine = **ppS;
                if(mine.z !=sp->z) throw Specific::Exception(CallSign, "charge mismatch for '%s'",sp->name.c_str());
                return mine;
            }
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            os << '{' << std::endl;
            size_t                 n = lib.db.size();
            Library::ConstIterator i = lib->begin();
            while(n-- > 0)
            {
                lib.lj(os,(**i).name) << " : " << (**i).z << std::endl;
                ++i;
            }
            os << '}';
            return os;
        }


        const Species * Library:: query(const String &id) const noexcept {
            const Species::Ptr *ppS = db.search(id);
            if(!ppS) return 0;
            return & **ppS;
        }

        const Species * Library:: query(const char * const id ) const {
            const String _(id); return query(_);
        }

        const Species & Library:: operator[](const String &     id) const
        {
            const Species * const p = query(id);
            if(!p) NoSpecies(id.c_str());
            return *p;
        }

        const Species & Library:: operator[](const char * const id) const
        {
            const String _(id);
            const Species * const p = query(_);
            if(!p) NoSpecies(_.c_str());
            return *p;
        }

    }
}

