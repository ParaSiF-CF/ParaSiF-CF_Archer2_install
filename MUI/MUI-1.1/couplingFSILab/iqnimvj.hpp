/*---------------------------------------------------------------------------*\
                          License Place Holder
-------------------------------------------------------------------------------

Filename
    iqnimvj.hpp

Created
    30 July 2019

Class
    muiCouplingIQNIMVJ

Description
    Interface Quasi-Newton
    with Inverse Multiple Vector Jacobian (IQN-IMVJ) Coupling Method
    FSI Coupling utility for the MUI library

Author
    W.L.

SourceFiles
    iqnimvj.cpp

\*---------------------------------------------------------------------------*/
#ifndef MUICOUPLINGIQNIMVJ_HPP
#define MUICOUPLINGIQNIMVJ_HPP

#include "mui.h"

#include <string>
#include <vector>
#include <Eigen/Dense>
#include <map>
#include <assert.h>
#include <iostream>

#define EIGEN_MPL2_ONLY

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace muiCoupling
{

using namespace std;
using namespace mui;
using namespace Eigen;

/*---------------------------------------------------------------------------*\
                 Class muiCouplingIQNIMVJ Declaration
\*---------------------------------------------------------------------------*/

class muiCouplingIQNIMVJ
{

private:

    // Private data

        //-MPI Common World
        MPI_Comm world_;

        //-MPI Common Rank
        int CppRank_;

        //-MPI Common Size
        int CppSize_;

        //- Under relaxation factor of the coupling at present iteration
        double undRelxCpl_;

        //- Maximum under relaxation factor of the coupling
        double undRelxCplMax_;

        //- Under relaxation factor of the coupling at previous iteration
        mutable double undRelxCplPrev_;

        const double initundRelxCpl_;

        int pointSize_;

        int aitkenIterationN_;

        std::vector<double> residualCoupling_;

        std::vector<double> residualDelta_;

        std::vector<double> residualCouplingPrev_;

        std::vector<double> fetchDispCoupling_;

        std::vector<double> deltaDisp_;

        std::vector<double> residualMagSq_;

        double residualMagSqSum_;

        double residualL2Norm_;

        double residualL2NormMax_;

        mutable map <int, int> pointMap_;

        map <int, int> :: iterator it_;

        MatrixXd matrixVk;

        MatrixXd matrixWk;

        MatrixXd matrixVref;

        MatrixXd matrixWref;

        MatrixXd matrixInvJacobian;

        MatrixXd matrixInvJacobianPrev;

    // Private Member Functions

      // Math tools
        //- Calculation of point ID
        int calcpointN(int pointV);

        //- Calculation of point ID
        int sign(double value)
        {
            return (value < 0) ? -1 : ((value > 0) ? 1 : 0);
        }

        //- Store the fetched displacement at present iteration
        void storeFetchDisp(    double fetchMUIx, 
                                double fetchMUIy, 
                                double fetchMUIz,
                                int pointN);

        //- Calculation of residual at present iteration
        void calcResidual(  double disPreX,
                            double disPreY,
                            double disPreZ,
                            int pointN);

        //- Calculation of under relaxation factor at previous iteration
        void setUndRelxPrev();

        //- Calculation of under relaxation factor at present iteration
        void calcUndRelx(int iterN);

        //- Calculation of delta displacement by using Aitken method
        void calcDeltaDispAitken();

        //- Calculation of delta residual at this iteration
        void calResidualDelta();

        //- Store residual at this iteration
        void setPrevResidual();

        //- Reset of residualMagSqSum_
        void resetResidualMagSqSum();

        //- Accumulation of residualMagSqSum_
        void accumResidualMagSqSum();

        //- Calculation of residualL2Norm_
        void calResidualL2Norm();

        //- Calculation of residualL2NormMax_
        void calResidualL2NormMax();

        //- Correction of residualL2Norm_ based on the maximum residualL2NormMax_
        void setResidualL2Norm();

        //- Logging
        void logging();

        void reSetRefMatrixes();

        void calMatrixes(int iterN);

        void refMatrixesPushback(int iterN);

        void calInvJacobian();

        void setInvJacobianPrev();

        void calcDeltaDispIQN();

protected:

    // Protected member functions


public:
    // Static data members


    // Constructors

        //- Construct from components - default
        muiCouplingIQNIMVJ
		(
            int pointSize,
            double initUndRelxCpl,
            MPI_Comm *Fworld
		);


    // Selectors


    // Destructor - default

        ~muiCouplingIQNIMVJ
	    (
	
	    );


    // Member Functions
        // Access

            //- Return under relaxation factor of the coupling
            double undRelxCpl()
            {
                return undRelxCpl_;
            }

            //- Return No. of points
            int pointSize() const
            {
                return pointSize_;
            }

            //- Return x axis component of the delta displacement
            double getXDeltaDisp(int pointV);


            //- Return y axis component of the delta displacement
            double getYDeltaDisp(int pointV);

            //- Return z axis component of the delta displacement
            double getZDeltaDisp(int pointV);

          // Edit

            //- Initialize coupling method
            void initialize();

            //- Collection of Residual at this iteration
            void collectResidual(   double fetchMUIx,
                                    double fetchMUIy,
                                    double fetchMUIz,
                                    double disPreX,
                                    double disPreY,
                                    double disPreZ,
                                    int pointV);

            //- Collection of coupling process at this iteration
            void process(int iterN);


};


// Global Functions


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace muiCoupling

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // End MUICOUPLINGIQNIMVJ_HPP

// ************************************************************************* //