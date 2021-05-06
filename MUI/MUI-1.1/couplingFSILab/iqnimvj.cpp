/*---------------------------------------------------------------------------*\
                          License Place Holder
-------------------------------------------------------------------------------

Filename
    iqnimvj.cpp

Created
    30 July 2019

Class
    muiCouplingIQNIMVJ

Description
    Interface Quasi-Newton 
    with Inverse Multiple Vector Jacobian (IQN-IMVJ) Coupling Method
    FSI Coupling utility for the MUI library
    THIS CLASS IS NOT READY TO USE YET
Author
    W.L.

\*---------------------------------------------------------------------------*/

#include "iqnimvj.hpp"

#include "mui.h"

#include <iostream>
#include <math.h>
#include <algorithm>
#include <map>
#include <numeric>
#include <iterator>
#include <stdlib.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace muiCoupling
{

using namespace std;
using namespace mui;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

int muiCouplingIQNIMVJ::calcpointN(int pointV)
{

    it_ = pointMap_.find(pointV);

    if(it_ != pointMap_.end())
    {
        // cout << (*it_).first << " : " << ( *it_ ).second << endl ;
        return (( *it_ ).second);
    }
    else
    {
        pointMap_.insert(std::pair<int, int>(pointV, (pointMap_.size()+1)));
        return (pointMap_.size());
    }

}

void muiCouplingIQNIMVJ::calcResidual(   double disPreX,
                                        double disPreY,
                                        double disPreZ,
                                        int pointN)
{

    residualCoupling_[(3*(pointN-1))+0] = fetchDispCoupling_[(3*(pointN-1))+0] - disPreX;

    residualCoupling_[(3*(pointN-1))+1] = fetchDispCoupling_[(3*(pointN-1))+1] - disPreY;

    residualCoupling_[(3*(pointN-1))+2] = fetchDispCoupling_[(3*(pointN-1))+2] - disPreZ;

    //cout << "residualCoupling_: " << residualCoupling_[(3*(pointN-1))+0] << endl;

}

void muiCouplingIQNIMVJ::setUndRelxPrev()
{

    if (CppRank_ == 0)
    {
        undRelxCplPrev_ = undRelxCpl_;
    }

    MPI_Bcast(&undRelxCplPrev_, 1, MPI::DOUBLE, 0, world_);

}

void muiCouplingIQNIMVJ::calcUndRelx(int iterN)
{

    assert(iterN >= 0);

    if (iterN < 2)
    {
        undRelxCpl_ = initundRelxCpl_;
    }
    else
    {
        calResidualDelta();

        double localNominator    = 0.0;
        double localDenominator  = 0.0;
        double nominator    = 0.0; 
        double denominator  = 0.0;

        localNominator    = inner_product(begin(residualCouplingPrev_), end(residualCouplingPrev_), begin(residualDelta_), 0.0);
        localDenominator  = inner_product(begin(residualDelta_), end(residualDelta_), begin(residualDelta_), 0.0); 

        MPI_Allreduce(&localNominator, &nominator, 1, MPI::DOUBLE, MPI::SUM, world_);
        MPI_Allreduce(&localDenominator, &denominator, 1, MPI::DOUBLE, MPI::SUM, world_);

        cout  << "iteration: " << iterN << " Rank: " << CppRank_ << " nominator: " << nominator << " denominator: " << denominator << " undRelxCplPrev_: " << undRelxCplPrev_ << endl;

        if (CppRank_ == 0)
        {
            if (denominator != 0.0 )
            {
                if (undRelxCplPrev_ != 0.0)
                {
                    undRelxCpl_ = -undRelxCplPrev_ * (nominator/denominator);
                }
                else
                {
                    undRelxCpl_ = -initundRelxCpl_ * (nominator/denominator);
                }

                // Constraint of the Aitken under relaxation factor
                undRelxCpl_ = sign(undRelxCpl_) * min(abs(undRelxCpl_),undRelxCplMax_);

            }
            else
            {
                assert(nominator == 0);

                undRelxCpl_ = 0.0;
            }

            assert(!isnan( undRelxCpl_));

            cout << "Aitken under relaxation factor is: " << undRelxCpl_ << endl;

        }

        MPI_Bcast(&undRelxCpl_, 1, MPI::DOUBLE, 0, world_);

        cout << "undRelxCpl_: " << undRelxCpl_ << " rank: " << CppRank_ << endl;

    }


}

void muiCouplingIQNIMVJ::calcDeltaDispAitken()
{

    for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {
        deltaDisp_[pointN] = undRelxCpl_ * residualCoupling_[pointN];
    }

}

void muiCouplingIQNIMVJ::calResidualDelta()
{

    for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {
        residualDelta_[pointN] = residualCoupling_[pointN] - residualCouplingPrev_[pointN];

        // cout << "pointN: " << pointN + 1 << endl;
        // cout << "residualCoupling_: " << residualCoupling_[pointN] << endl;
        // cout << "residualCouplingPrev_: " << residualCouplingPrev_[pointN] << endl;
        // cout << "residualDelta_: " << residualDelta_[pointN] << endl;
    }

}

void muiCouplingIQNIMVJ::setPrevResidual()
{

    for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {
        residualCouplingPrev_[pointN] = residualCoupling_[pointN];
        // cout << "setPrevResidual pointN: " << pointN + 1 << endl;
        // cout << "setPrevResidual residualCouplingPrev_: " << residualCouplingPrev_[pointN] << endl;
    }

}


void muiCouplingIQNIMVJ::resetResidualMagSqSum()
{

    residualMagSqSum_ = 0.0;

}

void muiCouplingIQNIMVJ::accumResidualMagSqSum()
{

    double localResidualMagSqSum_ = 0.0;

    for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {
        localResidualMagSqSum_ += pow(residualCoupling_[pointN], 2);
    }

    cout << "{MUI_Coupling} From rank " << CppRank_ << " localResidualMagSqSum_: " << localResidualMagSqSum_ << endl;


    MPI_Allreduce(&localResidualMagSqSum_, &residualMagSqSum_, 1, MPI::DOUBLE, MPI::SUM, world_);

/*     for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {
        residualMagSqSum_ +=    pow(residualCoupling_[pointN], 2);
    } */

}

void muiCouplingIQNIMVJ::calResidualL2Norm()
{

    if (CppRank_ == 0)
    {
        residualL2Norm_ = sqrt(residualMagSqSum_);
    }

    MPI_Bcast(&residualL2Norm_, 1, MPI::DOUBLE, 0, world_);

    cout << "residualMagSqSum_: " << residualMagSqSum_ << " residualL2Norm_: " << residualL2Norm_ << " rank: " << CppRank_ << endl;

}

void muiCouplingIQNIMVJ::calResidualL2NormMax()
{

    if (CppRank_ == 0)
    {
        residualL2NormMax_ = max(residualL2Norm_, residualL2NormMax_);
    }

    MPI_Bcast(&residualL2NormMax_, 1, MPI::DOUBLE, 0, world_);

    cout << "residualL2NormMax_: " << residualL2NormMax_ << " residualL2Norm_: " << residualL2Norm_ << " rank: " << CppRank_ << endl;

}

void muiCouplingIQNIMVJ::setResidualL2Norm()
{

    if (CppRank_ == 0)
    {
        if (residualL2NormMax_ <= 0.0 )
        {
            residualL2Norm_ = 1.0;
        }
        else
        {
            residualL2Norm_ /= residualL2NormMax_;
        }
    }

    MPI_Bcast(&residualL2Norm_, 1, MPI::DOUBLE, 0, world_);

}


void muiCouplingIQNIMVJ::logging()
{

    if (CppRank_ == 0)
    {
        std::cout << std::scientific;
        cout << endl;
        cout << "{MUI_Coupling} No. of points is: " << pointSize() << endl;
        cout << "{MUI_Coupling} Under-relaxation factor is: " << undRelxCpl() << endl;
        cout << "{MUI_Coupling} Size of residualCoupling_ is: " << residualCoupling_.size() << endl;
        cout << "{MUI_Coupling} Size of residualCouplingPrev_ is: " << residualCouplingPrev_.size() << endl;
        cout << "{MUI_Coupling} Size of residualMagSq_ is: " << residualMagSq_.size() << endl;
        cout << endl;
        cout << "{MUI_Coupling} Residual of MUI Coupling is: " << residualL2Norm_ << endl;
        cout << "{MUI_Coupling} residualL2NormMax of MUI Coupling is: " << residualL2NormMax_ << endl;
        cout << "{MUI_Coupling} residualMagSqSum of MUI Coupling is: " << residualMagSqSum_ << endl;
    }

}


void muiCouplingIQNIMVJ::storeFetchDisp( double fetchMUIx, 
                                        double fetchMUIy, 
                                        double fetchMUIz,
                                        int pointN)
{

    fetchDispCoupling_[(3*(pointN-1))+0] = fetchMUIx;

    fetchDispCoupling_[(3*(pointN-1))+1] = fetchMUIy;

    fetchDispCoupling_[(3*(pointN-1))+2] = fetchMUIz;

    //cout << "residualCoupling_: " << residualCoupling_[(3*(pointN-1))+0] << endl;

}

void muiCouplingIQNIMVJ::reSetRefMatrixes()
{

    matrixVref.resize(0,0); 

    matrixWref.resize(0,0);

}

void muiCouplingIQNIMVJ::calMatrixes(int iterN)
{

    // Calculate matrixVk
    matrixVk.resize(0,0);

    VectorXd vectorVTemp = VectorXd::Map(&residualCoupling_[0],residualCoupling_.size());

    matrixVk.resize(3*pointSize_,iterN-1);

    matrixVk = matrixVref.array().colwise() - vectorVTemp.array();

    vectorVTemp.resize(0);

    // Calculate matrixWk
    matrixWk.resize(0,0);

    VectorXd vectorWTemp = VectorXd::Map(&fetchDispCoupling_[0],fetchDispCoupling_.size());

    matrixWk.resize(3*pointSize_,iterN-1);

    matrixWk = matrixWref.array().colwise() - vectorWTemp.array();

    vectorWTemp.resize(0);

}

void muiCouplingIQNIMVJ::refMatrixesPushback(int iterN)
{

    // Push back matrixVref
    MatrixXd matrixVrefTemp = matrixVref;
    MatrixXd vectorVrefTemp = VectorXd::Map(&residualCoupling_[0],residualCoupling_.size());

    matrixVref.resize(3*pointSize_,iterN);

    if(matrixVrefTemp.size() != 0)
    {
        matrixVref.leftCols(iterN-1) = matrixVrefTemp;
    }

    matrixVref.rightCols(1) = vectorVrefTemp.rightCols(1);

    matrixVrefTemp.resize(0,0);
    vectorVrefTemp.resize(0,0); 

    // Push back matrixWref
    MatrixXd matrixWrefTemp = matrixWref;
    MatrixXd vectorWrefTemp = VectorXd::Map(&fetchDispCoupling_[0],fetchDispCoupling_.size());

    matrixWref.resize(3*pointSize_,iterN);

    if(matrixWrefTemp.size() != 0)
    {
        matrixWref.leftCols(iterN-1) = matrixWrefTemp;
    }

    matrixWref.rightCols(1) = vectorWrefTemp.rightCols(1);

    matrixWrefTemp.resize(0,0);
    vectorWrefTemp.resize(0,0);

}

void muiCouplingIQNIMVJ::calInvJacobian()
{

    VectorXd vectorVkTemp = matrixVk.rightCols(1);
    
    VectorXd vectorWkTemp = matrixWk.rightCols(1);
    
    matrixInvJacobian = matrixInvJacobianPrev 
                        + (
                            (vectorWkTemp - (matrixInvJacobianPrev * vectorVkTemp))
                            /(vectorVkTemp.dot(vectorVkTemp))
                          );

    vectorVkTemp.resize(0);
    vectorWkTemp.resize(0);

/*     matrixInvJacobian = matrixInvJacobianPrev 
                        + (
                            (matrixWk - (matrixInvJacobianPrev * matrixVk))
                            *
                            (matrixVk.transpose()*matrixVk).inverse()
                            *
                            matrixVk.transpose()
                          ); */

}


void muiCouplingIQNIMVJ::setInvJacobianPrev()
{

    matrixInvJacobianPrev = matrixInvJacobian;

}

void muiCouplingIQNIMVJ::calcDeltaDispIQN()
{

    MatrixXd vectorResidualTemp = VectorXd::Map(&residualCoupling_[0],residualCoupling_.size());

    VectorXd vectorDeltaDispTemp = (-matrixInvJacobian) * vectorResidualTemp;

    VectorXd::Map(&deltaDisp_[0], vectorDeltaDispTemp.size()) = vectorDeltaDispTemp;

    vectorResidualTemp.resize(0,0);

    vectorDeltaDispTemp.resize(0);

}

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

muiCouplingIQNIMVJ::muiCouplingIQNIMVJ
(
    int pointSize,
    double initUndRelxCpl,
    MPI_Comm *Cppworld
)
: 
    pointSize_(pointSize),
    initundRelxCpl_(initUndRelxCpl),
    world_(*Cppworld)
{
    cout << "{MUI_Coupling} C++ side, constructor: " << pointSize_ << "  " << initundRelxCpl_ <<endl;

    MPI_Comm_rank( world_, &CppRank_ );
    MPI_Comm_size( world_, &CppSize_ );
    cout << "{MUI_Coupling} Hello world from rank: " << CppRank_ << " out of " << CppSize_ << " processors. " <<endl;

}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

muiCouplingIQNIMVJ::~muiCouplingIQNIMVJ()
{
    cout << "{MUI_Coupling} C++ side, destructor" << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return x axis component of the delta displacement
double muiCouplingIQNIMVJ::getXDeltaDisp(int pointV)
{
    it_ = pointMap_.find(pointV);
  
    assert (it_ != pointMap_.end());
    return deltaDisp_[(3*(((*it_).second)-1))+0];

}

//- Return y axis component of the delta displacement
double muiCouplingIQNIMVJ::getYDeltaDisp(int pointV)
{
    it_ = pointMap_.find(pointV);

    assert (it_ != pointMap_.end());
    return deltaDisp_[(3*(((*it_).second)-1))+1];
}

//- Return z axis component of the delta displacement
double muiCouplingIQNIMVJ::getZDeltaDisp(int pointV)
{
    it_ = pointMap_.find(pointV);

    assert (it_ != pointMap_.end());
    return deltaDisp_[(3*(((*it_).second)-1))+2];
}

void muiCouplingIQNIMVJ::initialize()
{

    undRelxCplMax_ = 1.0;

    aitkenIterationN_ = 3;

    residualMagSq_.resize(pointSize_, 0.0);

    residualDelta_.resize((3*pointSize_), 0.0);

    residualCoupling_.resize((3*pointSize_), 0.0);

    residualCouplingPrev_.resize((3*pointSize_), 0.0);

    fetchDispCoupling_.resize((3*pointSize_), 0.0);

    deltaDisp_.resize((3*pointSize_), 0.0);

    matrixInvJacobian= MatrixXd::Zero((3*pointSize_), (3*pointSize_));

    matrixInvJacobianPrev= MatrixXd::Zero((3*pointSize_), (3*pointSize_));

    calcUndRelx(0);

    setUndRelxPrev();

/*       for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
    {

        cout << "Init pointN: " << pointN + 1 << endl;
        cout << "Init residualCouplingPrev_: " << residualCouplingPrev_[pointN] << endl;

    }  */ 

}

void muiCouplingIQNIMVJ::collectResidual(    double fetchMUIx,
                                            double fetchMUIy,
                                            double fetchMUIz,
                                            double disPreX,
                                            double disPreY,
                                            double disPreZ,
                                            int pointV)
{

    //calculate point ID
    int pointN = -999;
    pointN = calcpointN(pointV);
    assert (pointN >= 0);

    storeFetchDisp( fetchMUIx,
                    fetchMUIy,
                    fetchMUIz,
                    pointN);

    calcResidual(   disPreX,
                    disPreY,
                    disPreZ,
                    pointN);

}

void muiCouplingIQNIMVJ::process(int iterN)
{

    assert(iterN >= 0);

    if (iterN == 0)
    {
        // Skip zero iteration step
    }
    else if ((iterN <= aitkenIterationN_) && (iterN > 0))
    {

        if (iterN == 1)
        {
            reSetRefMatrixes();
        }
        else
        {
            calMatrixes(iterN);

            calInvJacobian();            
        }

        calcUndRelx(iterN);

        calcDeltaDispAitken();

        setPrevResidual();

        accumResidualMagSqSum();

        calResidualL2Norm();

        calResidualL2NormMax();

        setResidualL2Norm();

        setUndRelxPrev();

        logging();

        resetResidualMagSqSum();

        refMatrixesPushback(iterN);

        setInvJacobianPrev();

    }
    else
    {

        calMatrixes(iterN);

        calInvJacobian();  

        calcDeltaDispIQN();

        refMatrixesPushback(iterN);

        setInvJacobianPrev();

        setPrevResidual();

        accumResidualMagSqSum();

        calResidualL2Norm();

        calResidualL2NormMax();

        setResidualL2Norm();

        setUndRelxPrev();

        logging();

    }

}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace muiCoupling

// ************************************************************************* //