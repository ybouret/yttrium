
#include "y/chemical/library.hpp"
#include "y/system/exception.hpp"

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


        const Species & Library:: setup(const Species::Pointer &sp)
        {
            const String &                 id = sp.key();
            {
                const Species::Pointer * const pp = db.search(id);
                if(0!=pp) return **pp;
            }
            
            if(latched)
                throw Specific::Exception(CallSign,"latched while creating '%s'", id.c_str());

            if(!db.insert(sp))
                throw Specific::Exception(CallSign, "failed to insert new '%s'", id.c_str());


            enroll(*sp);
            return *sp;
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
            while(n-- > 0)
            {
                const Species::Pointer sp = Species::ReadFrom(fp);
                (void) setup(sp);
            }
        }

    }

}

