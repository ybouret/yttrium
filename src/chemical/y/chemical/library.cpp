
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
