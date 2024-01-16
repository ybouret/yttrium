//! \file
//!
#ifndef Y_FLTK_CURVE_INCLUDED
#define Y_FLTK_CURVE_INCLUDED 1

#include "y/string.hpp"
#include "y/mkl/v2d.hpp"
#include "y/ptr/ark.hpp"
#include "y/counted.hpp"

#include "y/sequence/vector.hpp"
#include "y/associative/hash/set.hpp"

#include <FL/fl_draw.H>

namespace Yttrium
{
    namespace FLTK 
    {
        
        typedef V2D<double>   Point;   //!< alias
        typedef Vector<Point> Points;  //!< alias

        //! named points+color
        class Curve : public Object, public Counted, public Points
        {
        public:
            enum Style
            {
                WithLines,
                WithPoints
            };

            enum PointShape
            {
                Circle,
                Square
            };

            enum PointStyle
            {
                Filled,
                Border
            };

            explicit Curve(const String &id);
            explicit Curve(const char   *id);
            virtual ~Curve() throw();
            
            const String name;
            Fl_Color     color;
            Style        style;
            int          pointSize;
            PointShape   pointShape;
            PointStyle   pointStyle;

            const String &key() const throw();
            typedef ArkPtr<String,Curve>            Pointer;
            typedef HashSet<String,Curve::Pointer>  Set;

            Point getMin() const;
            Point getMax() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curve);
        };
        
        //! a set of named curves
        class Curves : public Curve::Set
        {
        public:
            explicit Curves() throw(); //!< setup
            virtual ~Curves() throw(); //!< desctructor
            
            Curve &       operator[]( const String &id );       //!< existing or create
            const Curve & operator[]( const String &id ) const; //!< must exist
            
            Curve &       operator[]( const char * );           //!< alias: get existing or create
            const Curve & operator[]( const char *) const;      //!< alias: must exist

            Point getMin() const;
            Point getMax() const;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Curves);
        };
        
    }
}

#endif
