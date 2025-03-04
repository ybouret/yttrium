
//! \file

#ifndef Y_Apex_Ortho_Coven_Survey_Of_Included
#define Y_Apex_Ortho_Coven_Survey_Of_Included 1

#include "y/apex/api/ortho/coven/survey.hpp"
#include "y/apex/api/ortho/coven/survey/sarray.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                template <typename T>
                class SurveyOf : public Survey, public Proxy< const ListOf< SArray<T> > >
                {
                public:
                    typedef SArray<T>                  ArrayType;
                    typedef Proxy< ListOf<ArrayType> > ProxyType;

                    inline virtual ~SurveyOf() noexcept
                    {
                    }

                    friend inline
                    std::ostream & operator<<(std::ostream &os, const SurveyOf &self)
                    {
                        os << '{';
                        if(self->size>0)
                        {
                            os << std::endl;
                            for(const ArrayType *node=self->head;node;node=node->next)
                            {
                                os << "  " << *node << std::endl;
                            }
                        }
                        os << '}';
                        return os;
                    }

                protected:
                    inline explicit SurveyOf() noexcept : my()
                    {
                    }

                    inline void finalize()
                    {
                        MergeSort::Call(my,ArrayType::Compare);
                    }

                    CxxListOf<ArrayType> my;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SurveyOf);
                    inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept
                    {
                        return my;
                    }

                };
            }

        }

    }

}

#endif
