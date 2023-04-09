// Copyright (c) 2009-2022 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.
// ashbaugh_potential_for_HOOMD-blue_v3: revised by Xianshi Liu @ Fudan University, 2023.

#ifndef __PAIR_EVALUATOR_ASHBAUGH_H__
#define __PAIR_EVALUATOR_ASHBAUGH_H__

#ifndef __HIPCC__
#include <string>
#endif

#include "hoomd/HOOMDMath.h"

/*! \file EvaluatorPairExample.h
    \brief Defines the pair evaluator class for the example potential
*/

// need to declare these class methods with __device__ qualifiers when building in nvcc
// DEVICE is __host__ __device__ when included in nvcc and blank when included into the host
// compiler
#ifdef __HIPCC__
#define DEVICE __device__
#define HOSTDEVICE __host__ __device__
#else
#define DEVICE
#define HOSTDEVICE
#endif

namespace hoomd
    {
namespace md
    {

class EvaluatorPairAshbaugh
    {
    public:
    //! Define the parameter type used by this pair potential evaluator
    struct param_type
        {
        Scalar lj1; //!< The coefficient for 1/r^12
        Scalar lj2; //!< The coefficient for 1/r^6
        Scalar lam; //!< Controls the attractive tail, between 0 and 1
        Scalar rwcasq; //!< The square of the location of the LJ potential minimum
        Scalar wca_shift; //!< The amount to shift the repulsive part by

        DEVICE void load_shared(char*& ptr, unsigned int& available_bytes) { }

        HOSTDEVICE void allocate_shared(char*& ptr, unsigned int& available_bytes) const { }

#ifdef ENABLE_HIP
        //! Set CUDA memory hints
        void set_memory_hint() const
            {
            // default implementation does nothing
            }
#endif

#ifndef __HIPCC__
        param_type() : lj1(0), lj2(0), lam(0), rwcasq(0), wca_shift(0) { }

        param_type(pybind11::dict v, bool managed = false)
            {
            lj1 = v["lj1"].cast<Scalar>();
            lj2 = v["lj2"].cast<Scalar>();
            lam = v["lam"].cast<Scalar>();
            rwcasq = v["rwcasq"].cast<Scalar>();
            wca_shift = v["wca_shift"].cast<Scalar>();
            }

        pybind11::dict asDict()
            {
            pybind11::dict v;
            v["lj1"] = lj1;
            v["lj2"] = lj2;
            v["lam"] = lam;
            v["rwcasq"] = rwcasq;
            v["wca_shift"] = wca_shift;
            return v;
            }
#endif
        }
#ifdef SINGLE_PRECISION
        __attribute__((aligned(8)));
#else
        __attribute__((aligned(16)));
#endif

    //! Constructs the pair potential evaluator
    /*! \param _rsq Squared distance between the particles
        \param _rcutsq Squared distance at which the potential goes to 0
        \param _params Per type pair parameters of this potential
    */
    DEVICE EvaluatorPairAshbaugh(Scalar _rsq, Scalar _rcutsq, const param_type& _params)
        : rsq(_rsq), rcutsq(_rcutsq), lj1(_params.lj1), lj2(_params.lj2), lam(_params.lam),
          rwcasq(_params.rwcasq), wca_shift(_params.wca_shift)
        {
        }

    //! Example doesn't use diameter
    DEVICE static bool needsDiameter()
        {
        return false;
        }
    //! Accept the optional diameter values
    /*! \param di Diameter of particle i
        \param dj Diameter of particle j
    */
    DEVICE void setDiameter(Scalar di, Scalar dj) { }

    //! Example doesn't use charge
    DEVICE static bool needsCharge()
        {
        return false;
        }
    //! Accept the optional diameter values
    /*! \param qi Charge of particle i
        \param qj Charge of particle j
    */
    DEVICE void setCharge(Scalar qi, Scalar qj) { }

    //! Evaluate the force and energy
    /*! \param force_divr Output parameter to write the computed force divided by r.
        \param pair_eng Output parameter to write the computed pair energy
        \param energy_shift If true, the potential must be shifted so that
        V(r) is continuous at the cutoff
        \note There is no need to check if rsq < rcutsq in this method.
        Cutoff tests are performed in PotentialPair.

        \return True if they are evaluated or false if they are not because
        we are beyond the cutoff
    */
    DEVICE bool evalForceAndEnergy(Scalar& force_divr, Scalar& pair_eng, bool energy_shift)
        {
        if (rsq < rcutsq && lj1 != 0)
            {
            Scalar r2inv = Scalar(1.0)/rsq;
            Scalar r6inv = r2inv * r2inv * r2inv;
            force_divr= r2inv * r6inv * (Scalar(12.0)*lj1*r6inv - Scalar(6.0)*lj2);
            pair_eng = r6inv * (lj1*r6inv - lj2);
            if (rsq < rwcasq)
                {
                pair_eng += wca_shift;
                }
            else
                {
                force_divr *= lam;
                pair_eng *= lam;
                }
            if (energy_shift)
                {
                Scalar rcut2inv = Scalar(1.0)/rcutsq;
                Scalar rcut6inv = rcut2inv * rcut2inv * rcut2inv;
                pair_eng -= lam * rcut6inv * (lj1*rcut6inv - lj2);
                }
            return true;
            }
        else
            return false;
        }

    //! Example doesn't eval LRC integrals
    DEVICE Scalar evalPressureLRCIntegral()
        {
        return 0;
        }

    //! Example doesn't eval LRC integrals
    DEVICE Scalar evalEnergyLRCIntegral()
        {
        return 0;
        }

#ifndef __HIPCC__
    //! Get the name of this potential
    /*! \returns The potential name.
     */
    static std::string getName()
        {
        return std::string("ashbaugh");
        }

    std::string getShapeSpec() const
        {
        throw std::runtime_error("Shape definition not supported for this pair potential.");
        }
#endif

    protected:
    Scalar rsq;    //!< Stored rsq from the constructor
    Scalar rcutsq; //!< Stored rcutsq from the constructor
    Scalar lj1;     //!< lj1 parameter extracted from the params passed to the constructor
    Scalar lj2;     //!< lj2 parameter extracted from the params passed to the constructor
    Scalar lam;     //!< lambda parameter
    Scalar rwcasq;  //!< WCA cutoff radius squared
    Scalar wca_shift; //!< Energy shift for WCA part of the potential
    };

    } // end namespace md
    } // end namespace hoomd

#endif // __PAIR_EVALUATOR_ASHBAUGH_H__
