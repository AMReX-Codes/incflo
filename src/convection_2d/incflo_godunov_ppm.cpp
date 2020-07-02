#include <incflo_godunov_ppm.H>
#include <Godunov.H>

using namespace amrex;

void godunov::predict_ppm (int lev, Box const& bx, int ncomp,
                           Array4<Real> const& Imx,
                           Array4<Real> const& Ipx,
                           Array4<Real> const& Imy,
                           Array4<Real> const& Ipy,
                           Array4<Real const> const& q,
                           Array4<Real const> const& vel,
                           Vector<Geometry> geom,
                           Real dt,
                           BCRec const* pbc)
{
    const Box& domain = geom[lev].Domain();
    const Dim3 dlo = amrex::lbound(domain);
    const Dim3 dhi = amrex::ubound(domain);

    const auto dx = geom[lev].CellSizeArray();
    Real l_dtdx = dt / dx[0];
    Real l_dtdy = dt / dx[1];
    Real l_dtdz = 0.0;//dt / dx[2];

    amrex::ParallelFor(bx, AMREX_SPACEDIM, 
    [=] AMREX_GPU_DEVICE (int i, int j, int k, int n) noexcept
    {
        Godunov_ppm_pred_x(i,j,k,n,l_dtdx,vel(i,j,k,0),q,Imx,Ipx,pbc[n],dlo.x,dhi.x);
        Godunov_ppm_pred_y(i,j,k,n,l_dtdy,vel(i,j,k,1),q,Imy,Ipy,pbc[n],dlo.y,dhi.y);
        //Godunov_ppm_pred_z(i,j,k,n,l_dtdz,vel(i,j,k,2),q,Imz,Ipz,pbc[n],dlo.z,dhi.z);
    });
}
