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

    Copyright 2020 Thomas Dauser, Remeis Observatory & ECAP
*/

#include "cppmodels.h"

#include <stdexcept>
#include <iostream>

// namespace  relxill{


void eval_line_model() {
  // shift the spectrum such that we can calculate the line for 1 keV
  double *ener1keV = shift_energ_spec_1keV(ener, n_ener, param_struct->lineE, param_struct->z, status);
  CHECK_STATUS_VOID(*status);

  // call the function which calculates the line (assumes a line at 1keV!)
  rel_spec *spec = relbase(ener1keV, n_ener, rel_para, , NULL, status);

  int ii;
  for (ii = 0; ii < n_ener; ii++) {
    photar[ii] = spec->flux[0][ii];
  }
}

void eval_relxill_model() {
  relxill_kernel(ener, flux, n_ener0, xill_param, rel_param, status);
  double *ener_shifted = shift_energ_spec_1keV(ener0, n_ener0, 1.0, rel_param->z, status);
  rebin_spectrum(ener_shifted, photar, n_ener0, ener, flux, n_ener0);
}

void eval_conv_model() {
  // shift the spectrum such that we can calculate the line for 1 keV
  double *ener1keV = shift_energ_spec_1keV(ener, n_ener, param_struct->lineE, param_struct->z, status);
  CHECK_STATUS_VOID(*status);

  relconv_kernel(ener1keV, photar, n_ener, rel_param, status);
}

void eval_xillver_model() {
  xillver_base(ener0, n_ener0, photar, xill_param, status);
}

void eval_model_xspec(ModelName model, const Array &energy, const Array &flux, const Array &parameter) {

  try {
    auto const model_info = ModelDefinition::instance().getModelInfo(model);

    //lmod{model, parameter};

    // use a "LocalModel" class which contains
    // - parameter array
    // - model type / id
    auto param_map = ParamMap(model_info.parameters(), parameter);

    // Spectrum spec{energy, flux};
    //  Parameters pars{parameter, std::move(par_names)};


    switch (model_info.type()) {

      case T_Model::Line:puts(" I am LINE ");
        eval_line_model();
        break;

      case T_Model::Relxill:puts(" I am RELXILL ");
        eval_relxill_model();
        break;

      case T_Model::Conv:puts(" I am CONV ");
        eval_conv_model();

      case T_Model::Xill:puts(" I am XILL ");
        eval_xillver_model();
        break;
    }

  } catch (std::out_of_range &e) {
    std::cout << " *** relxill-error: required model not found in database " << std::endl;
    throw e;
  }

}

extern "C" {

[[maybe_unused]] void lmodcpprelline(const Array &energy, const Array &parameter,
                                     int spectrum, Array &flux, Array &fluxError,
                                     const string &init) {
  eval_model_xspec(ModelName::relline, energy, flux, parameter);
}

[[maybe_unused]] void lmodcpprelxill(const Array &energy, const Array &parameter,
                                     int spectrum, Array &flux, Array &fluxError,
                                     const string &init) {
  eval_model_xspec(ModelName::relxill, energy, flux, parameter);
}

}

// }   // namespace relxill

