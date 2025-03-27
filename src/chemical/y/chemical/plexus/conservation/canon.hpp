
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/chemical/type/meta-list.hpp"
#include "y/chemical/type/show-list.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            // definition for group of laws
            //
            //
            //__________________________________________________________________
            typedef Small::BareLightList<const Law> LList; //!< alias
            typedef LList::NodeType                 LNode; //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! list of interdependent laws
            //
            //
            //__________________________________________________________________
            class Canon : public Object, public LList, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Canon(const Law &first); //!< setup with first law
                virtual ~Canon() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                bool     accepts(const Law &)  const noexcept; //!< if common actor
                bool     accepts(const Canon&) const noexcept; //!< if common actor
                void     compile();                            //!< compile assembly or laws/species

                //! transfer according to species using '=' semantics
                template <typename TARGET, typename SOURCE> inline
                TARGET & transfer(TARGET &target, const Level targetLevel,
                                  SOURCE &source, const Level sourceLevel) const
                {
                    for(const SNode *sn=species->head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        sp(target,targetLevel) = sp(source,sourceLevel);
                    }
                    return target;
                }

                template <typename ARRAY> inline
                std::ostream & show(std::ostream &os, ARRAY &arr) const
                {
                    return ShowList(os,*species,sformat,AuxLevel, "[", arr, "]");
                }


                template <typename ARRAY, typename PROC> inline
                std::ostream & show(std::ostream &os, ARRAY &arr, PROC &fcn ) const
                {
                    return ShowList(os,*species,sformat,AuxLevel, "[", arr, "]"),fcn;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const AuxSList species; //!< Aux Level
                const Assembly sformat; //!< for species
                Canon *        next;    //!< for list
                Canon *        prev;    //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canon);
            };

        }

    }

}

#endif

