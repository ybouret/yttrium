#include "y/ink/ops/blur.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {

            Blur:: ~Blur() noexcept {}
            Blur::  Blur() noexcept {}

            const char * const Blur:: CallSign = "Ink::Blur";

            void Blur:: ThrowInvalidSigma(const double sig)
            {
                throw Specific::Exception(CallSign, "invalid sigma=%g", sig);
            }

        }

    }

}

