
template <>
Derivatives<real_t>:: ~Derivatives() noexcept
{
    assert(0!=code);
    delete code;
    code = 0;
}


template <>
Derivatives<real_t>:: Derivatives()  : Kernel::Derivatives(),
code( new Code() )
{

}

