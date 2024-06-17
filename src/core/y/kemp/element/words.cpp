#include "y/kemp/element/kar.hpp"
#include "y/kemp/element/add.hpp"
#include "y/kemp/element/sub.hpp"
#include "y/system/error.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Element:: Words::  Words() noexcept : lower(0), upper(0) {}
        Element:: Words:: ~Words() noexcept {}

        size_t Element:: Split(Element   &X,
                               Words     &XP,
                               Element   &Y,
                               Words     &YP,
                               const Ops &ops)
        {
            switch(ops)
            {
                case Ops64_8:
                case Ops32_8:
                case Ops16_8:
                    return WordOps:: SplitWith(X.get<uint8_t>(),XP,Y.get<uint8_t>(),YP);

                case Ops64_16:
                case Ops32_16:
                    return WordOps:: SplitWith(X.get<uint16_t>(),XP,Y.get<uint16_t>(),YP);

                case Ops64_32:
                    return WordOps:: SplitWith(X.get<uint32_t>(),XP,Y.get<uint32_t>(),YP);

            }
            Libc::CriticalError(EINVAL, "corrupted ops type in %s::Split", CallSign);
            return 0;
        }


       


        Element * Element:: Merge(Element       &lower,
                                  Element       &upper,
                                  const size_t   m,
                                  const Ops      ops)
        {
            switch(ops)
            {
                case Ops64_32: return Karatsuba<uint64_t,uint32_t>::Merge(lower,upper,m);
                case Ops64_16: return Karatsuba<uint64_t,uint16_t>::Merge(lower,upper,m);
                case Ops64_8:  return Karatsuba<uint64_t,uint8_t>::Merge(lower,upper,m);

                case Ops32_16: return Karatsuba<uint32_t,uint16_t>::Merge(lower,upper,m);
                case Ops32_8:  return Karatsuba<uint32_t,uint8_t>::Merge(lower,upper,m);
                case Ops16_8:  return Karatsuba<uint16_t,uint8_t>::Merge(lower,upper,m);

            }
            Libc::CriticalError(EINVAL, "corrupted ops type in %s::Merge", CallSign);
            return 0;
        }

    }

}

#if 0
namespace Yttrium
{

    namespace Kemp
    {

        Element *KaratsubaMul(Element &lhs, Element &rhs, const Ops ops)
        {
            switch(ops)
            {
                case Ops64_32: return Karatsuba<uint64_t,uint32_t>::Get(lhs.get<uint32_t>(),rhs.get<uint32_t>());
                case Ops64_16: return Karatsuba<uint64_t,uint16_t>::Get(lhs.get<uint16_t>(),rhs.get<uint16_t>());
                case Ops64_8:  return Karatsuba<uint64_t,uint8_t>::Get(lhs.get<uint8_t>(),rhs.get<uint8_t>());

                case Ops32_16: return Karatsuba<uint32_t,uint16_t>::Get(lhs.get<uint16_t>(),rhs.get<uint16_t>());
                case Ops32_8:  return Karatsuba<uint32_t,uint8_t>::Get(lhs.get<uint8_t>(),rhs.get<uint8_t>());

                case Ops16_8:  return Karatsuba<uint16_t,uint8_t>::Get(lhs.get<uint8_t>(),rhs.get<uint8_t>());

            }
            Libc::CriticalError(EINVAL, "corrupted ops type in %s::KMul", Element::CallSign);
            return 0;
        }



    }

}
#endif

