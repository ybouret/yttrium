//! \file

#ifndef Y_Vizible_Included
#define Y_Vizible_Included 1

#include "y/string.hpp"
#include "y/stream/output.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    namespace GraphViz
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
            static OutputStream &Label(OutputStream &, const void   *, const size_t);           //!< label="...
            static OutputStream &Label(OutputStream &, const String &);                         //!< label="..."
            static OutputStream &Label(OutputStream &, const char   *);                         //!< label="...
            static OutputStream &Color(OutputStream &, const String &scheme, unsigned indx);    //!< emit \"/schemeXX/indx\" or \"indx\"
            static OutputStream &Color(OutputStream &, const char   *scheme, unsigned indx);    //!< emit \"/schemeXX/indx\" or \"indx\"
            
            static void Enter(OutputStream &fp, const String &graphName); //!< start  a directed graph
            static void Enter(OutputStream &fp, const char   *graphName); //!< start  a directed graph
            static void Leave(OutputStream &);                            //!< finish a directed graph
            
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
            
            
            //! save and render any CLASS with a graphViz(OutputStream &) methods
            template <typename CLASS>
            static inline void DotToPng(const String &dotFile,
                                        CLASS        &vizible,
                                        const bool    keepDot = false)
            {
                {
                    AutoPtr<OutputStream> fp = OpenFile(dotFile);
                    vizible.graphViz(*fp);
                }
                RenderPNG(dotFile,keepDot);
            }
            
            
            //! save and render any CLASS with a graphViz(OutputStream &,param) methods
            template <typename CLASS, typename PARAM>
            static inline void DotToPngEx(const String &dotFile,
                                          CLASS        &vizible,
                                          const PARAM  &paramEx,
                                          const bool    keepDot = false)
            {
                {
                    AutoPtr<OutputStream> fp = OpenFile(dotFile);
                    vizible.graphViz(*fp,paramEx);
                }
                RenderPNG(dotFile,keepDot);
            }
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vizible);
            static OutputStream *OpenFile(const String &dotFile);
            static void          RenderPNG(const String &dotFile, const bool keepDot);
        };
        
    }
}

#endif

