/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/gpu/cuda/CudaTypes.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/component/constraint/lagrangian/correction/UncoupledConstraintCorrection.h>
#include <sofa/component/constraint/lagrangian/correction/UncoupledConstraintCorrection.inl>
#include <sofa/core/behavior/ConstraintCorrection.inl>

namespace sofa::component::constraint::lagrangian::correction
{
using namespace sofa::gpu::cuda;

template class SOFA_GPU_CUDA_API UncoupledConstraintCorrection< CudaVec3fTypes >;
template class SOFA_GPU_CUDA_API UncoupledConstraintCorrection< CudaVec3f1Types >;

#ifdef SOFA_GPU_CUDA_DOUBLE
template class SOFA_GPU_CUDA_API UncoupledConstraintCorrection< CudaVec3dTypes >;
template class SOFA_GPU_CUDA_API UncoupledConstraintCorrection< CudaVec3d1Types >;
#endif

} // namespace sofa::component::constraint::lagrangian::correction

namespace sofa::gpu::cuda
{
using namespace sofa::component::constraint::lagrangian::correction;

const int CudaUncoupledConstraintCorrectionClass = core::RegisterObject("Supports GPU-side computations using CUDA.")
.add< UncoupledConstraintCorrection< CudaVec3fTypes > >()
.add< UncoupledConstraintCorrection< CudaVec3f1Types > >()
#ifdef SOFA_GPU_CUDA_DOUBLE
.add< UncoupledConstraintCorrection< CudaVec3dTypes > >()
.add< UncoupledConstraintCorrection< CudaVec3d1Types > >()
#endif
;

} // namespace sofa::gpu::cuda
