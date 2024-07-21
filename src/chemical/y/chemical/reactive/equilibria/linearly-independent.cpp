#include "y/chemical/reactive/equilibria/linearly-independent.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
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

        void LinearlyIndependent:: ensure(const size_t primary)
        {
            const size_t capa = qfam.size + qfam.reservoir.size;
            if(capa<primary) { qfam.reserve(primary-capa); }
        }


        void LinearlyIndependent:: init() noexcept
        {
            list.free();
            qfam.free();
        }

#if 0
        bool LinearlyIndependent:: keep(const Equilibrium &eq, const Matrix<int> &topo)
        {
            if(qfam.wouldAccept(topo[eq.indx[SubLevel]]))
            {
                qfam.expand();
                list << eq;
                return true;
            }
            else
                return false;
        }
#endif

        bool  LinearlyIndependent:: keep(Prospect &pro, const Matrix<int> &topo)
        {
            if(qfam.wouldAccept(topo[pro.eq.indx[SubLevel]]))
            {
                qfam.expand();
                list << pro;
                return true;
            }
            else
                return false;
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

namespace Yttrium
{
    namespace Chemical
    {
        const char * const LinearlyIndependentSet:: CallSign = "Chemical::LinearlyIndependentSet";

        LinearlyIndependentSet:: ~LinearlyIndependentSet() noexcept
        {
        }

        LinearlyIndependentSet:: LinearlyIndependentSet() : LinearlyIndependent::Set()
        {
        }


        void LinearlyIndependentSet:: operator()(const size_t primary,
                                                 const size_t species) 
        {
            {
                LinearlyIndependent::Ptr *pp = search(species);
                if(pp)
                {
                    (**pp).ensure(primary);
                    return;
                }
            }

            {
                LinearlyIndependent::Ptr p = new LinearlyIndependent(primary,species);
                if(!insert(p)) throw Specific::Exception(CallSign,"corrupted for #species=%u", unsigned(species));
            }
        }


        LinearlyIndependent & LinearlyIndependentSet:: operator[](const size_t species)
        {
            LinearlyIndependent::Ptr *pp = search(species);
            if(!pp) throw Specific::Exception(CallSign, "missing for #species=%u", unsigned(species));
            return **pp;
        }

    }

}
