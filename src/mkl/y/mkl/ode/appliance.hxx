
template <>
Appliance<real_t>:: ~Appliance() noexcept
{
    
}

template <>
Appliance<real_t>:: Appliance(const Scheme &scheme,
                              const real_t usrEps,
                              const size_t  usrDim) :
Scheme(scheme),
Integrator<real_t>(usrEps,usrDim)
{
}

template <>
Appliance<real_t>:: Appliance(Actuator<real_t> * scheme,
                              const real_t       usrEps,
                              const size_t       usrDim) :
Scheme(scheme),
Integrator<real_t>(usrEps,usrDim)
{
}
