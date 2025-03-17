
//! \file


#ifndef Y_Chemical_MixedEquilibrium_Included
#define Y_Chemical_MixedEquilibrium_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareHeavyList<const int> WList; //!< alias
        typedef WList::NodeType                 WNode; //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! based on a weighted combination of equilibria
        //
        //
        //______________________________________________________________________
        class MixedEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create by stealing lists and latch
            explicit MixedEquilibrium(WList               &wl,
                                      EList               &el,
                                      const SList         &sl,
                                      const Readable<int> &st,
                                      const size_t         ii,
                                      XWritable           &KK);

            //! cleanup
            virtual ~MixedEquilibrium() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            XWritable   &topK;  //!< top level, shared K
            const WList  wlist; //!< weights of equilibria
            const EList  elist; //!< equilibria
            XMul         xmul;  //!< for internal computation


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(xreal_t)
            {
                return 1;
            }

            static String        FirstName(const int   cf, const String &id);
            static String        ExtraName(const int   cf, const String &id);
            static const String *MakeName(const WList &wl, const EList  &el);
        };

    }

}


#endif

