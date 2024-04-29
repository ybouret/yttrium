//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/proxy.hpp"

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
            //! conservation law using Actors
            //
            //
            //__________________________________________________________________
            class Law :  public Entity, public Proxy<const Actors>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Law>                 List;   //!< alias
                static const char * const              Colors; //!< default Color Scheme
                typedef Vector<xreal_t,Memory::Dyadic> VecType; //!< alias
                typedef Matrix<xreal_t,Memory::Dyadic> MatType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! initialize a new conservation law
                /**
                 - fill actors, left alpha and beta untouched
                 \param label law label, mostly for GraphViz
                 \param iboth index for entity Top/Sublevel
                 \param coeff coefficients, Sublevel
                 \param table species = table[1..coeff.size()]
                 */
                explicit Law(const String             &label,
                             const size_t              iboth,
                             const Readable<unsigned> &coeff,
                             const SpSubSet           &table);
                virtual ~Law() noexcept;                                       //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const Law &); //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const String &key()                             const noexcept; //!< return actors' name
                void          viz(OutputStream &fp)                      const; //!< GraphViz
                bool          sharesSpeciesWith(const Law &law) const noexcept; //!< check if common species with another law
                void          makeAlgebraic(const SList &species);              //!< make alpha and beta for species.size

                //! TODO
                xreal_t       required(Writable<xreal_t>       &dC,
                                       const Level             outgoing,
                                       const Readable<xreal_t> &C,
                                       const Level             incoming,
                                       XAdd                   &xadd) const;

                xreal_t required(Writable<xreal_t>       &Caux,
                                 const Readable<xreal_t> &Ctop,
                                 XAdd                    &Xadd) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xreal_t         nrm2;  //!< |this|^2
                const xreal_t         zero;  //!< 0
                const VecType         alpha; //!< coefficient in AuxLevel
                const MatType         beta;  //!< nrm2 * Id - alpha'*alpha

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                Actors       cast;
                virtual ConstInterface & surrogate() const noexcept;

            public:
                const String uuid; //!< made from actors
                Law         *next; //!< for list
                Law         *prev; //!< for list
            };
            


        }
    }
}

#endif
