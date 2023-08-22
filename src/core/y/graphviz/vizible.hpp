//! \file

#ifndef Y_Vizible_Included
#define Y_Vizible_Included 1

#include "y/string.hpp"
#include "y/stream/output.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    
    //__________________________________________________________________________
    //
    //
    //
    //! Helper to create GraphViz directed graphs
    //
    //
    //__________________________________________________________________________
    class Vizible
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Vizible() noexcept; //!< setup
        virtual ~Vizible() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // API
        //
        //______________________________________________________________________
        static OutputStream &Addr(OutputStream &fp, const void *   ptr);                    //!< emit address
        static OutputStream &Node(OutputStream &fp, const Vizible *viz);                    //!< emit UUID
        static OutputStream &Endl(OutputStream &);                                          //!< ";\n"
        static OutputStream &Text(OutputStream &,const String &);                           //!< encode text
        static OutputStream &Text(OutputStream &,const char   *);                           //!< encode text
        static OutputStream &Arrow(OutputStream &, const Vizible *src, const Vizible *dst); //!< start arrow
        static OutputStream &Label(OutputStream &, const String &);                         //!< label="..."
        static OutputStream &Label(OutputStream &, const char   *);                         //!< label="...

        //! start a directed graph
        static void Enter(OutputStream &fp, const String &graphName);
        static void Enter(OutputStream &fp, const char   *graphName);

        //! finish a directed grapj
        static void Leave(OutputStream &);

        //! 'dot -T png -o pngFile dotFile'
        static void Render(const String &pngFile,
                           const String &dotFile,
                           const bool    keepDot = false);

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        OutputStream & node(OutputStream &) const;                    //!< emit self uuid
        OutputStream & arrow(OutputStream &f, const Vizible *) const; //!< arrow from self to other


        //! save and render any CLASS with a graphViz(OutputStream &) methods
        template <typename CLASS>
        static inline void GraphViz(const String &dotFile,
                                    CLASS        &vizible)
        {
            {
                AutoPtr<OutputStream> fp = OpenFile(dotFile);
                vizible.graphViz(*fp);
            }
            RenderPNG(dotFile);
        }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(Vizible);
        static OutputStream *OpenFile(const String &dotFile);
        static void          RenderPNG(const String &dotFile);
    };

}

#endif

