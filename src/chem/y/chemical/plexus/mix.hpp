//! \file

#ifndef Y_Chemical_Mix_Included
#define Y_Chemical_Mix_Included 1

#include "y/chemical/plexus/connected.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"
#include "y/ptr/drill-down.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Equilibria;

        class Mix : public Oversized, public Proxy<const Connected>
        {
        public:
            typedef CxxListOf<Mix> List;
            typedef AutoPtr<const Conservation::Laws,DrillDown> ConservationLaws;

            explicit Mix(const Equilibrium &first);
            virtual ~Mix() noexcept;
            Y_OSTREAM_PROTO(Mix);
            

            bool accepts(const Equilibrium &) const noexcept;
            bool accepts(const Mix &)         const noexcept;
            void addPrimary(const Equilibrium &);
            void addPrimary(const Mix &);
            void buildConfiguration(Equilibria &eqs, XMLog &xml);
            void buildConservations(XMLog &xml);
            void buildCombinatorics(Equilibria &eqs, XMLog &xml);

            //! formatted, species-wise display at SubLevel
            template <typename ARRAY> inline
            std::ostream & operator()(std::ostream &     os,
                                      const char * const pfx,
                                      ARRAY &            arr,
                                      const char * const sfx) const
            {
                if(my.species.size<=0)
                    os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(const SNode *it=my.species.head;it;it=it->next)
                    {
                        const Species &sp = **it;
                        my.sformat.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << sp(arr,SubLevel);
                        os << std::endl;
                    }
                    os << '}';
                }
                return os;
            }


            //! formatted, species-wise display at SubLevel
            template <typename ARRAY, typename PROC> inline
            std::ostream & operator()(std::ostream &     os,
                                      const char * const pfx,
                                      ARRAY &            arr,
                                      const char * const sfx,
                                      PROC              &proc) const
            {
                if(my.species.size<=0)
                    os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(const SNode *it=my.species.head;it;it=it->next)
                    {
                        const Species &sp = **it;
                        my.sformat.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << proc(sp(arr,SubLevel));
                        os << std::endl;
                    }
                    os << '}';
                }
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mix);
            virtual ConstInterface & surrogate() const noexcept;
            Connected my;

        public:
            const Matrix<unsigned> conservancy;
            const ConservationLaws laws;
            Mix * next;
            Mix * prev;
        };

    }

}

#endif

