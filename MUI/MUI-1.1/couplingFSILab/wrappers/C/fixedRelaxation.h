/*---------------------------------------------------------------------------*\
                          License Place Holder
-------------------------------------------------------------------------------

Filename 
    fixedRelaxation.h

Created
    29 May 2019

Class
    muiCouplingFixedRelaxation

Description
    C wrapper of Fixed Relaxation Coupling Method
    FSI Coupling utility for the MUI library

Author
    W.L.

SourceFiles
    fixedRelaxation_CAPI.cpp

\*---------------------------------------------------------------------------*/
#ifndef MUICOUPLINGFIXEDRELAXATIONCAPI_H
#define MUICOUPLINGFIXEDRELAXATIONCAPI_H

#include "mui_3d.h"
#include <stdarg.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/* namespace muiCoupling
{ */


/*---------------------------------------------------------------------------*\
                 Class muiCouplingFixedRelaxation Declaration
\*---------------------------------------------------------------------------*/

//- Compiler judgment: C++ / C
#ifdef __cplusplus

extern "C" 
{

    //- With the C++ Compiler   
    class muiCouplingFixedRelaxation;

    typedef muiCouplingFixedRelaxation CAPI_muiCouplingFixedRelaxation;

#else

    //- With the C Compiler, an opaque pointer is used
    typedef struct CAPI_muiCouplingFixedRelaxation CAPI_muiCouplingFixedRelaxation;

#endif // __cplusplus


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Constructor
CAPI_muiCouplingFixedRelaxation* create_muiCouplingFixedRelaxation
(
    int Narg, 
    ...
);

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

void delete_muiCouplingFixedRelaxation
(
    CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation
);


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- The const qualificators maps from the member function to pointers to the class instances.

    //- Return under relaxation factor of the coupling
    double muiCouplingFixedRelaxation_undRelxCpl(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);

    //- Return x axis component of the delta displacement
    double muiCouplingFixedRelaxation_getXDeltaDisp(    CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation,
                                                        int pointN);

    //- Return y axis component of the delta displacement
    double muiCouplingFixedRelaxation_getYDeltaDisp(    CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation,
                                                        int pointN);

    //- Return z axis component of the delta displacement
    double muiCouplingFixedRelaxation_getZDeltaDisp(    CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation,
                                                        int pointN);

    //- Return No. of points
    int muiCouplingFixedRelaxation_pointSize(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);

    //- Return square sum of the residual
    double muiCouplingFixedRelaxation_residualMagSqSum(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);

    //- Return maximum value of the residual L-2 Norm
    double muiCouplingFixedRelaxation_residualL2NormMax(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);

    //- Return the value of the residual L-2 Norm
    double muiCouplingFixedRelaxation_residualL2Norm(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);

    // Edit

    //- Initialize coupling method
    void muiCouplingFixedRelaxation_initialize(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation,
                                                int Narg,
                                                ...);

    //- Collection of coupling process at this iteration
    void muiCouplingFixedRelaxation_collectResidual(    CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation, 
                                                        double fetchMUIx,
                                                        double fetchMUIy,
                                                        double fetchMUIz,
                                                        double disPreX,
                                                        double disPreY,
                                                        double disPreZ,
                                                        int pointN);
                            
    //- Collection of residual calculation at this iteration
    void muiCouplingFixedRelaxation_process(CAPI_muiCouplingFixedRelaxation* muiCouplingFixedRelaxation);


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Compiler judgment: C++ / C
#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

/* } */ // End namespace muiCoupling

#endif // End MUICOUPLINGFIXEDRELAXATIONCAPI_H

// ************************************************************************* //