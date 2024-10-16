
#include "y/csv/line.hpp"

namespace Yttrium
{

    namespace CSV
    {

        Line:: ~Line() noexcept {}

        Line::  Line() noexcept : Fields() {}

        Line::  Line(const size_t capa) : Fields(capa,AsCapacity) {}

        Line::  Line(const Line &other) :
        Identifiable(), Collection(), Fields(other) {}

        const char * Line:: callSign() const noexcept
        {
            return "CSV::Line";
        }
    }

}

