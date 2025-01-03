//! \file

#ifndef Y_Chemical_Conservation_Act_Included
#define Y_Chemical_Conservation_Act_Included 1

#include "y/chemical/plexus/conservation/law.hpp"


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
            //! group of dependent laws
            //
            //
            //__________________________________________________________________
            class Act : public Proxy<const Law::Group>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Act> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Act(const Law &first); //!< setup
                virtual ~Act() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                bool accepts(const Law &) const noexcept; //!< if law is linked to mine
                bool accepts(const Act &) const noexcept; //!< if one of the law is linked to mine
                void collect(const Law &);                //!< collect law
                void collect(const Act &);                //!< collect laws
                void compile();                           //!< gather species, make their AuxLevel indices and sformat

                //! transfer to target from source using my species
                template <typename TARGET, typename SOURCE> inline
                TARGET & transfer(TARGET     &target,
                                  const Level tgt,
                                  SOURCE     &source,
                                  const Level src) const
                {
                    for(const SNode *sn=my.species.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        sp(target,tgt) = sp(source,src);
                    }
                    return target;
                }

                //! formatted, species-wise display at AuxLevel
                template <typename ARRAY> inline
                std::ostream & operator()(std::ostream &     os,
                                          const char * const pfx,
                                          ARRAY &            arr,
                                          const char * const sfx) const
                {
                    return my.display(os, pfx, arr, sfx, AuxLevel);
                }


                //! formatted, species-wise display at AuxLevel
                template <typename ARRAY, typename PROC> inline
                std::ostream & operator()(std::ostream &     os,
                                          const char * const pfx,
                                          ARRAY &            arr,
                                          const char * const sfx,
                                          PROC              &proc) const
                {
                    return my.display(os, pfx, arr, sfx, AuxLevel, proc);
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Act);
                Y_PROXY_DECL();
                
                Law::Group my;
                
            public:
                Act * next; //!< for list
                Act * prev; //!< for list
            };

        }

    }

}

#endif
