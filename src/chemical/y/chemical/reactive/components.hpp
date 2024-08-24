//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/assembly.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<String, Component::Ptr> Manifest; //!< alias

        //! situation of an equilibrium
        enum Situation
        {
            Blocked, //!< deficient in both reactant and product
            Running, //!< all requested components
            Crucial  //!< crucially lacking components
        };

        //! category according to cardinality
        enum Category
        {
            Nebulous, //!< no reac/prod => bad!
            ReacOnly, //!< reactant(s) only
            ProdOnly, //!< product(s) only
            Standard  //!< reactant(s) AND product(s)
        };

        //! human readable category
        const char * CategoryText(const Category) noexcept;

        //______________________________________________________________________
        //
        //
        //
        //! Components: database of reac+prod
        //
        //
        //______________________________________________________________________
        class Components : public Indexed, public Proxy<Manifest>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Manifest::ConstIterator ConstIterator; //!< alias
            static const char * const       Mark;          //!< "<=>";




            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            template <typename ID>
            explicit Components(const ID &   userName,
                                const size_t topLevel) :
            Indexed(userName,topLevel),
            Proxy<Manifest>(),
            reac(),
            prod(),
            kind(Nebulous),
            db(),
            zero( 0),
            mOne(-1),
            pOne(+1)
            {
            }

            //! cleanup
            virtual ~Components() noexcept;



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert and dispatch a new component and matching actor
            void operator()(const int      nu,
                            const Species &sp);

            //! regularized mass action
            xreal_t massAction(const xreal_t     K,
                               XMul            & xmul,
                               const XReadable & C,
                               const Level       level) const;

            //! regularized mass action at C0+nu'*xi
            xreal_t massAction(const xreal_t     K,
                               XMul            & xmul,
                               const XReadable & C,
                               const Level       L,
                               const xreal_t     xi) const;

            //! get affinity from fully valid concentration
            real_t affinity(const xreal_t    K,
                            XMul            &xmul,
                            const XReadable &C,
                            const Level      L) const;

            //! d_massAction/d_C
            void drvsMassAction(const xreal_t      K,
                                XWritable  &       drvs,
                                const Level        Lout,
                                const XReadable &  Cinp,
                                const Level        Linp,
                                XMul             & xmul) const;

            //! derivative of mass action at Cinp+nu*xi
            void drvsMassAction(const xreal_t     K,
                                XWritable       & drvs,
                                XWritable       & Cout,
                                const Level       Lout,
                                XMul            & xmul,
                                const XReadable & Cinp,
                                const Level       Linp,
                                const xreal_t     xi) const;

            //! load topology vector
            template <typename T> inline
            void topology(Writable<T> &Nu, const Level level) const
            {
                { const T _z(0); Nu.ld(_z); }
                size_t            n  = db.size();
                for(ConstIterator it = db.begin();n>0;--n,++it)
                {
                    const Component &cc = **it;
                    Nu[ cc.sp.indx[level] ] = cc.nu;
                }
            }

            //! Nu * array[level]
            xreal_t dot(const XReadable & array,
                        const Level       level,
                        XAdd &            xadd) const;

            //! lhs[level] * rhs[level]
            xreal_t dot(const XReadable &lhs,
                        const XReadable &rhs,
                        const Level      level,
                        XAdd            &xadd) const;


            //! transfer components-wise
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, const Level tgtLevel,
                          SOURCE &source, const Level srcLevel) const
            {
                size_t            n  = db.size();
                for(ConstIterator it = db.begin();n>0;--n,++it)
                {
                    const Species &sp = (**it).sp;
                    target[sp.indx[tgtLevel]] = source[sp.indx[srcLevel]];
                }
            }

            //! move assuming xi is valid
            void moveSafe(XWritable    &C,
                          const Level   L,
                          const xreal_t xi) const;

            //! Cout = (Cinp + xi * nu) >= 0
            void addSafe(XWritable       &Cout,
                         const Level      Lout,
                         const XReadable &Cinp,
                         const Level     &Linp,
                         const xreal_t    xi) const;



            void updateKind()                                  noexcept; //!< update kind according to category
            bool linkedTo(const Species    &sp)          const noexcept; //!< linked to species
            bool linkedTo(const Components &)            const noexcept; //!< linked to other compoennts
            void record(AddressBook &)                            const; //!< record (add) all species
            bool neutral()                               const noexcept; //!< return neutrality check
            bool crucial(const XReadable &, const Level) const noexcept; //!< check if crucial

            
            //! linked to a conserved species
            bool linkedToConserved(const Species     &sp,
                                   const AddressBook &conserved) const noexcept;

            //! linked by conserved common species
            bool linkedToConserved(const Components  &other,
                                   const AddressBook &conserved) const noexcept;


            //! assuming species are already in fp
            void viz(OutputStream &fp, const String &color) const;


            //! display compact array
            template <typename ARRAY> inline
            std::ostream & displayCompact(std::ostream &os, ARRAY &arr, const Level level) const
            {
                os << '{' << ' ';
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    const Species &sp = (**it).sp;
                    os << '[' << sp.name << ']' << '=' << real_t( arr[sp.indx[level]] ) << ' ';
                }
                os << '}';
                return os;
            }

            //! human readable kind
            const char *kindText() const noexcept;

            //! check
            bool analogous(const Components &other) const noexcept
            {
                return (reac.akin(other.reac) && prod.akin(other.prod))
                ||     (reac.akin(other.prod) && prod.akin(other.reac));
            }

            //! check all accounted, throw exception otherwise
            void mustSupport(const XReadable &C, const Level L) const;

            //! check if species is a component
            const Component * query(const Species &sp) const noexcept;
            bool              found(const Actors  &ac) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors             reac; //!< reactants
            const Actors             prod; //!< products
            const Category           kind; //!< precomputed category

        private:
            Manifest db;
            virtual ConstInterface & surrogate() const noexcept;

        public:
            const xreal_t zero; //!< value : 0
            const xreal_t mOne; //!< value : -1
            const xreal_t pOne; //!< value : +1

        };
    }

}

#endif

