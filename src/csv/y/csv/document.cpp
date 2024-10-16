#include "y/csv/document.hpp"

namespace Yttrium
{

    namespace CSV
    {
        Document::  Document() noexcept : Object(), Counted(), Lines() {}


        Document::  Document(const size_t capa)   : Object(), Counted(), Lines(capa,AsCapacity) {}


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


        Line & Document:: add()
        {
            {
                const Line _;
                pushTail(_);
            }
            return tail();
        }

    }

}


