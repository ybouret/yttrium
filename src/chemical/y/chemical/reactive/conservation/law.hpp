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
            class Law :  public Proxy<const Actors>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Law> List; //!< alias
         
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Law(const Readable<unsigned> &, const SpSubSet &);    //!< setup from SubLevel info
                virtual ~Law() noexcept;                                       //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const Law &); //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const String &key() const noexcept; //!< return actors' name

                bool sharesSpeciesWith(const Law &law) const noexcept;


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Law *next; //!< for list
                Law *prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                Actors       cast;
                const String uuid;

                virtual ConstInterface & surrogate() const noexcept;
            };

            //typedef Small::BareLightList<const Law> clList;
            //typedef clList::NodeType                clNode;


        }
    }
}

#endif
