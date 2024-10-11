
{
    Y_XML_SECTION(xml, "ODE");
    computeRate(dC);
    xreal_t scale;
    if( stepWasCut(Cend,Cini,dC, &scale) )
    {
        Y_XML_COMMENT(xml, "scaled by " << real_t(scale) );
    }
    else
    {
        Y_XML_COMMENT(xml, "full step");
    }
    saveProfile("ode.pro",1000);

    const xreal_t slope = aftermath.xadd.dot(gradient,dC);
    if(slope.mantissa<0.0)
    {
        Y_XML_COMMENT(xml,"negative ODE slope");
        const xreal_t    ode = lookUp();
        Y_XMLOG(xml, "|ode=" << Formatted::Get("%15.4g",real_t(ode)) << "|");
        if(ode<ff1)
        {
            Y_XML_COMMENT(xml,"upgrade ODE result");
            ff1 = ode;
            Copt.ld(Ctmp);
            if(ff1.mantissa<=0)
            {
                Y_XML_COMMENT(xml, "solving by ODE step");
                mine.transfer(C,L,Copt,SubLevel);
                Y_DEVICE_RETURN(Solved);
            }
        }
        else
        {
            Y_XML_COMMENT(xml,"discard ODE result");
        }
    }
    else
    {
        Y_XML_COMMENT(xml,"positive ODE slope");
    }

}
assert(ff1.mantissa>0.0);
