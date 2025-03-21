
//! \file

#ifndef Y_Chemical_Squad_Included
#define Y_Chemical_Squad_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! species and their assembly
        //
        //
        //______________________________________________________________________
        class Squad
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Squad() noexcept; //!< setup
            virtual ~Squad() noexcept; //!< cleanup
            Squad(const Squad &_);     //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! formatted, species-wise display at given level
            template <typename ARRAY> inline
            std::ostream & display(std::ostream &     os,
                                   const char * const pfx,
                                   ARRAY &            arr,
                                   const char * const sfx,
                                   const Level        lvl) const
            {
                if(species.size<=0)
                    os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(const SNode *it=species.head;it;it=it->next)
                    {
                        const Species &sp = **it;
                        sformat.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << sp(arr,lvl);
                        if(0!=it->next)
                            os << std::endl;
                        else
                            os << ' ' <<  '}';
                    }
                }
                return os;
            }


            //! formatted, species-wise display at given level
            template <typename ARRAY, typename PROC> inline
            std::ostream & display(std::ostream &     os,
                                   const char * const pfx,
                                   ARRAY &            arr,
                                   const char * const sfx,
                                   const Level        lvl,
                                   PROC              &proc) const
            {
                if(species.size<=0)
                    os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(const SNode *it=species.head;it;it=it->next)
                    {
                        const Species &sp = **it;
                        sformat.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << proc(sp(arr,lvl));
                        if(0!=it->next)
                            os << std::endl;
                        else
                            os << ' ' <<  '}';
                    }
                }
                return os;
            }

            //! load zero for each species
            template <typename ARRAY> inline
            void ldz(ARRAY &arr, const Level L) const
            {
                typename ARRAY::ConstType _0 = 0;
                for(const SNode *it=species.head;it;it=it->next)
                {
                    const Species &sp = **it;
                    sp(arr,L) = _0;
                }
            }


            //! post build if necessary
            void buildSpeciesFormat() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SList    species; //!< list of species
            Assembly sformat; //!< assembly for species format

        private:
            Y_DISABLE_ASSIGN(Squad);
        };


    }

}

#endif

