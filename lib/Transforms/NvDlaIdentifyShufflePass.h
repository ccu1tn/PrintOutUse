//===- NvDlaIdentifyShufflePass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------------===//
#ifndef NVDLA_IDENTIFY_SHUFFLE_PASS_H
#define NVDLA_IDENTIFY_SHUFFLE_PASS_H

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class NvDlaIdentifyShuffle
 *  
 */
class NvDlaIdentifyShuffle : public CustomPass<NvDlaIdentifyShuffle>
{
public:
  NvDlaIdentifyShuffle() = default;

  virtual ~NvDlaIdentifyShuffle() = default;

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  Pass::ReturnType runOnComputeGraph(ComputeGraph& graph) override;

  StringRef getPassName() const override { return "NvDlaIdentifyShuffle"; }
};

ModulePass* CreateNvDlaIdentifyShufflePass();

} // namespace of onnc

#endif