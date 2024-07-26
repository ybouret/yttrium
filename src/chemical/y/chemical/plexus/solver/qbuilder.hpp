
//! \file

#ifndef Y_Chemical_QBuilder_Included
#define Y_Chemical_QBuilder_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/orthogonal/family.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Orthogonal::Family QFamily; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Orthogonal Family Builder, to find independent equilibria
        //
        //
        //______________________________________________________________________
        class QBuilder : public Quantized, public Counted, public Proxy<const PList>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<size_t,QBuilder> Ptr; //!< alias
            typedef HashSet<size_t,Ptr>     Set; //!< alis

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param primary number of primary equilibria (max base size)
             \param species number of involved species   (dimensions)
             \param probank shared bank of PNode
             */
            explicit QBuilder(const size_t primary,
                              const size_t species,
                              const PBank &probank);
            //! cleanup
            virtual ~QBuilder() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const size_t & key() const noexcept; //!< key for hash set
            void           init()      noexcept; //!< free list and family

            //! try insert prospect into family
            bool           grow(const Prospect &pro, const Matrix<int> &topo);

            //! ensure reservoir of QVectors
            void           ensure(const size_t primary);

            //! access internal family
            const QFamily &family() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QBuilder);
            PList   list;
            QFamily qfam;
            virtual ConstInterface & surrogate() const noexcept { return list; }
        };


        //______________________________________________________________________
        //
        //
        //
        //! Database of QBuilder for different number of species
        //
        //
        //______________________________________________________________________
        class QBuilders : public QBuilder::Set
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::QBuilders"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit QBuilders();           //!< setup empty
            virtual ~QBuilders() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! create/update QBuilder
            void operator()(const size_t primary,
                            const size_t species,
                            const PBank &probank);

            //! QBuilber for number of species, throw on not present
            QBuilder & operator[](const size_t species);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QBuilders);
        };

    }

}

#endif

