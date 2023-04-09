# Copyright (c) 2009-2022 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.
# ashbaugh_potential_for_HOOMD-blue_v3: revised by Xianshi Liu @ Fudan University, 2023.

"""Ashbaugh pair potential."""

# Import the C++ module.
from hoomd.pair_plugin import _pair_plugin

# Impot the hoomd Python package and other necessary components.
from hoomd.md import pair
from hoomd.data.parameterdicts import TypeParameterDict
from hoomd.data.typeparam import TypeParameter


class Ashbaugh(pair.Pair):
    """Ashbaugh pair potential."""

    # set static class data
    _ext_module = _pair_plugin
    _cpp_class_name = "PotentialPairAshbaugh"
    _accepted_modes = ("none", "shift", "xplor")

    def __init__(self, nlist, default_r_cut=None, default_r_on=0., mode='none', tail_correction=False):
        super().__init__(nlist, default_r_cut, default_r_on, mode)
        params = TypeParameter(
            'params', 'particle_types',
            TypeParameterDict(lj1=float, lj2=float, lam=float, rwcasq=float, wca_shift=float, len_keys=2))
        self._add_typeparam(params)
#        self._param_dict.update(
#            ParameterDict(tail_correction=bool(tail_correction)))
