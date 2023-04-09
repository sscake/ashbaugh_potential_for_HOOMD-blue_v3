// Copyright (c) 2009-2022 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.
// ashbaugh_potential_for_HOOMD-blue_v3: revised by Xianshi Liu @ Fudan University, 2023.

#include "EvaluatorPairAshbaugh.h"
#include "hoomd/md/PotentialPairGPU.cuh"

namespace hoomd
    {
namespace md
    {
namespace kernel
    {
template __attribute__((visibility("default"))) hipError_t
gpu_compute_pair_forces<EvaluatorPairAshbaugh>(const pair_args_t& pair_args,
                                              const EvaluatorPairAshbaugh::param_type* d_params);
    } // end namespace kernel
    } // end namespace md
    } // end namespace hoomd
