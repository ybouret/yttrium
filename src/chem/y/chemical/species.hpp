
//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/indexed.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/chemical/formula.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/suffix/set.hpp"

#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/associative/address-book.hpp"
#include "y/sort/merge.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Species is a charged entity
        //
        //
        //______________________________________________________________________
        class Species :
        public Indexed,
        public Counted,
        public Formula
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<const String,const Species>   Handle;   //!< alias
            typedef SuffixSet<const String,const Handle> Set;      //!< alias
            static const char * const                    CallSign; //!< "Chemical::Species"
            static const char * const                    Scheme;   //!< color scheme

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! build with XCode, Formula, string...
            template <typename DATA> inline
            Species(const DATA &data, const size_t topIndex) :
            Indexed(topIndex),
            Formula(data)
            {

            }

            //! cleanup
            virtual ~Species() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept; //! [Entity] name

            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            String makeColor() const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };


        typedef Small::BareLightList<const Species> SList; //!< alias
        typedef SList::NodeType                     SNode; //!< alias
        typedef Small::CoopLightList<const Species> SRepo; //!< alias
        typedef SRepo::ProxyType                    SBank; //!< alias

        //! operators
        struct DBOps
        {

            //! extract stored addresses from book into target list
            template <typename LIST> static inline
            void Extract(LIST &target, const AddressBook &book)
            {
                size_t                     n = book.size();
                AddressBook::ConstIterator i = book.begin();
                while(n-- > 0)
                {
                    const void * const addr = *(i++); assert(0!=addr);
                    target << *static_cast<typename LIST::ConstType *>(addr);
                }
            }

            template <typename LIST>
            struct Revamp
            {
                typedef typename LIST::NodeType NODE;

                template <const Level LEVEL> static inline
                SignType Compare(const NODE * const lhs, const NODE * const rhs) noexcept
                {
                    const Indexed &l = **lhs;
                    const Indexed &r = **rhs;
                    return Sign::Of(l.indx[LEVEL],r.indx[LEVEL]);
                }

                static inline
                void Sort(LIST &list) noexcept
                {
                    MergeSort::Call(list,Compare<TopLevel>);
                }

                template <const Level LEVEL> static inline
                void Indx(LIST &list) noexcept
                {
                    Y_STATIC_CHECK(LEVEL>TopLevel,BadLevel);
                    size_t i=0;
                    for(typename LIST::NodeType *node=list.head;node;node=node->next)
                    {
                        Coerce( (**node).indx[LEVEL] ) = ++i;
                    }
                }

                template <const Level LEVEL> static inline
                void At(LIST &list) noexcept
                {
                    Sort(list);
                    Indx<LEVEL>(list);
                }



            };







#if 0
            template <typename LIST> static inline
            void RevampSub(LIST &list) noexcept
            {
                Sort<TopLevel,LIST>(list);
                Index<SubLevel,LIST>(list);
            }
#endif

        };

    }

}

#endif

