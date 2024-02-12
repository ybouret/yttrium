//! \file

//______________________________________________________________________________
//
//! compute least squares of S w.r.t a sequential F
/**
 \param F    a sequential function
 \param S    a single sample
 \param aorg parameters
 */
//______________________________________________________________________________
inline ABSCISSA Of(SequentialFunc           &F,
                   SampleType               &S,
                   const Readable<ABSCISSA> &aorg)
{
    //--------------------------------------------------------------------------
    // initialize
    //--------------------------------------------------------------------------
    const size_t     numPoints  = S.numPoints();
    const Abscissae &abscissae  = S.abscissae();
    const Ordinates &ordinates  = S.ordinates();
    Predicted       &predicted  = S.predicted();

    xadd.make(numPoints*Dimension);
    xlst.flush();


    //--------------------------------------------------------------------------
    // first point
    //--------------------------------------------------------------------------
    {
        const size_t   j  = S.indx[1];
        const ORDINATE Fj = predicted[j] = F.set(abscissae[j],aorg,S.vars);
        pushDSQ(ordinates[j],Fj);
    }

    //--------------------------------------------------------------------------
    // following points
    //--------------------------------------------------------------------------
    for(size_t i=2;i<=numPoints;++i)
    {
        const size_t   j  = S.indx[i];
        const ORDINATE Fj = predicted[j] = F.run(abscissae[j],aorg,S.vars);
        pushDSQ(ordinates[j],Fj);
    }

    //--------------------------------------------------------------------------
    // return sum
    //--------------------------------------------------------------------------
    Coerce(npts) = numPoints;
    return ( half * xadd.sum() );
}

