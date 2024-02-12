
//______________________________________________________________
//
//! compute least squares of S w.r.t an out of order F
/**
 \param F    an out-of-order function
 \param S    a single sample
 \param aorg parameters
 \param vars variables
 */
//______________________________________________________________
inline ABSCISSA Of(OutOfOrderFunc           &F,
                   const SampleType         &S,
                   const Readable<ABSCISSA> &aorg)
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
    // compute
    //----------------------------------------------------------
    for(size_t j=np;j>0;--j)
    {
        const ORDINATE Fj = F(a[j],aorg,S.vars);
        pushDSQ(b[j],Fj);
    }

    //----------------------------------------------------------
    // return sum
    //----------------------------------------------------------
    Coerce(npts) = np;
    return (  half * xadd.sum() );
}

//______________________________________________________________
//
//! compute full least squares of S w.r.t an out-of-order F
/**
 \param F    an out of order function
 \param S    a single sample
 \param aorg parametes
 \param vars variables
 \param used used parameters/variables
 \param G   an out of order gradient
 */
//______________________________________________________________
inline ABSCISSA Of(OutOfOrderFunc           &F,
                   const SampleType         &S,
                   const Readable<ABSCISSA> &aorg,
                   const Booleans           &used,
                   OutOfOrderGrad           &G)

{
    //----------------------------------------------------------
    // initialize
    //----------------------------------------------------------
    const size_t     np = S.numPoints();
    const Abscissae &a  = S.abscissae();
    const Ordinates &b  = S.ordinates();
    const size_t     nv = aorg.size();
    const size_t     nc = nv*Dimension;
    assert(used.size() == nv);


    xadd.make(nc);           assert(xadd.accepts(nc));
    xlst.setup(nv,nc);       assert(xlst.size==nv);
    dFda.adjust(nv,zord);
    beta.adjust(nv,zero);
    curv.make(nv,nv);

    //----------------------------------------------------------
    // global initialization
    //----------------------------------------------------------
    beta.ld(zero);
    curv.ld(zero);



    //----------------------------------------------------------
    // accumulation over points
    //----------------------------------------------------------
    for(size_t j=np;j>0;--j)
    {
        const ORDINATE Fj = F(a[j],aorg,S.vars);
        dFda.ld(zord); // local setting before call
        G(dFda,a[j],aorg,S.vars,used);
        pushAll(b[j],Fj,used);
    }
    xlst.sum(beta);


    //----------------------------------------------------------
    // ensure consistency
    //----------------------------------------------------------
    for(size_t i=nv;i>0;--i)
    {
        if( S.vars.found(i) && used[i] )
        {
            for(size_t j=i-1;j>0;--j)
                curv[j][i] = curv[i][j];
            continue;
        }

        curv[i][i] = one;
    }

    //----------------------------------------------------------
    // return sum
    //----------------------------------------------------------
    Coerce(npts) = np;
    return ( half * xadd.sum() );
}
