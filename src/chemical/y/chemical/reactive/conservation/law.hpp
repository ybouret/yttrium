//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"
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
                typedef CxxListOf<Law>    List;   //!< alias
                static const char * const Colors; //!< default Color Scheme

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Law(const String             &label,
                             const size_t              iboth,
                             const Readable<unsigned> &coeff,
                             const SpSubSet           &table);                 //!< setup from SubLevel info
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
                
                xreal_t       required(Writable<xreal_t>       &dC,
                                       const Level             outgoing,
                                       const Readable<xreal_t> &C,
                                       const Level             incoming,
                                       XAdd                   &xadd) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xreal_t nrm2; //!< |this|^2
                const xreal_t zero; //!< 0

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                Actors       cast;
                const String uuid;
                virtual ConstInterface & surrogate() const noexcept;
            public:
                Law        *next; //!< for list
                Law        *prev; //!< for list
            };

            //typedef Small::BareLightList<const Law> clList;
            //typedef clList::NodeType                clNode;


        }
    }
}

#endif
