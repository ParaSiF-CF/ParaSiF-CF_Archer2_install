/*---------------------------------------------------------------------------*\
                          License Place Holder
-------------------------------------------------------------------------------

Filename 
    aitken.h

Created
    05 July 2019

Class
    muiCouplingAitken

Description
    C wrapper of Aitken Coupling Method
    FSI Coupling utility for the MUI library

Author
    W.L.

SourceFiles
    aitken_CAPI.cpp

\*---------------------------------------------------------------------------*/
#ifndef MUICOUPLINGAITKENCAPI_H
#define MUICOUPLINGAITKENCAPI_H

#include "mui_3d.h"
#include <stdarg.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/* namespace muiCoupling
{ */


/*---------------------------------------------------------------------------*\
                 Class muiCouplingAitken Declaration
\*---------------------------------------------------------------------------*/

//- Compiler judgment: C++ / C
#ifdef __cplusplus

extern "C" 
{

    //- With the C++ Compiler   
    class muiCouplingAitken;

    typedef muiCouplingAitken CAPI_muiCouplingAitken;

#else

    //- With the C Compiler, an opaque pointer is used
    typedef struct CAPI_muiCouplingAitken CAPI_muiCouplingAitken;

#endif // __cplusplus


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

//                                   [ NONE ]                               //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Constructor - default
CAPI_muiCouplingAitken* create_muiCouplingAitken(int Narg, ...);

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

void delete_muiCouplingAitken
(
    CAPI_muiCouplingAitken* muiCouplingAitken
);


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- The const qualificators maps from the member function to pointers to the class instances.

    //- Return under relaxation factor of the coupling
    double muiCouplingAitken_undRelxCpl(CAPI_muiCouplingAitken* muiCouplingAitken);

    //- Return x axis component of the delta displacement
    double muiCouplingAitken_getXDeltaDisp(    CAPI_muiCouplingAitken* muiCouplingAitken,
                                                        int pointv);

    //- Return y axis component of the delta displacement
    double muiCouplingAitken_getYDeltaDisp(    CAPI_muiCouplingAitken* muiCouplingAitken,
                                                        int pointv);

    //- Return z axis component of the delta displacement
    double muiCouplingAitken_getZDeltaDisp(    CAPI_muiCouplingAitken* muiCouplingAitken,
                                                        int pointv);

    //- Return No. of points
    int muiCouplingAitken_pointSize(CAPI_muiCouplingAitken* muiCouplingAitken);

    //- Return square sum of the residual
    double muiCouplingAitken_residualMagSqSum(CAPI_muiCouplingAitken* muiCouplingAitken);

    //- Return maximum value of the residual L-2 Norm
    double muiCouplingAitken_residualL2NormMax(CAPI_muiCouplingAitken* muiCouplingAitken);

    //- Return the value of the residual L-2 Norm
    double muiCouplingAitken_residualL2Norm(CAPI_muiCouplingAitken* muiCouplingAitken);

    // Edit

    //- Initialize coupling method
    void muiCouplingAitken_initialize(CAPI_muiCouplingAitken* muiCouplingAitken,
                                      int Narg,
                                      ...);

    //- Collection of coupling process at this iteration
    void muiCouplingAitken_collectResidual( CAPI_muiCouplingAitken* muiCouplingAitken, 
                                            double fetchMUIx,
                                            double fetchMUIy,
                                            double fetchMUIz,
                                            double disPreX,
                                            double disPreY,
                                            double disPreZ,
                                            int pointv);
                            
    //- Collection of residual calculation at this iteration
    void muiCouplingAitken_process( CAPI_muiCouplingAitken* muiCouplingAitken,
                                    int iterN,
                                    int curIterN);


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Compiler judgment: C++ / C
#ifdef __cplusplus

} // extern "C"

#endif // __cplusplus

/* } */ // End namespace muiCoupling

#endif // End MUICOUPLINGAITKENCAPI_H

// ************************************************************************* //