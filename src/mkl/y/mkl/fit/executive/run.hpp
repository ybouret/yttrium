//! \file

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

    //--------------------------------------------------------------------------
    //
    // top level memory
    //
    //--------------------------------------------------------------------------
    solv->prepare(nvar);


    //--------------------------------------------------------------------------
    //
    // initialize cycles
    //--------------------------------------------------------------------------
    ABSCISSA      D2org = D2(F,S,aorg,used,G); // full metrics
    unsigned long cycle = 0;
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
        Y_XMLOG(xml,"# *** no possible step");
        return Failure;
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
        Y_XMLOG(xml,"# bad step");
        Y_MKL_FIT_DEGRADE(Spurious);
        goto BUILD_STEP;
    }

    Y_XMLOG(xml,"# accepted!");


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

    //--------------------------------------------------------------------------
    // update and check status
    //--------------------------------------------------------------------------
    Tao::Load(aorg,atry);
    if( success )
        return Success;


    //--------------------------------------------------------------------------
    // prepare for next cycle
    //--------------------------------------------------------------------------
    D2org = D2(F,S,aorg,used,G);
    goto CYCLE;
}
