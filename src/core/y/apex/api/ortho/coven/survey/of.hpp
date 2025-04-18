
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
                //______________________________________________________________
                //
                //
                //
                //! Survey + arrays
                //
                //
                //______________________________________________________________
                template <typename T>
                class SurveyOf : public Survey, public Proxy< const ListOf< SArray<T> > >
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef SArray<T>                  ArrayType; //!< alias
                    typedef Proxy< ListOf<ArrayType> > ProxyType; //!< alias

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! cleanup
                    inline virtual ~SurveyOf() noexcept
                    {
                    }

                    //! display
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
                    //! setup empty
                    inline explicit SurveyOf() noexcept :
                    my()
                    {
                    }

                    //! finalize arrays by sorting them
                    inline void finalize()
                    {
                        MergeSort::Call(my,ArrayType::Compare);
                    }

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    CxxListOf<ArrayType> my; //!< list instance

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
