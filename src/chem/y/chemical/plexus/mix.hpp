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

        //______________________________________________________________________
        //
        //
        //
        //! Mix of interlinked equilibria
        //
        //
        //______________________________________________________________________
        class Mix : public Oversized, public Proxy<const Connected>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const                           CallSign;         //!< "Chemical::Mix"
            typedef CxxListOf<Mix>                              List;             //!< alias
            typedef AutoPtr<const Conservation::Laws,DrillDown> ConservationLaws; //!< alias
            typedef CxxArray<EList,MemoryModel>                 ELists;           //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Mix(const Equilibrium &first); //!< initialize
            virtual ~Mix() noexcept;                //!< cleanup
            Y_OSTREAM_PROTO(Mix);                   //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool accepts(const Equilibrium &) const noexcept; //!< if shared species
            bool accepts(const Mix &)         const noexcept; //!< if shared species
            void addPrimary(const Equilibrium &);             //!< add primary equilibrium
            void addPrimary(const Mix &);                     //!< add primary connected mix

            //! compile once built
            void buildConfiguration(XMLog &xml, Equilibria &eqs);


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
            void buildConservations(XMLog &xml);
            void buildCombinatorics(XMLog &xml, Equilibria &eqs);
            
            Connected my;

        public:
            const Matrix<unsigned> conservancy; //!< conservancy matrix
            const ConservationLaws laws;        //!< conservation laws
            const ELists           order;       //!< at least 1
            Mix * next; //!< for list
            Mix * prev; //!< for list
        };

    }

}

#endif

