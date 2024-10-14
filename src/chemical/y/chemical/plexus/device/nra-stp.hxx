{
    Y_XML_SECTION(xml, "NRA");
    const size_t n = basis.size;
    const size_t m = nspc;
    //--------------------------------------------------------------
    //
    //
    // initialize Phi and Nu
    //
    //
    //--------------------------------------------------------------
    XMatrix &Phi = xPhi[n]; assert(Phi.rows  == n);
    XMatrix &Nu  = xNu[n];  assert(Nu.rows   == n);
    XMatrix &Chi = xChi[n]; assert(Chi.rows  == n);
    XArray  &xi  = xXi[n];  assert(xi.size() == n);

    {
        const ANode *an = basis.head;
        for(size_t i=1;i<=n;++i,an=an->next)
        {
            assert(0!=an);
            const Ansatz      &ans = **an;
            const Equilibrium &eq  = ans.eq;
            XWritable         &phi = Phi[i];
            XWritable         &nu  = Nu[i];
            eq.topology(nu, SubLevel);
            eq.drvsAffinity(phi, SubLevel, Cini, SubLevel);
            (xi[i] = eq.affinity(ans.ek, aftermath.xmul, Cini, SubLevel)).neg();
        }
    }

    //--------------------------------------------------------------
    //
    //
    // compute Phi*Nu'
    //
    //
    //--------------------------------------------------------------
    XAdd   &xadd = aftermath.xadd;
    for(size_t i=n;i>0;--i)
    {
        for(size_t j=n;j>0;--j)
        {
            xadd.free();
            for(size_t k=m;k>0;--k)
            {
                xadd << Phi[i][k] * Nu[j][k];
            }
            Chi[i][j] = xadd.sum();
        }
    }


    //--------------------------------------------------------------
    //
    //
    // compute inv(Phi*Nu')
    //
    //
    //--------------------------------------------------------------
    if(xlu.build(Chi))
    {
        //----------------------------------------------------------
        //
        // compute NRA dC
        //
        //----------------------------------------------------------
        xlu.solve(Chi,xi);
        for(size_t j=m;j>0;--j)
        {
            xadd.free();
            for(size_t k=n;k>0;--k)
            {
                xadd << Nu[k][j] * xi[k];
            }
            dC[j] = xadd.sum();
        }

        //----------------------------------------------------------
        //
        // cut
        //
        //----------------------------------------------------------
        xreal_t scale;
        if( stepWasCut(Cend,Cini,dC, &scale) )
        {
            Y_XML_COMMENT(xml, "scaled by " << real_t(scale) );
        }
        else
        {
            Y_XML_COMMENT(xml, "full step");
        }

        if(trace)
        {
            saveProfile("nra.pro",1000);
        }

        const xreal_t slope = aftermath.xadd.dot(gradient,dC);
        if(slope.mantissa<0.0)
        {
            Y_XML_COMMENT(xml,"negative NRA slope");
            const xreal_t    nra = lookUp();
            Y_XMLOG(xml, "|nra=" << Formatted::Get("%15.4g",real_t(nra)) << "|");
            if(nra<ff1)
            {
                Y_XML_COMMENT(xml,"upgrade NRA result");
                ff1 = nra;
                Copt.ld(Ctmp);
                if(ff1.mantissa<=0)
                {
                    Y_XML_COMMENT(xml,"solving by NRA step");
                    mine.transfer(C,L,Copt,SubLevel);
                    Y_DEVICE_RETURN(Solved);
                }
            }
            else
            {
                Y_XML_COMMENT(xml,"discard NRA result");
            }
        }
        else
        {
            Y_XML_COMMENT(xml,"positive NRA slope");
        }

    }
    else
    {
        Y_XML_COMMENT(xml, "singular matrix");
    }
}
assert(ff1.mantissa>0.0);

