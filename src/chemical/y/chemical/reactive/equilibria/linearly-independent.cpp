#include "y/chemical/reactive/equilibria/linearly-independent.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const LinearlyIndependent:: CallSign = "Chemical::LinearlyIndependent";

        LinearlyIndependent:: LinearlyIndependent(const size_t primary,
                                                  const size_t species) :
        bank(primary),
        list(bank),
        qfam(species,primary)
        {
        }

        LinearlyIndependent:: ~LinearlyIndependent() noexcept
        {
        }


        void LinearlyIndependent:: init() noexcept
        {
            list.free();
        }

        const size_t & LinearlyIndependent:: key() const noexcept
        {
            return qfam.dimensions;
        }


        LinearlyIndependent::ConstInterface & LinearlyIndependent:: surrogate() const noexcept
        {
            return list;
        }


        


    }

}
