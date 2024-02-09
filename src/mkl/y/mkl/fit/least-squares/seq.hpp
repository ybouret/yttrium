//______________________________________________________________
//
//! compute least squares of S w.r.t a sequential F
/**
 \param F    a sequential function
 \param S    a single sample
 \param aorg parameters
 \param vars variables
 */
//______________________________________________________________
inline ABSCISSA Of(SequentialFunc           &F,
                   const SampleType         &S,
                   const Readable<ABSCISSA> &aorg,
                   const Variables          &vars)
{
    //----------------------------------------------------------
    // initialize
    //----------------------------------------------------------
    const size_t     np = S.numPoints();
    const Abscissae &a  = S.abscissae();
    const Ordinates &b  = S.ordinates();
    xadd.make(np*Dimension);
    xlst.flush();


    //----------------------------------------------------------
    // first point
    //----------------------------------------------------------
    {
        const size_t   j  = S.indx[1];
        const ORDINATE Fj = F.set(a[j],aorg,vars);
        pushDSQ(b[j],Fj);
    }

    //----------------------------------------------------------
    // following points
    //----------------------------------------------------------
    for(size_t i=2;i<=np;++i)
    {
        const size_t   j  = S.indx[i];
        const ORDINATE Fj = F.run(a[j],aorg,vars);
        pushDSQ(b[j],Fj);
    }

    //----------------------------------------------------------
    // return sum
    //----------------------------------------------------------
    Coerce(npts) = np;
    return ( half * xadd.sum() );
}

