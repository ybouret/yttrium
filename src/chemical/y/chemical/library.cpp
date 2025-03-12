
#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Library::CallSign = "Library";

        Library:: Library() :
        Proxy<const LibraryType>(),
        db()
        {
        }


        Library:: ~Library() noexcept
        {
        }

        size_t Library:: nextIndex() const noexcept
        {
            return db.size()+1;
        }



        const Species & Library:: setup(const Species::Pointer &sp)
        {
            const String &                 id = sp.key();
            {
                const Species::Pointer * const pp = db.search(id);
                if(0!=pp) return **pp;
            }
            
            if(latched)
                throw Specific::Exception(CallSign,"latched while creating '%s'", id.c_str());

            if( nextIndex() != sp->indx[TopLevel] )
                throw Specific::Exception(CallSign,"mismatch index while creating '%s'", id.c_str());


            if(!db.insert(sp))
                throw Specific::Exception(CallSign, "failed to insert new '%s'", id.c_str());


            enroll(*sp);
            return *sp;
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            os << '{';
            if(lib->size()>0)
            {
                os << std::endl;
                for(LibraryType::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    os << '\t' << sp;
                    lib.pad(os,sp);
                    os << " | z = " << std::setw(3) << sp.z;
                    os << std::endl;
                }
            }
            os << '}';
            return os;
        }


        Y_PROXY_IMPL(Library,db)


    }

}

#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        size_t Library:: serialize(OutputStream &fp) const
        {

            size_t n       = db.size();
            size_t written = fp.emitVBR(n);
            {
                LibraryType::ConstIterator it=db.begin();
                while(n-- > 0)
                {
                    written += (**it).serialize(fp);
                    ++it;
                }
            }
            return written;
        }
    }

}
#include "y/stream/input.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Library:: readFrom(InputStream &fp)
        {
            size_t n = fp.readVBR<size_t>("species");
            size_t i = db.size();
            while(n-- > 0)
            {
                const Species::Pointer sp = Species::ReadFrom(fp,++i);
                (void) setup(sp);
            }
        }

    }

}

