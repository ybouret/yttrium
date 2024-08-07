//! \file

//! running algorithm from starting point
/**

 */
template <
typename FUNCTION,
typename GRADIENT,
typename SAMPLE>
Result run(FUNCTION           &F,
           SAMPLE             &S,
           Writable<ABSCISSA> &aorg,
           const DomainType   &adom,
           const Booleans     &used,
           GRADIENT           &G,
           Writable<ABSCISSA> &aerr,
           XMLog              &xml)
{
    Y_XML_SECTION(xml, "LeastSquaresFit");

    //--------------------------------------------------------------------------
    //
    // sanity check
    //
    //--------------------------------------------------------------------------
    assert( aorg.size() == used.size() ) ;
    assert( aorg.size() == adom.size() );
    assert( aorg.size() == aerr.size() );
    assert( adom.contains(aorg) );


    //--------------------------------------------------------------------------
    //
    // top level initialization
    //
    //--------------------------------------------------------------------------
    Writable<ABSCISSA>       & atry = solv->atry;          // alias
    const Readable<ABSCISSA> & beta = mine->beta;          // alias
    const int                  pmin = solv->pmin;          // alias
    const int                  pmax = solv->pmax;          // alias
    const size_t               nvar = aorg.size();         // num variables
    int                        p    = -4;                  // initial guess
    aerr.ld(0);
    //--------------------------------------------------------------------------
    //
    // top level memory
    //
    //--------------------------------------------------------------------------
    solv->prepare(nvar);


    //--------------------------------------------------------------------------
    //
    // initialize cycles
    //
    //--------------------------------------------------------------------------
    ABSCISSA      D2org  = D2(F,S,aorg,used,G); // full metrics
    unsigned long cycle  = 0;
    Result        result = Failure;

CYCLE:
    const int p0 = p;
    ++cycle;
    Y_XMLOG(xml,"# -------- cycle = " << cycle << " --------");
    Y_XMLOG(xml,"D2org = " << D2org << "# @" << aorg << ", p=" << p << ", used=" << used );
    Y_XMLOG(xml,"beta  = " << beta);
    Y_XMLOG(xml,"curv  = " << mine->curv);


    //--------------------------------------------------------------------------
    //
    // compute predicted step
    //
    //--------------------------------------------------------------------------
BUILD_STEP:
    if(!solv->buildStep(*mine,aorg,adom,p,used,xml))
    {
        Y_XMLOG(xml,"# *** no possible step"); assert(Failure==result);
        return result;
    }

    //--------------------------------------------------------------------------
    //
    // here, we have an approximated step and trial
    //
    //--------------------------------------------------------------------------
    const ABSCISSA D2try = D2(F,S,atry);
    Y_XMLOG(xml,"D2try = " << D2try << "# @" << atry << ", p=" << p);

    if(D2try>D2org)
    {
        //----------------------------------------------------------------------
        Y_XMLOG(xml,"# bad step");
        //----------------------------------------------------------------------
        if(++p>pmax)
        {
            p      = pmax;         // top
            result = Spurious;     // spurious convergence
            D2org  = D2(F,S,aorg); // recompute predicted values
            goto CONVERGED;
        }
        goto BUILD_STEP;
    }
    else
    {
        //----------------------------------------------------------------------
        Y_XMLOG(xml,"# accepted!");
        //----------------------------------------------------------------------

        bool           success = false;
        const bool     kept    = (p==p0);
        if(kept)
        {
            Y_XMLOG(xml,"# upgrade parameter");
            if(--p<=pmin) p = pmin;

            const ABSCISSA delta = D2org-D2try;
            const ABSCISSA limit = dtol * D2org;
            Y_XMLOG(xml,"# delta = " << delta << " / limit=" << limit);
            if( delta <= limit)
            {
                Y_XMLOG(xml,"# success");
                success = true;
            }

        }

        //----------------------------------------------------------------------
        // update and check status
        //----------------------------------------------------------------------
        Tao::Load(aorg,atry);
        if( success )
        {
            result = Success;
            goto CONVERGED;
        }


        //----------------------------------------------------------------------
        // prepare for next cycle
        //----------------------------------------------------------------------
        D2org = D2(F,S,aorg,used,G);
        goto CYCLE;
    }

CONVERGED:
    //--------------------------------------------------------------------------
    //
    // Estimating errors
    //
    //--------------------------------------------------------------------------
    Y_XML_SECTION(xml, "LeastSquaresErrors");

    //--------------------------------------------------------------------------
    // compute covariance
    //--------------------------------------------------------------------------
    if(!solv->covar(mine->curv,xml))
    {
        Y_XMLOG(xml, "*** singular covariance");
        return Failure;
    }
    Y_XMLOG(xml, "D2 = " << D2org);

    //--------------------------------------------------------------------------
    // compute degrees of freedom
    //--------------------------------------------------------------------------
    const size_t ndata = mine->npts;
    size_t       nlive = nvar;
    for(size_t i=nvar;i>0;--i) { if(!used[i]) --nlive; }
    Y_XMLOG(xml, "#data = " << ndata);
    Y_XMLOG(xml, "#live = " << nlive);
    if(ndata<=nlive)
    {
        // ill-conditionned/interpolation
        Y_XMLOG(xml, "-- undefined variance");
        return result;
    }
    const size_t            dof = ndata-nlive;
    const ABSCISSA          den = dof;
    Y_XMLOG(xml, "#dof  = " << dof);

    //--------------------------------------------------------------------------
    // evaluate individual standard deviations
    //--------------------------------------------------------------------------
    const Matrix<ABSCISSA> &covar = solv->curv;
    for(size_t i=1;i<=nvar;++i)
    {
        if(!used[i]) continue;
        const ABSCISSA cii = covar[i][i];
        const ABSCISSA esq = Max(zero,cii)*D2org/den;
        const ABSCISSA err = Sqrt<ABSCISSA>::Of(esq);
        Y_XMLOG(xml, "covar[" << i << "] = " << cii << " => err=" << err);
        aerr[i] = err;
    }
    return result;

}
