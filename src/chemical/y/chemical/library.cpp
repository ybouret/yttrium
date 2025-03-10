
#include "y/chemical/library.hpp"

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


        void Library:: throwFailedInsertion() const
        {

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
