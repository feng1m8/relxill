/*
   This file is part of the RELXILL model code.

   RELXILL is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   RELXILL is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
   For a copy of the GNU General Public License see
   <http://www.gnu.org/licenses/>.

    Copyright 2022 Thomas Dauser, Remeis Observatory & ECAP
*/

#ifndef RELXILL_SRC_PRIMARYSOURCE_H_
#define RELXILL_SRC_PRIMARYSOURCE_H_

#include <string>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>

#include "ModelParams.h"
#include "Relphysics.h"
#include "Relbase.h"

extern "C" {
#include "common.h"
#include "xilltable.h"
}

#include <vector>

typedef struct {
} primeSourceParam;

class PrimarySourceParameters {

 public:

  explicit PrimarySourceParameters(const ModelParams &inp_param) : m_inp_param{inp_param} {

    m_rel_param = get_rel_params(m_inp_param);  // can be nullptr if xillver model

    if (m_rel_param != nullptr && m_rel_param->emis_type == EMIS_TYPE_LP) {
      energy_shift_source_observer = energy_shift_source_obs(m_rel_param);
    } else { // energy shift set to "1" for non-LP and xillver models
      energy_shift_source_observer = 1.0;
    }

    m_xilltab_param = m_get_xillver_params_primary_source(m_rel_param, energy_shift_source_observer);

    refl_frac = inp_param.get_otherwise_default(XPar::refl_frac, 0);
    mass = inp_param.get_otherwise_default(XPar::mass, 0);
    distance = inp_param.get_otherwise_default(XPar::distance, 0);
    lbol = inp_param.get_otherwise_default(XPar::lbol, 0);
    interpret_reflfrac_as_boost =
        static_cast<int>(lround(inp_param.get_otherwise_default(XPar::switch_switch_reflfrac_boost, 0)));
  }

  ~PrimarySourceParameters() {
    delete m_rel_param;
    delete m_xilltab_param;
  }


  // return full xilltab and rel_param structures (TODO: refactoring methods such that this is not needed, at least for rel_param?)
  [[nodiscard]] const xillTableParam *xilltab_param() const {
    return m_xilltab_param;
  }
  [[nodiscard]] const relParam *rel_param() const {
    return m_rel_param;
  }

  int model_type() const {
    return m_xilltab_param->model_type;
  }
  int prim_type() const {
    return m_xilltab_param->prim_type;
  }
  int emis_type() const { // need to make sure it works for xillver models as well
    return (m_rel_param != nullptr) ? m_rel_param->emis_type : EMIS_TYPE_NONE;
  }

  xillTableParam *get_xillver_params_primary_source() {
    return m_get_xillver_params_primary_source(m_rel_param, energy_shift_source_observer);
  }

  double lbol;
  double mass;
  double distance;
  double refl_frac;
  int interpret_reflfrac_as_boost;
  double energy_shift_source_observer = 1.0; // by default, no energy shift

  // functions for parameters we need
  [[nodiscard]] double gam() const {
    return m_xilltab_param->gam;
  }
  [[nodiscard]] double beta() const {
    return m_rel_param->beta;
  }
  [[nodiscard]] double ect() const {
    return m_xilltab_param->ect;
  }

 private:
  ModelParams m_inp_param;
  relParam *m_rel_param = nullptr;
  xillTableParam *m_xilltab_param = nullptr;

  xillTableParam *m_get_xillver_params_primary_source(relParam *_rel_param, double _energy_shift_source_observer) {
    int status = EXIT_SUCCESS;
    auto xill_param = get_xill_params(m_inp_param);
    auto xilltab_param = get_xilltab_param(xill_param, &status);
    delete xill_param;

    // special case, for the LP model with the Ecut spectrum: cutoff energy is given in the observer frame
    // -> convert it such that ecut is also given in the source frame
    if (_rel_param != nullptr && _rel_param->emis_type == EMIS_TYPE_LP && xilltab_param->prim_type == PRIM_SPEC_ECUT) {
      xilltab_param->ect /= _energy_shift_source_observer;
    }

    return xilltab_param;
  }

};

// TODO:
//  - properly include the reflection fraction and a nice error message
class PrimarySource {

 public:
  explicit PrimarySource(const ModelParams &_model_params) :
      parameters{PrimarySourceParameters(_model_params)} {
  }

  PrimarySource(const ModelParams &_model_params, RelSysPar *sys_par) :
      parameters{_model_params} {
    m_struct_refl_frac = (sys_par == nullptr) ?
                         nullptr : sys_par->emis->photon_fate_fractions;
  }

  /*
   * @brief: print the reflection strength on the screen (requires struct_refl_frac to be set)
   */
  void print_reflection_strength(const XspecSpectrum &spectrum, const double *pl_flux) const {
    if (m_struct_refl_frac != nullptr) {
      return;
    } // will do nothing if the refl_frac structure is not set

    const relParam *rel_param = parameters.rel_param();

    int imin = binary_search(spectrum.energy, spectrum.num_flux_bins() + 1, RSTRENGTH_EMIN);
    int imax = binary_search(spectrum.energy, spectrum.num_flux_bins() + 1, RSTRENGTH_EMAX);

    double sum_pl = 0.0;
    double sum = 0.0;
    for (int ii = imin; ii <= imax; ii++) {
      sum_pl += pl_flux[ii];
      sum += spectrum.flux[ii];
    }

    printf("For a = %.3f, Rin = %.3f, and h = %.2f rg", rel_param->a, rel_param->rin, rel_param->height);
    if (is_iongrad_model(rel_param->ion_grad_type) || rel_param->beta > 1e-6) {
      printf(" and beta=%.3f v/c", rel_param->beta);
    }
    printf(" (using boost=1): \n - reflection fraction  %.3f \n - reflection strength is: %.3f \n",
           m_struct_refl_frac->refl_frac,
           sum / sum_pl);
    printf(" - photons falling into the black hole or plunging region: %.2f%%\n", m_struct_refl_frac->f_bh * 100);
    printf(" - energy shift from the primary source to the observer is %.3f\n", energy_shift_source_obs(rel_param));
  }

  // @brief: adds primary spectrum to the input spectrum
  void add_primary_spectrum(const XspecSpectrum &spectrum) {

    int status = EXIT_SUCCESS;
    double *pl_flux = calc_normalized_primary_spectrum(spectrum.energy, spectrum.num_flux_bins(),
                                                       parameters.rel_param(), parameters.xilltab_param(), &status);
    CHECK_STATUS_VOID(status);

    // For the non-relativistic model and if not the LP geometry, we simply multiply by the reflection fraction
    if (is_xill_model(parameters.model_type()) || parameters.emis_type() != EMIS_TYPE_LP) {
      spectrum.multiply_flux_by(fabs(parameters.refl_frac));

    } else { // we are in the LP geometry

      assert(m_struct_refl_frac != nullptr);

      double refl_frac = parameters.refl_frac;  // todo: disentable refl_frac and boost
      if (parameters.interpret_reflfrac_as_boost) {
        // if set, it is given as boost, wrt predicted refl_frac
        refl_frac *= m_struct_refl_frac->refl_frac;
      }

      double prim_fac =
          m_struct_refl_frac->f_inf_rest / 0.5 * pow(parameters.energy_shift_source_observer, parameters.gam());
      // flux boost of primary radiation taking into account here (therfore we need f_inf_rest above)
      if (parameters.beta() > 1e-4) {
        prim_fac *= pow(doppler_factor_source_obs(parameters.rel_param()), 2);
      }

      // if the user sets the refl_frac parameter manually, we need to calculate the ratio
      // to end up with the correct normalization
      double norm_fac_refl = (fabs(refl_frac)) / m_struct_refl_frac->refl_frac;

      spectrum.multiply_flux_by(norm_fac_refl);
      for (int ii = 0; ii < spectrum.num_flux_bins(); ii++) {
        pl_flux[ii] *= prim_fac;
      }

      /** 5 ** if desired, we ouput the reflection fraction and strength (as defined in Dauser+2016) **/
      if (shouldAuxInfoGetPrinted()) {
        print_reflection_strength(spectrum, pl_flux);
      }

    }

    // Finally, add the power law component if refl_frac >= 0
    if (parameters.refl_frac >= 0) {
      for (int ii = 0; ii < spectrum.num_flux_bins(); ii++) {
        spectrum.flux[ii] += pl_flux[ii];
      }
    }

    delete[] pl_flux;

  }

  xillTableParam *get_xillver_params_primary_source() {
    auto *xilltab_param = new xillTableParam;
    *xilltab_param = *parameters.xilltab_param();
    return xilltab_param;
  }

  const PrimarySourceParameters parameters;

 private:
  lpReflFrac *m_struct_refl_frac = nullptr;
};

#endif //RELXILL_SRC_PRIMARYSOURCE_H_