/*---------------------------------------------------------------------------*\
                          License Place Holder
-------------------------------------------------------------------------------

Filename 
    iqnils.h

Created
    10 July 2019

Class
    muiCouplingIQNILS

Description
    C wrapper of Interface Quasi-Newton
    with Inverse Jacobian from Least Squares model (IQN-ILS) Coupling Method
    FSI Coupling utility for the MUI library

Author
    W.L.

SourceFiles
    iqnils_CAPI.cpp

\*---------------------------------------------------------------------------*/
#ifndef MUICOUPLINGIQNILSCAPI_H
#define MUICOUPLINGIQNILSCAPI_H

#include "mui_3d.h"
#include <stdarg.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/* namespace muiCoupling
{ */


/*---------------------------------------------------------------------------*\
                 Class muiCouplingIQNILS Declaration
\*---------------------------------------------------------------------------*/

//- Compiler judgment: C++ / C
#ifdef __cplusplus

extern "C" 
{

    //- With the C++ Compiler   
    class muiCouplingIQNILS;

    typedef muiCouplingIQNILS CAPI_muiCouplingIQNILS;

#else

    //- With the C Compiler, an opaque pointer is used
    typedef struct CAPI_muiCouplingIQNILS CAPI_muiCouplingIQNILS;

#endif // __cplusplus


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

//                                   [ NONE ]                               //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Constructor - default
CAPI_muiCouplingIQNILS* create_muiCouplingIQNILS(int Narg, ...);

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

void delete_muiCouplingIQNILS
(
    CAPI_muiCouplingIQNILS* muiCouplingIQNILS
);


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- The const qualificators maps from the member function to pointers to the class instances.

    //- Return under relaxation factor of the coupling
    double muiCouplingIQNILS_undRelxCpl(CAPI_muiCouplingIQNILS* muiCouplingIQNILS);

    //- Return x axis component of the delta displacement
    double muiCouplingIQNILS_getXDeltaDisp(    CAPI_muiCouplingIQNILS* muiCouplingIQNILS,
                                                        int pointv);

    //- Return y axis component of the delta displacement
    double muiCouplingIQNILS_getYDeltaDisp(    CAPI_muiCouplingIQNILS* muiCouplingIQNILS,
                                                        int pointv);

    //- Return z axis component of the delta displacement
    double muiCouplingIQNILS_getZDeltaDisp(    CAPI_muiCouplingIQNILS* muiCouplingIQNILS,
                                                        int pointv);

    //- Return No. of points
    int muiCouplingIQNILS_pointSize(CAPI_muiCouplingIQNILS* muiCouplingIQNILS);

    //- Return square sum of the residual
    double muiCouplingIQNILS_residualMagSqSum(CAPI_muiCouplingIQNILS* muiCouplingIQNILS);

    //- Return maximum value of the residual L-2 Norm
    double muiCouplingIQNILS_residualL2NormMax(CAPI_muiCouplingIQNILS* muiCouplingIQNILS);

    //- Return the value of the residual L-2 Norm
    double muiCouplingIQNILS_residualL2Norm(CAPI_muiCouplingIQNILS* muiCouplingIQNILS);

    // Edit

    //- Initialize coupling method
    void muiCouplingIQNILS_initialize(CAPI_muiCouplingIQNILS* muiCouplingIQNILS,
                                      int Narg,
                                      ...);

    //- Collection of coupling process at this iteration
    void muiCouplingIQNILS_collectResidual( CAPI_muiCouplingIQNILS* muiCouplingIQNILS, 
                                            double fetchMUIx,
                                            double fetchMUIy,
                                            double fetchMUIz,
                                            double disPreX,
                                            double disPreY,
                                            double disPreZ,
                                            int pointv);
                            
    //- Collection of residual calculation at this iteration
    void muiCouplingIQNILS_process( CAPI_muiCouplingIQNILS* muiCouplingIQNILS,
                                    int iterN);


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Compiler judgment: C++ / C
#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

/* } */ // End namespace muiCoupling

#endif // End MUICOUPLINGIQNILSCAPI_H

// ************************************************************************* //