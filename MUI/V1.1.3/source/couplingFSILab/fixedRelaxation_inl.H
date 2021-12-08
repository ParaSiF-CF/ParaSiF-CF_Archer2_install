// No license
// ----------

// BAE-FSI
// fixedRelaxation_inl.H
//
// This is a part of the Partitioned Multi-physical Simulation Framework (parMupSiF)
//
// FEniCSV2019.1.0+ <-> MUI(mui4py) <-> MUI(C++) <-> OpenFOAMV6+ two way Coupling Module.
//
// Incompressible Navier-Stokes equations for fluid domain in OpenFOAM
// Structure dynamics equations for structure domain in FEniCS
//
// fixedRelaxation_inl.H is a C++ code of Fixed Relaxation Coupling Method 
// muiCouplingFixedRelaxation class of FSI Coupling utility for the MUI library of the parMupSiF code
//
// located in the src/MUI_Utility/couplingLab sub-folder of the parMupSiF folder
//
// Created: 29-May-2019
//
// Author(s): W.L
// Contact: wendi.liu@stfc.ac.uk
//
// Copyright 2019 UK Research and Innovation
// IBM Confidential
// OCO Source Materials
// 5747-SM3
// (c) Copyright IBM Corp. 2017, 2019
// The source code for this program is not published or otherwise 
// divested of its trade secrets, irrespective of what has 
// been deposited with the U.S. Copyright Office. 
//
// All rights reserved

#ifndef MUICOUPLINGFIXEDRELAXATION_HPP
#define MUICOUPLINGFIXEDRELAXATION_HPP

#include "mui.h"

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <mpi.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace muiCoupling
{

/*---------------------------------------------------------------------------*\
                 Class muiCouplingFixedRelaxation Declaration
\*---------------------------------------------------------------------------*/

    class muiCouplingFixedRelaxation 
    {
        
    private:

        // Private data

            //-MPI Common World
            MPI_Comm world_;

            //-MPI Common Rank      
            int CppRank_;

            //-MPI Common Size      
            int CppSize_;

            //- Under relaxation factor of the coupling
            double undRelxCpl_;

            const double initundRelxCpl_;

            int pointSize_ = -999;
        
            std::vector<double> residualCoupling_;
        
            mutable std::vector<double> residualCouplingPrev_;
            
            std::vector<double> deltaDisp_;
        
            std::vector<double> residualMagSq_;

            double residualMagSqSum_;
        
            double residualL2Norm_;
        
            double residualL2NormMax_;
            
            mutable std::map <int, int> pointMap_ ;

            std::map <int, int> :: iterator it_;

        // Private Member Functions

            //- Calculation of point ID
            inline int calcpointN(int pointV)
            {

                it_ = pointMap_.find(pointV);
        
                if(it_ != pointMap_.end())
                {
                    return (( *it_ ).second);
                }
                else
                {
                    pointMap_.insert(std::pair<int, int>(pointV, (pointMap_.size()+1)));
                    return (pointMap_.size());
                }

            }

            //- Calculation of residual at present iteration                           
            inline void calcResidual(   double fetchMUIx, 
                                        double fetchMUIy, 
                                        double fetchMUIz,
                                        double disPreX,
                                        double disPreY,
                                        double disPreZ,
                                        int pointN)
            {
                residualCoupling_[(3*(pointN-1))+0] = fetchMUIx - disPreX;
                residualCoupling_[(3*(pointN-1))+1] = fetchMUIy - disPreY;
                residualCoupling_[(3*(pointN-1))+2] = fetchMUIz - disPreZ;
            }

            //- Calculation of under relaxation factor at present iteration
            inline void calcUndRelx()
            {
#ifdef USE_MUI
                if (Pstream::master())
                {
                    undRelxCpl_ = initundRelxCpl_;
                }
                Foam::Pstream::scatter(undRelxCpl_);
#else
                if (CppRank_ == 0)
                {
                    undRelxCpl_ = initundRelxCpl_;
                }
                MPI_Bcast(&undRelxCpl_, 1, MPI_DOUBLE, 0, world_);
#endif
            }

            //- Calculation of delta displacement        
            inline void calcDeltaDisp()
            {
                for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
                {
                    deltaDisp_[pointN] = undRelxCpl_ * residualCoupling_[pointN];
                }
            }
            
            //- Store residual at this iteration
            inline void setPrevResidual()
            {
                for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
                {
                    residualCouplingPrev_[pointN] = residualCoupling_[pointN];
                }
            }

            //- Reset of residualMagSqSum_
            inline void resetResidualMagSqSum()
            {
                residualMagSqSum_ = 0.0;
            }

            //- Accumulation of residualMagSqSum_
            inline void accumResidualMagSqSum()
            {
                double localResidualMagSqSum_ = 0.0;
                for (int pointN=0; pointN<=(3*pointSize_-1); ++pointN)
                {
                    localResidualMagSqSum_ +=    std::pow(residualCoupling_[pointN], 2);
                }
#ifdef USE_MUI
                std::cout << "{MUI_Coupling} From rank " << Pstream::myProcNo() 
                        << " localResidualMagSqSum_: " << localResidualMagSqSum_ << std::endl;
                residualMagSqSum_ = localResidualMagSqSum_;
                Foam::reduce(residualMagSqSum_, sumOp<scalar>());
#else
                std::cout << "{MUI_Coupling} From rank " << CppRank_ 
                    << " localResidualMagSqSum_: " << localResidualMagSqSum_ << std::endl;
                MPI_Allreduce(&localResidualMagSqSum_, &residualMagSqSum_, 1, MPI_DOUBLE, MPI_SUM, world_);
#endif
            }

            //- Calculation of residualL2Norm_         
            inline void calResidualL2Norm()
            {
#ifdef USE_MUI
                if (Pstream::master())
                {
                    residualL2Norm_ = std::sqrt(residualMagSqSum_);
                }
                Foam::Pstream::scatter(residualL2Norm_); 
                std::cout << "residualMagSqSum_: " << residualMagSqSum_ 
                    << " residualL2Norm_: " << residualL2Norm_ << " rank: " 
                    << Pstream::myProcNo() << std::endl;
#else
                if (CppRank_ == 0)
                {
                    residualL2Norm_ = std::sqrt(residualMagSqSum_);
                }
                MPI_Bcast(&residualL2Norm_, 1, MPI_DOUBLE, 0, world_);
                std::cout << "residualMagSqSum_: " << residualMagSqSum_ 
                    << " residualL2Norm_: " << residualL2Norm_ << " rank: " 
                    << CppRank_ << std::endl;
#endif
            }
            
            //- Calculation of residualL2NormMax_       
            inline void calResidualL2NormMax()
            {
#ifdef USE_MUI
                if (Pstream::master())
                {
                    residualL2NormMax_ = max(residualL2Norm_, residualL2NormMax_);
                }
                Foam::Pstream::scatter(residualL2NormMax_); 
                std::cout << "residualL2NormMax_: " << residualL2NormMax_ 
                    << " residualL2Norm_: " << residualL2Norm_ << " rank: " 
                    << Pstream::myProcNo() << std::endl;
#else
                if (CppRank_ == 0)
                {
                    residualL2NormMax_ = std::max(residualL2Norm_, residualL2NormMax_);
                }
                MPI_Bcast(&residualL2NormMax_, 1, MPI_DOUBLE, 0, world_);
                std::cout << "residualL2NormMax_: " << residualL2NormMax_ 
                    << " residualL2Norm_: " << residualL2Norm_ << " rank: " 
                    << CppRank_ << std::endl;
#endif
            }

            //- Correction of residualL2Norm_ based on the maximum residualL2NormMax_        
            inline void setResidualL2Norm()
            {
#ifdef USE_MUI
                if (Pstream::master())
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
                Foam::Pstream::scatter(residualL2Norm_); 
#else
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
                MPI_Bcast(&residualL2Norm_, 1, MPI_DOUBLE, 0, world_);
#endif
            }

            //- Logging        
            inline void logging()
            {
#ifdef USE_MUI
                if (Pstream::master())
                {
                    std::cout << std::scientific;
                    std::cout << std::endl;
                    std::cout << "{MUI_Coupling} No. of points is: " << pointSize() << std::endl;
                    std::cout << "{MUI_Coupling} Under-relaxation factor is: " << undRelxCpl() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualCoupling_ is: " << residualCoupling_.size() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualCouplingPrev_ is: " << residualCouplingPrev_.size() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualMagSq_ is: " << residualMagSq_.size() << std::endl;
                    std::cout << std::endl;
                    std::cout << "{MUI_Coupling} Residual of MUI Coupling is: " << residualL2Norm_ << std::endl;    
                    std::cout << "{MUI_Coupling} residualL2NormMax of MUI Coupling is: " << residualL2NormMax_ << std::endl;    
                    std::cout << "{MUI_Coupling} residualMagSqSum of MUI Coupling is: " << residualMagSqSum_ << std::endl;    
                }
#else
                if (CppRank_ == 0)
                {
                    std::cout << std::scientific;
                    std::cout << std::endl;
                    std::cout << "{MUI_Coupling} No. of points is: " << pointSize() << std::endl;
                    std::cout << "{MUI_Coupling} Under-relaxation factor is: " << undRelxCpl() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualCoupling_ is: " << residualCoupling_.size() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualCouplingPrev_ is: " << residualCouplingPrev_.size() << std::endl;
                    std::cout << "{MUI_Coupling} Size of residualMagSq_ is: " << residualMagSq_.size() << std::endl;
                    std::cout << std::endl;
                    std::cout << "{MUI_Coupling} Residual of MUI Coupling is: " << residualL2Norm_ << std::endl;    
                    std::cout << "{MUI_Coupling} residualL2NormMax of MUI Coupling is: " << residualL2NormMax_ << std::endl;    
                    std::cout << "{MUI_Coupling} residualMagSqSum of MUI Coupling is: " << residualMagSqSum_ << std::endl;    
                }
#endif
            }

    protected:

        // Protected member functions

    public:
        // Static data members

        // Constructors

            //- Constructor - default
            inline muiCouplingFixedRelaxation
            (
                double initUndRelxCpl
            ):
            initundRelxCpl_(initUndRelxCpl)
            {
                std::cout << "{MUI_Coupling} C++ FixedRelaxation default constructor" 
                    << " initial under-relaxation factor: " << initundRelxCpl_ << std::endl;
            }

            //- Construct from components
            inline muiCouplingFixedRelaxation
            (
                int pointSize,
                double initUndRelxCpl
            )
            :
                pointSize_(pointSize),
                initundRelxCpl_(initUndRelxCpl)
            {
                std::cout << "{MUI_Coupling} C++ FixedRelaxation constructor: " 
                   <<" number of points: " << pointSize_ 
                   << " initial under-relaxation factor: " << initundRelxCpl_ <<std::endl;
            }

            //- Construct from components - overload
            inline muiCouplingFixedRelaxation
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
                std::cout << "{MUI_Coupling} C++ FixedRelaxation constructor: " 
                    <<" number of points: " << pointSize_ 
                   << " initial under-relaxation factor: " << initundRelxCpl_ <<std::endl;
                MPI_Comm_rank( world_, &CppRank_ );
                MPI_Comm_size( world_, &CppSize_ );
                std::cout << "{MUI_Coupling} Hello world from rank: " << CppRank_ 
                    << " out of " << CppSize_ << " processors. " <<std::endl;
            }

        // Selectors

        // Destructor - default

            inline ~muiCouplingFixedRelaxation
            (
        
            )
            {
                std::cout << "{MUI_Coupling} C++ FixedRelaxation destructor" << std::endl;
            }

        // Member Functions
            // Access

                //- Return under relaxation factor of the coupling
                inline double undRelxCpl()
                {
                    return undRelxCpl_;
                }

                //- Return No. of points
                inline int pointSize() const
                {
                    return pointSize_;
                }

                //- Return square sum of the residual
                inline double residualMagSqSum() const
                {
                    return residualMagSqSum_;
                }

                //- Return maximum value of the residual L-2 Norm
                inline double residualL2NormMax() const
                {
                    return residualL2NormMax_;
                }

                //- Return the value of the residual L-2 Norm
                inline double residualL2Norm() const
                {
                    return residualL2Norm_;
                }

                //- Return x axis component of the delta displacement
                inline double getXDeltaDisp(int pointV)
                {
                    it_ = pointMap_.find(pointV);
                    assert (it_ != pointMap_.end());
                    return deltaDisp_[(3*(((*it_).second)-1))+0];
                }

                //- Return y axis component of the delta displacement
                inline double getYDeltaDisp(int pointV)
                {
                    it_ = pointMap_.find(pointV);
                    assert (it_ != pointMap_.end());
                    return deltaDisp_[(3*(((*it_).second)-1))+1];
                }

                //- Return z axis component of the delta displacement
                inline double getZDeltaDisp(int pointV)
                {
                    it_ = pointMap_.find(pointV);
                    assert (it_ != pointMap_.end());
                    return deltaDisp_[(3*(((*it_).second)-1))+2];
                }

              // Edit

                //- Initialize coupling method
                inline void initialize(int pointSize)
                {
                    pointSize_ = pointSize;
                    residualMagSq_.resize(pointSize_, 0);
                    residualCoupling_.resize((3*pointSize_), 0);
                    residualCouplingPrev_.resize((3*pointSize_), 0);
                    deltaDisp_.resize((3*pointSize_), 0);
                    calcUndRelx();
                    std::cout << "{MUI_Coupling} C++ FixedRelaxation initialize: " 
                    <<" number of points: " << pointSize_ <<std::endl;
                }

                //- Initialize coupling method - overload
                inline void initialize(int pointSize,
                                       MPI_Comm *Cppworld)
                {
                    pointSize_ = pointSize;
                    world_ = *Cppworld;
                    residualMagSq_.resize(pointSize_, 0);
                    residualCoupling_.resize((3*pointSize_), 0);
                    residualCouplingPrev_.resize((3*pointSize_), 0);
                    deltaDisp_.resize((3*pointSize_), 0);
                    calcUndRelx();
                    std::cout << "{MUI_Coupling} C++ FixedRelaxation initialize: " 
                        <<" number of points: " << pointSize_ <<std::endl;
                    MPI_Comm_rank( world_, &CppRank_ );
                    MPI_Comm_size( world_, &CppSize_ );
                    std::cout << "{MUI_Coupling} Hello world from rank: " << CppRank_ 
                        << " out of " << CppSize_ << " processors. " <<std::endl;
                }

                //- Initialize coupling method - overload
                inline void initialize()
                {
                    residualMagSq_.resize(pointSize_, 0);
                    residualCoupling_.resize((3*pointSize_), 0);
                    residualCouplingPrev_.resize((3*pointSize_), 0);
                    deltaDisp_.resize((3*pointSize_), 0);
                    calcUndRelx();
                }

                //- Collection of coupling process at this iteration
                inline void collectResidual(double fetchMUIx, 
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
                    calcResidual(fetchMUIx, 
                                fetchMUIy, 
                                fetchMUIz,
                                disPreX,
                                disPreY,
                                disPreZ,
                                pointN);
                }
     
                //- Collection of residual calculation at this iteration
                inline void process()
                {
                    calcUndRelx();
                    calcDeltaDisp();
                    setPrevResidual();
                    accumResidualMagSqSum();
                    calResidualL2Norm();
                    calResidualL2NormMax();
                    setResidualL2Norm();
                    logging();
                    resetResidualMagSqSum();
                }
    };

// Global Functions


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace muiCoupling

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // End MUICOUPLINGFIXEDRELAXATION_HPP

// ************************************************************************* //