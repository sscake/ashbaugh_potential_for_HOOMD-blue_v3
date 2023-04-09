// Copyright (c) 2009-2022 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.
// ashbaugh_potential_for_HOOMD-blue_v3: revised by Xianshi Liu @ Fudan University, 2023.

// Include the defined classes that are to be exported to python
#include "EvaluatorPairAshbaugh.h"

#include "hoomd/md/PotentialPair.h"
#include <pybind11/pybind11.h>
#ifdef ENABLE_HIP
#include "hoomd/md/PotentialPairGPU.h"
#endif

namespace hoomd
    {
namespace md
    {

// specify the python module. Note that the name must explicitly match the PROJECT() name provided
// in CMakeLists (with an underscore in front)
PYBIND11_MODULE(_pair_plugin, m)
    {
    detail::export_PotentialPair<EvaluatorPairAshbaugh>(m, "PotentialPairAshbaugh");
#ifdef ENABLE_HIP
    detail::export_PotentialPairGPU<EvaluatorPairAshbaugh>(m, "PotentialPairAshbaughGPU");
#endif
    }

    } // end namespace md
    } // end namespace hoomd
