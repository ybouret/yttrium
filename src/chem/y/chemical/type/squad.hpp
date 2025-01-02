
//! \file

#ifndef Y_Chemical_Squad_Included
#define Y_Chemical_Squad_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Squad
        {
        public:
            explicit Squad() noexcept;
            virtual ~Squad() noexcept;
            Squad(const Squad &_);

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

            //! post build if necessary
            void buildSpeciesFormat() noexcept;
            

            SList    species;
            Assembly sformat;

        private:
            Y_DISABLE_ASSIGN(Squad);
        };


    }

}

#endif

