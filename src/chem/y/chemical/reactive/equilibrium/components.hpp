//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/equilibrium/component.hpp"
#include "y/chemical/reactive/equilibrium/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! Attribute for Components
        enum Attribute
        {
            Nebulous, //!< no component
            ReacOnly, //!< only reactants
            ProdOnly, //!< only products
            Definite  //!< reactants+products
        };


        //! Situation for Components
        enum Situation {
            Blocked, //!< at least one reac AND one prod are zero
            Crucial, //!< one side is OK, the other one has at least one zero reac
            Running  //!< all components have positive concentration
        };

        //______________________________________________________________________
        //
        //
        //
        //! internal component database
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String, const Component> Compendium;




        //______________________________________________________________________
        //
        //
        //
        //! database of components
        //
        //
        //______________________________________________________________________
        class Components :
        public Indexed,
        public Proxy<const Compendium>,
        public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Compendium::ConstIterator ConstIterator; //!< alias
            static const char * const         Scheme;        //!< color scheme
            static const char * AttributeText(const Attribute) noexcept; //!< human readable attribute
            static const char * SituationText(const Situation) noexcept; //!< human readable situation

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create empty
            template <typename NAME> inline
            explicit Components(const NAME & _name,
                                const size_t _indx) :
            Indexed(_indx),
            Proxy<const Compendium>(),
            name(_name),
            reac(NameAsNormalSpecies),
            prod(NameAsNormalSpecies),
            d_nu(0),
            cmdb(),
            attr(Nebulous),
            one(1)
            {
            }

            //! cleanup
            virtual ~Components() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept; //!< [Entity] name

            //__________________________________________________________________
            //
            //
            // Methods to setup
            //
            //__________________________________________________________________

            //! create new actor with role
            void add(const Role role, const unsigned nu, const Species &sp);

            //! create new actor with role and unit coefficient
            void add(const Role role, const Species &sp);


            //! quick display
            std::ostream & print(std::ostream &) const;

            //! make color from TopLevel index and Scheme
            String makeColor() const;

            //! display name + arrows, but NOT the species
            void viz(OutputStream &fp,
                     const char * const color,
                     const char * const style) const;

            //! standalone graph
            void graphViz(OutputStream &) const;

            //__________________________________________________________________
            //
            //
            // Methods to query
            //
            //__________________________________________________________________
            bool neutral()                      const; //!< check charge conservation
            bool working()             const noexcept; //!< not nebulous
            void checkpoint(const char * const) const; //!< neutral and working in given function
            void addSpeciesTo(AddressBook &)    const; //!< OR'd species into book
            bool included(const Species &)      const; //!< test if species is in components (name+ref)
            bool linkedTo(const Components &)   const; //!< check if at least one species is shared


            xReal activity(XMul &xmul, const xReal K, const XReadable &C, const Level L) const;                 //!< delta activities

            //! K * product of activities at -xi
            xReal reacActivity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const;

            //! - 1*product of activities at +xi
            xReal prodActivity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const;

            //! reacActivity+prodActivity
            xReal activity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const;


            //! transfer only components from source to target
            template <typename TARGET, typename SOURCE> inline
            TARGET & transfer(TARGET &target, const Level targetLevel,
                              SOURCE &source, const Level sourceLevel) const
            {
                size_t        n = cmdb.size();
                ConstIterator i = cmdb.begin();
                while( n-- > 0 )
                {
                    const Species &s = (*i).actor.sp;
                    s(target,targetLevel) = s(source,sourceLevel);
                    ++i;
                }
                return target;
            }


            Situation    situation(const XReadable &C, const Level L) const noexcept; //!< situation from concentrations
            const char * attrText()                                   const noexcept; //!< human readable attribute

            //! bracket RUNNING
            SignType bracket(XTriplet &       xi,
                             XTriplet &       ff,
                             XMul    &        xmul,
                             const xReal      K,
                             const XReadable &C,
                             const Level      L) const;

            //! move assuming positivity
            void moveSafely(XWritable &C, const Level L, const xReal xi) const noexcept;

            //! fill topology matrix
            template <typename ARRAY> inline
            void topology(ARRAY &Nu, const Level L) const
            {
                for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
                {
                    const Component &cm = *it;
                    const Actor     &ac = cm.actor;
                    const int        nu = int(ac.nu);
                    switch(cm.role)
                    {
                        case Product:  ac.sp(Nu,L) =  nu; break;
                        case Reactant: ac.sp(Nu,L) = -nu; break;
                    }
                }
            }

            template <typename ARRAY> inline
            std::ostream & displayCompact(std::ostream &os, ARRAY &C, const Level L) const
            {
                os << '{';
                size_t i=0;
                for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it,++i)
                {
                    const Species &sp = (*it).actor.sp;
                    os << '[' << sp.name << ']' << '=' << real_t( sp(C,L) );
                    if(i>0) os << ';';
                }
                os << '}';
                return os;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String name; //!< unique name
            const Actors reac; //!< reactants
            const Actors prod; //!< products
            const int    d_nu; //!< sum of signed nu

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            virtual ConstInterface & surrogate() const noexcept;
            Compendium cmdb;
            Actors & actorsPlaying(const Role);
            void     updateAttribute() noexcept;
            SignType positiveBracket(XTriplet &       xi,
                                     XTriplet &       ff,
                                     XMul    &        xmul,
                                     const xReal      K,
                                     const XReadable &C,
                                     const Level      L) const;
            SignType negativeBracket(XTriplet &       xi,
                                     XTriplet &       ff,
                                     XMul    &        xmul,
                                     const xReal      K,
                                     const XReadable &C,
                                     const Level      L) const;
        public:
            const Attribute attr; //!< current attibute
            const xReal     one;  //!< alias
        };

    }

}

#endif

