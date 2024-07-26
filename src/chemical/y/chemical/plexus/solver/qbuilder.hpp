
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
        class QBuilder : public Quantized, public Counted, public Proxy<const PList>
        {
        public:
            typedef ArkPtr<size_t,QBuilder> Ptr;
            typedef HashSet<size_t,Ptr>     Set;

            explicit QBuilder(const size_t primary,
                              const size_t species,
                              const PBank &probank);

            virtual ~QBuilder() noexcept;

            const size_t & key() const noexcept;
            void           init()      noexcept;
            bool           grow(const Prospect &pro, const Matrix<int> &topo);
            void           ensure(const size_t primary);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(QBuilder);
            PList              list;
            Orthogonal::Family qfam;
            virtual ConstInterface & surrogate() const noexcept { return list; }
        };


        class QBuilders : public QBuilder::Set
        {
        public:
            static const char * const CallSign;

            explicit QBuilders();
            virtual ~QBuilders() noexcept;

            void operator()(const size_t primary,
                            const size_t species,
                            const PBank &probank);

            QBuilder & operator[](const size_t species);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QBuilders);
        };

    }

}

#endif

