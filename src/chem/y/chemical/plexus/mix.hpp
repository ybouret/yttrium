//! \file

#ifndef Y_Chemical_Mix_Included
#define Y_Chemical_Mix_Included 1

#include "y/chemical/plexus/connected.hpp"
#include "y/chemical/plexus/conservation/authority.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Equilibria;

        //______________________________________________________________________
        //
        //
        //
        //! Duplex List/Book to classify elements
        //
        //
        //______________________________________________________________________
        template <typename LIST>
        class Duplex
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline  Duplex() : list(), book() {} //!< setup
            inline ~Duplex() noexcept {}         //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build list from book
            void compile() {
                assert(0==list.size);
                DBOps::Revamp<LIST>::Sort( book.sendTo(Coerce(list)) );
                assert(book.size()==list.size);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const LIST        list; //!< ordered list
            const AddressBook book; //!< dictionary


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Duplex);
        };


        typedef Duplex<SList> SDuplex; //!< alias
        typedef Duplex<EList> EDuplex; //!< alias





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
            typedef AutoPtr<const Conservation::Authority>      ConservationAuth; //!< alias
            typedef CxxArray<EList,MemoryModel>                 ELists;           //!< alias

            //__________________________________________________________________
            //
            //
            //! Genus for unbounded/conserved species
            //
            //__________________________________________________________________
            class Genus : public Object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Genus(const ConservationAuth & , const SList &); //!< setup
                virtual ~Genus() noexcept;                                //!< cleanup

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________
                bool hasConserved(const Actors &)    const noexcept; //!< at least one conserved actor
                bool isLimiting(const Equilibrium &) const noexcept; //!< at least one conserved reac and prod

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const SDuplex conserved; //!< duplex of conserved species
                const SDuplex unbounded; //!< duplex of unbounded species

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Genus);
            };

            //__________________________________________________________________
            //
            //
            //! Grade for equilibria
            //
            //__________________________________________________________________
            class Grade : public Object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Grade(const EList &, const Genus &); //!< setup
                virtual ~Grade() noexcept;                    //!< cleanup

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const EDuplex prodOnly;    //!< product only
                const EDuplex reacOnly;    //!< reactant only
                const EDuplex oneSided;    //!< reacOnly + prodOnly
                const EDuplex limiting;    //!< limiting equilibria
                const EDuplex floating;    //!< floating equilibria

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grade);
            };


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
            const Matrix<unsigned>     conservancy; //!< conservancy matrix
            const ConservationAuth     auth;        //!< conservation authority
            const ELists               order;       //!< at least 1
            const AutoPtr<const Genus> genus;   //!< conserved/unbounded
            const AutoPtr<const Grade> grade;   //!< graded equilibria


            Mix * next; //!< for list
            Mix * prev; //!< for list
        };

    }

}

#endif

