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

    Copyright 2021 Thomas Dauser, Remeis Observatory & ECAP
*/

#ifndef RELXILL__CPPMODELDATABASE_H_
#define RELXILL__CPPMODELDATABASE_H_

#include <typeinfo>
#include <utility>
#include "ModelParams.h"
#include "ModelInfo.h"

#include "xspec_wrapper_lmodels.h"


/**
 * @class ModelNotFound:
 * exception if anything is wrong with the input parameters
 */
class ModelNotFound : public std::exception {
 public:
  ModelNotFound() = default;

  explicit ModelNotFound(const std::string &_msg) {
    m_msg += _msg;
  }

  [[nodiscard]] const char *what() const noexcept override {
    return m_msg.c_str();
  }

 private:
  std::string m_msg{"*** model not found: "};
};



/**
 * @brief not a real class, but rather a global database (container) for all possible
 * local models with, using instances of the class "ModelDefinition":
 *  - input parameters as given in the order in the lmodel.dat file
 *  - specify the type of the model, including it's relevant physical components
 */
class ModelDatabase {

 public:
  /**
   * @brief use this instance to always and securely access
   * the database, without needing to initialize it anywhere
   * else
   * @return ModelDatabase
   */
  static ModelDatabase &instance() {
    static auto *instance = new ModelDatabase();
    return *instance;
  }

  /**
   * @brief get model info (as defined in the database)
   * @param ModelName
   * @return ModelInfo
   */
  ModelInfo model_info(ModelName name){
    try {
      return lmodel_info.at(name);
    } catch (std::out_of_range &e) {
      throw ModelNotFound(lmodel_database.name_string(name));
    }
  }

  /**
   * @brief get list of Model Parameters (from lmodel.dat definition)
   * @param ModelName
   * @return ModelParamVector
   */
  ModelParamVector param_names(ModelName name){
    try {
      return lmodel_database.params(name);
    } catch (std::out_of_range &e) {
      throw ModelNotFound(lmodel_database.name_string(name));
    }
  }



 private:
  ModelDatabase() = default;  //hidden constructor and destructor to avoid initialization
  ~ModelDatabase() = default;


  // stores all scanned information from the lmodel.dat file (automatically created Class)
  XspecModelDatabase lmodel_database{}; //

  const std::unordered_map<ModelName, ModelInfo> lmodel_info = {
      {ModelName::relline, ModelInfo(T_Model::Line, T_Irrad::BknPowerlaw)},
      {ModelName::relline_lp, ModelInfo(T_Model::Line, T_Irrad::LampPost)},

      {ModelName::relconv_lp, ModelInfo(T_Model::Conv, T_Irrad::LampPost)},
      {ModelName::relconv, ModelInfo(T_Model::Conv, T_Irrad::BknPowerlaw)},

      {ModelName::relxill, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::CutoffPl)},
      {ModelName::relxillCO, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::CutoffPl)},
      {ModelName::relxillNS, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::Blackbody)},
      {ModelName::relxillD, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::CutoffPl)},
      {ModelName::relxillCp, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::Nthcomp)},
      {ModelName::relxillDCp, ModelInfo(T_Model::Relxill, T_Irrad::BknPowerlaw, T_PrimSpec::Nthcomp)},

      {ModelName::relxilllp, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::CutoffPl)},
      {ModelName::relxilllpD, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::CutoffPl)},
      {ModelName::relxilllpion, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::CutoffPl)},
      {ModelName::relxilllpCp, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::Nthcomp)},
      {ModelName::relxilllpDCp, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::Nthcomp)},
      {ModelName::relxilllpionCp, ModelInfo(T_Model::Relxill, T_Irrad::LampPost, T_PrimSpec::Nthcomp)},

      {ModelName::xillver, ModelInfo(T_Model::Xill, T_PrimSpec::CutoffPl)},
      {ModelName::xillverD, ModelInfo(T_Model::Xill, T_PrimSpec::CutoffPl)},
      {ModelName::xillverCp, ModelInfo(T_Model::Xill, T_PrimSpec::Nthcomp)},
      {ModelName::xillverDCp, ModelInfo(T_Model::Xill, T_PrimSpec::Nthcomp)},
      {ModelName::xillverCO, ModelInfo(T_Model::Xill, T_PrimSpec::CutoffPl)},
      {ModelName::xillverNS, ModelInfo(T_Model::Xill, T_PrimSpec::Blackbody)},

  };
};



#endif //RELXILL__CPPMODELDATABASE_H_