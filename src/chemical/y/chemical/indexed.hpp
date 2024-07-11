
//! \file

#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/entity.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! index level
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel, //!< top     index
            SubLevel, //!< sub     index
            AuxLevel  //!< sub-sub index
        };


        //______________________________________________________________________
        //
        //
        //
        //! Entity with indices
        //
        //
        //______________________________________________________________________
        class Indexed : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t Levels = AuxLevel+1; //!< number of indices

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with identifier and top level index
            template <typename ID> inline
            explicit Indexed(const ID &   userName,
                             const size_t topLevel) :
            Entity(userName), indx()
            {
                setAllIndices(topLevel);
            }
            virtual ~Indexed() noexcept; //!< cleanup
            

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! algorithm to sort lists according to level
            template <Level LEVEL>
            struct SortBy
            {
                //! dedicated comparison
                template <typename NODE> static inline
                SignType CompareNodes(const NODE *lhs, const NODE *rhs) noexcept
                {
                    return Sign::Of( (**lhs).indx[LEVEL], (**rhs).indx[LEVEL] );
                }

                //! call algorithm
                template <typename NODE> static inline
                void Using(ListOf<NODE> &L)
                {
                    MergeSort::Call(L, CompareNodes<NODE> );
                }
            };

            //! algorithm to revamp indices within a list
            template <Level LEVEL>
            struct Revamp
            {
                //! call algorithm
                template <typename NODE> static inline
                void Using(ListOf<NODE> &L)
                {
                    size_t i=0;
                    for(NODE *node=L.head;node;node=node->next)
                        Coerce( (**node).indx[LEVEL] ) = ++i;
                }
            };

            //! SortBy TopLevel, Revamp SubLevel
            template <typename NODE> static inline
            void SubOrganize(ListOf<NODE> &L)
            {
                SortBy<TopLevel>::Using(L);
                Revamp<SubLevel>::Using(L);
            }

            //! SortBy TopLevel, Revamp AuxLevel
            template <typename NODE> static inline
            void AuxOrganize(ListOf<NODE> &L)
            {
                SortBy<TopLevel>::Using(L);
                Revamp<AuxLevel>::Using(L);
            }




            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t indx[Levels];  //!< indices

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
            void setAllIndices(const size_t level);
        };



    }
}

#endif
