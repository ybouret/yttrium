//! \file

//______________________________________________________________________________
//
//! compute least squares of S w.r.t an out of order F
/**
 \param F    an out-of-order function
 \param S    a single sample
 \param aorg parameters
 */
//______________________________________________________________________________
inline ABSCISSA Of(OutOfOrderFunc           &F,
                   SampleType               &S,
                   const Readable<ABSCISSA> &aorg)
{
    //--------------------------------------------------------------------------
    // initialize
    //--------------------------------------------------------------------------
    const size_t        numPoints = S.numPoints();
    const Abscissae    &abscissae = S.abscissae();
    const Ordinates    &ordinates = S.ordinates();
    Predicted          &predicted = S.predicted();
    xadd.make(numPoints*Dimension);
    xlst.flush();

    //--------------------------------------------------------------------------
    // compute
    //--------------------------------------------------------------------------
    for(size_t j=numPoints;j>0;--j)
    {
        const ORDINATE Fj = predicted[j] = F(abscissae[j],aorg,S.vars);
        pushDSQ(ordinates[j],Fj);
    }

    //--------------------------------------------------------------------------
    // return sum
    //--------------------------------------------------------------------------
    Coerce(npts) = numPoints;
    return (  half * xadd.sum() );
}

private:
inline void regularizeWith(const SampleType         &S,
                           const Booleans           &used)
{
    for(size_t i=used.size();i>0;--i)
    {
        if( S.vars.found(i) && used[i] )
        {
            for(size_t j=i-1;j>0;--j)
                curv[j][i] = curv[i][j];
            continue;
        }
        curv[i][i] = one;
    }
}

public:
//______________________________________________________________________________
//
//! compute full least squares of S w.r.t an out-of-order F
/**
 \param F    an out of order function
 \param S    a single sample
 \param aorg parameters
 \param used used parameters/variables
 \param G   an out of order gradient
 \param flag regularize if true
 */
//______________________________________________________________________________
inline ABSCISSA Of(OutOfOrderFunc           &F,
                   SampleType               &S,
                   const Readable<ABSCISSA> &aorg,
                   const Booleans           &used,
                   OutOfOrderGrad           &G,
                   const bool                flag = true)

{
    //----------------------------------------------------------
    // initialize
    //----------------------------------------------------------
    const size_t     numPoints = S.numPoints();
    const Abscissae &abscissae = S.abscissae();
    const Ordinates &ordinates = S.ordinates();
    Predicted       &predicted = S.predicted();
    const size_t     nv = aorg.size();
    const size_t     nc = numPoints*Dimension;
    assert(used.size() == nv);


    xadd.make(nc);           assert(xadd.accepts(nc));
    xlst.setup(nv,nc);       assert(xlst.size==nv);
    dFda.adjust(nv,zord);
    beta.adjust(nv,zero);
    curv.make(nv,nv);

    //--------------------------------------------------------------------------
    // global initialization
    //--------------------------------------------------------------------------
    beta.ld(zero);
    curv.ld(zero);



    //--------------------------------------------------------------------------
    // accumulation over points
    //--------------------------------------------------------------------------
    for(size_t j=numPoints;j>0;--j)
    {
        const ORDINATE Fj = predicted[j] = F(abscissae[j],aorg,S.vars);
        dFda.ld(zord); // local setting before call
        G(dFda,abscissae[j],aorg,S.vars,used);
        pushAll(ordinates[j],Fj,used);
    }
    xlst.sum(beta);


    //--------------------------------------------------------------------------
    // optional regularization
    //--------------------------------------------------------------------------
    if(flag) regularizeWith(S,used);

    //--------------------------------------------------------------------------
    // return sum
    //--------------------------------------------------------------------------
    Coerce(npts) = numPoints;
    return ( half * xadd.sum() );
}
