
//! \file


#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1


#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! database of components
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String,const Component> ComponentsDB;


        //______________________________________________________________________
        //
        //
        //
        //! Situation of Components w.r.t Concentrations
        //
        //
        //______________________________________________________________________
        enum Situation
        {
            Running, //!< all strictly positive
            Blocked, //!< at least one zero concentration on EACH side
            Crucial  //!< at least one zero concentratoon on ONE side
        };

        //! human readable situation
        const char * SituationText(const Situation) noexcept;

        //______________________________________________________________________
        //
        //
        //
        //! Latchable Database of components
        //
        //
        //______________________________________________________________________
        class ComponentsType : public ComponentsDB, public Latchable
        {
        public:
            explicit ComponentsType();           //!< setup
            virtual ~ComponentsType() noexcept;  //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ComponentsType);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Qualification of Components
        //
        //
        //______________________________________________________________________
        enum ComponentsKind
        {
            Deserted, //!< no actors
            ReacOnly, //!< only reactant(s)
            ProdOnly, //!< only product(s)
            Standard  //!< both sides are populated
        };

        //______________________________________________________________________
        //
        //
        //
        //! named components
        //
        //
        //______________________________________________________________________
        class Components :
        public Indexed,
        public Proxy<const ComponentsType>,
        public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const             Symbol;          //!< "<=>";
            static const char                     Separator = ':'; //!< for parser
            typedef ComponentsType::ConstIterator ConstIterator;   //!< alias



            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Components(const String * const, const size_t); //!< setup
            virtual ~Components() noexcept;                          //!< cleanup
            Y_OSTREAM_PROTO(Components);                             //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void use(const Role, const unsigned, const Species &); //!< use new actor
            void use(const Role, const  Species &);                //!< use(role,1,species)
            void latch() noexcept;                                 //!< latch all

            //! check connected by species
            static bool AreConnected(const Components &lhs, const Components &rhs);

            //! fill compatible topology
            template <typename ARRAY> inline
            void fillTopology(ARRAY &topo, const Level level) const
            {
                typedef typename ARRAY::Type _T;
                for(const Actor *a=prod->head;a;a=a->next) a->sp(topo,level) =  _T(a->nu);
                for(const Actor *a=reac->head;a;a=a->next) a->sp(topo,level) = -_T(a->nu);
            }


            void gatherSpeciesIn(AddressBook &book) const; //!< OR'ed species into bool
            bool neutral()                 const noexcept; //!< check neutrality

            //! current activity
            xreal_t   activity(const xreal_t K, XMul &X, const XReadable &C, const Level L) const;

            //! shifted activity
            xreal_t   activity(const xreal_t K, XMul &X, const XReadable &C, const Level L, const xreal_t xi) const;

            //! Positive/Zeroed shifted reactant activity = K*reac.activity(C-Nu'*xi)
            xreal_t   activityReac(const xreal_t K, XMul &X, const XReadable &C, const Level L, const xreal_t xi) const;

            //! Negative/Zeroed shifted product activity = -1*prod.activity(C+Nu'*xi)
            xreal_t   activityProd(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const;


            //! current situation
            Situation situation(const XReadable &C, const Level L) const noexcept;

            //! cricitical concentration for reac or prod
            bool      critical(const XReadable &C, const Level L) const noexcept;

            //! move with forced clipping
            void      safeMove(XWritable &C, const Level L, const xreal_t xi) const noexcept;

            //! gather affinity with given constant
            xreal_t   affinity(const xreal_t K, XAdd &xadd, const XReadable &C, const Level L) const;

            //! evaluate extent from average of individual extents
            xreal_t   extent(XAdd &           xadd,
                             const XReadable &target,
                             const Level     targetLevel,
                             const XReadable &source,
                             const Level      sourceLevel) const;


            //! compute jacobian and return affinity, assuming zeroed array initially
            xreal_t jacobian(XWritable &xjac, const xreal_t K, XAdd &xadd, const XReadable &C, const Level L) const;

            //! emit GraphViz code one species are emitted
            void viz(OutputStream &fp,
                     const String * const color = 0) const;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            const Actors         reac; //!< current reactants
            const Actors         prod; //!< current products
            const ComponentsKind kind; //!< current kind

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            Y_PROXY_DECL();
        protected:
            ComponentsType db; //!< database
        };

    }

}

#endif

