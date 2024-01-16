#include "y/fltk/curve.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace FLTK
    {
        
        Curve:: ~Curve() throw() {}

#define Y_FLTK_CURVE_INIT() \
color(FL_WHITE),            \
style( WithLines ),         \
pointSize(0),               \
pointShape( Circle ),       \
pointStyle( Filled )

        Curve:: Curve( const String &id ) :
        name(id),
        Y_FLTK_CURVE_INIT()
        {
        }
        
        Curve:: Curve( const char *id ) :
        name(id),
        Y_FLTK_CURVE_INIT()
        {
        }
        
        const String & Curve:: key() const throw() { return name; }


        Point Curve:: getMin() const
        {
            const size_t n = size();
            if(n<=0) throw Exception("FLTK::Curve: no points to getMin");

            const Readable<Point> &self = *this;
            Point ans = self[1];
            for(size_t i=n;i>1;--i)
            {
                ans = Point::MinOf(ans,self[i]);
            }
            return ans;

        }

        Point Curve:: getMax() const
        {
            const size_t n = size();
            if(n<=0) throw Exception("FLTK::Curve: no points to getMax");

            const Readable<Point> &self = *this;
            Point ans = self[1];
            for(size_t i=n;i>1;--i)
            {
                ans = Point::MaxOf(ans,self[i]);
            }
            return ans;
        }


        
        Curves:: Curves() throw() : Curve::Set()
        {}
        
        
        Curves:: ~Curves() throw()
        {
        }

        Curve & Curves:: operator[]( const String &id )
        {
            Curve::Pointer *ppC = search(id);
            if( ppC )
            {
                return **ppC;
            }
            else
            {
                Curve::Pointer p( new Curve(id) );
                if( ! insert(p) )
                    throw Exception("Curves.insert('%s') unexpected failure!!!", id.c_str() );
                return *p;
            }
        }

        Curve & Curves:: operator[]( const char *name )
        {
            const String id = name;
            return (*this)[id];
        }
        
        const Curve & Curves:: operator[]( const String &id ) const
        {
            const Curve::Pointer *ppC = search(id);
            if( !ppC )
            {
                throw Exception("no Curves['%s']",id.c_str() );
            }
            return **ppC;
        }
        
        const Curve & Curves:: operator[]( const char *name ) const
        {
            const String id = name;
            return (*this)[id];
        }


        Point Curves:: getMin() const
        {
            if( size() <= 0 )
            {
                throw Exception("FLTK::Curves empty for getMin()");
            }
            ConstIterator  i   = begin();
            Point          ans = (**i).getMin();
            while( ++i != end() )
            {
                ans = Point::MinOf(ans,(**i).getMin());
            }
            return ans;
        }

        Point Curves:: getMax() const
        {
            if( size() <= 0 )
            {
                throw Exception("FLTK::Curves empty for getMax()");
            }

            ConstIterator  i   = begin();
            Point          ans = (**i).getMin();
            while( ++i != end() )
            {
                ans = Point::MaxOf(ans,(**i).getMax());
            }
            return ans;
        }

        
    }
}

