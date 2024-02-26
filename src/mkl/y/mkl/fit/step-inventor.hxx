
namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {
            template <>
            StepInventor<real_t>:: StepInventor() :
            curv(),
            hess(),
            step(),
            atry(),
            atmp(),
            lu(),
            pmin( -int(Numeric<real_t>::DIG)-1   ),
            pmax( Numeric<real_t>::MAX_10_EXP-1  ),
            zero(0),
            one(1),
            two(2),
            ten(10),
            tenth(0.1)
            {}

            template <>
            StepInventor<real_t>:: ~StepInventor() noexcept
            {
            }


            template <>
            void StepInventor<real_t>:: prepare(const size_t nvar)
            {
                curv.make(nvar,nvar);
                hess.make(nvar,nvar);
                step.adjust(nvar,zero);
                atry.adjust(nvar,zero);
                atmp.adjust(nvar,zero);
                lu.ensure(nvar);
            }

            template <>
            real_t StepInventor<real_t>:: xFactor(const int param) const
            {
                assert(param>=pmin);
                assert(param<=pmax);
                if(param<=pmin) return one;

                switch( Sign::Of(param) )
                {
                    case __Zero__: break;
                    case Positive: return one + ipower(ten,param);
                    case Negative: return one + ipower(tenth,-param);
                }
                return two;
            }

            template <>
            bool StepInventor<real_t>:: buildCurvature(const Matrix<real_t> &alpha,
                                                       const int             param,
                                                       const Booleans       &used)
            {
                const real_t fac = xFactor(param);
                curv.assign(alpha);
                for(size_t i=curv.rows;i>0;--i)
                {
                    if(used[i])
                        curv[i][i] *= fac;
                }
                hess.assign(curv);
                return lu.build(curv);
            }

            template <>
            bool StepInventor<real_t>:: buildStep(const LeastSquaresCom<real_t>  &lsqf,
                                                  const Readable<real_t>         &aorg,
                                                  const Domain<real_t>           &adom,
                                                  int                            &p,
                                                  const Booleans                 &used,
                                                  XMLog                          &xml)
            {
                assert(p>=pmin);
                assert(p<=pmax);

                //--------------------------------------------------------------
                //
                // initialize and sanity check
                //
                //--------------------------------------------------------------
                const Readable<real_t> &beta  = lsqf.beta;
                const Matrix<real_t>   &alpha = lsqf.curv;
                const size_t            nvar  = beta.size();

                assert(nvar == curv.rows);
                assert(nvar == hess.rows);
                assert(nvar == step.size());
                assert(nvar == atry.size());
                assert(adom.contains(aorg));

                //--------------------------------------------------------------
                //
                // look for invertible matrix
                //
                //--------------------------------------------------------------
            TRIAL:
                Y_XMLOG(xml, "-- buildCurvature(p=" << p << ")");
                while( !buildCurvature(alpha,p,used) )
                {
                    Y_XMLOG(xml,"*** singular curvature");
                    Y_MKL_FIT_DEGRADE(false);
                }

                //--------------------------------------------------------------
                //
                // compute local step
                //
                //--------------------------------------------------------------
                for(size_t i=nvar;i>0;--i) step[i] = beta[i];
                lu.solve(curv,step);

                //--------------------------------------------------------------
                //
                // compute trial position
                //
                //--------------------------------------------------------------
                Tao::Add(atry,aorg,step);
                Y_XMLOG(xml,"step=" << step);
                Y_XMLOG(xml,"atry=" << atry);
                if(!adom.contains(atry))
                {
                    Y_XMLOG(xml,"*** out of domain");
                    Y_MKL_FIT_DEGRADE(false);
                    goto TRIAL;
                }
                

                return true;
            }


        }

    }

}

