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
         
            //! category according to cardinality
            enum Category
            {
                Nebulous, //!< no reac/prod => bad!
                ReacOnly, //!< reactant(s) only
                ProdOnly, //!< product(s) only
                Standard  //!< reactant(s) AND product(s)
            };

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
            zero(0),
            mOne(-1)
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
                               const XReadable & C0,
                               const xreal_t     xi,
                               const Level       level) const;

            //! d_massAction/d_C
            void drvsMassAction(const xreal_t      K,
                                XWritable  &       phi,
                                const Level        output,
                                const XReadable &  C,
                                const Level        input,
                                XMul             &xmul) const;

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

            //! update kind according to category
            void updateKind() noexcept;

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


            bool linkedTo(const Species    &sp) const noexcept; //!< linked to species
            bool linkedTo(const Components &)   const noexcept; //!< linked to other compoennts
            void record(AddressBook &) const;                   //!< record (add) all species
            bool neutral()    const noexcept;                   //!< return neutrality check

            //! assuming species are already in fp
            void viz(OutputStream &fp,
                     const String &color) const;


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

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors   reac; //!< reactants
            const Actors   prod; //!< products
            const Category kind; //!< precomputed category
            

        private:
            Manifest db;
            virtual ConstInterface & surrogate() const noexcept;

        public:
            const xreal_t zero; //!< value
            const xreal_t mOne; //!< value


        };
    }

}

#endif

