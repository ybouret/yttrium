#include "y/csv/document.hpp"

namespace Yttrium
{

    namespace CSV
    {
        Document::  Document() noexcept : Object(), Counted(), Lines() {}

        Document:: ~Document() noexcept {}

        Document::  Document(const Document &doc) :
        Identifiable(),
        Collection(),
        Object(),
        Counted(),
        Lines(doc)
        {}
        
        const char * Document:: callSign() const noexcept
        {
            return "CSV::Document";
        }

    }

}


