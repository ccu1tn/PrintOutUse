//===- NvDlaIdentifyShufflePass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <iostream>
#include <vector>


#include "NvDlaIdentifyShufflePass.h"
#include "Compute/NvDlaShuffle.h"
#include "NvDlaDefine.h"
#include <onnc/Core/PassSupport.h>
//#include <onnc/IR/Compute/Reshape.h>
//#include <onnc/IR/Compute/Transpose.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/Algorithm.h>



using namespace onnc;
using namespace foonvdla;

//===----------------------------------------------------------------------===//
// NvDlaIdentifyShuffle
//===----------------------------------------------------------------------===//
Pass::ReturnType NvDlaIdentifyShuffle::runOnModule(::onnc::Module &pModule)
{
  xGraph* graph = pModule.getRootTensorGraph();

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    xNode* n = *it;
    // Remove 'undefined' node.
    if (n->kind() == xBuiltinSymbol::kUndefined) {
      dropOutputs(*n);
      it.destroyCurrent();
    }
  }

  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg) {
    runOnComputeGraph(*cg->value());
  }

  return Pass::kModuleNoChanged;
}

Pass::ReturnType NvDlaIdentifyShuffle::runOnComputeGraph(ComputeGraph& graph)
{
  std::vector<ComputeOperator*> toRemove;
  for (ComputeOperator& op : graph) {
    if (isa<OutputOperator>(op)) {
      continue;
    }

    bool hasUser = false;
    for (unsigned idx = 0; idx < op.getNumOfOutputs(); ++idx) {
      const Value::UseList& users = op.getOutput(idx)->getUses();
      if (!empty(users)) {
        hasUser = true;
        break;
      }
    }

    if (!hasUser) {
      toRemove.emplace_back(&op);
    }
  }

  for (ComputeOperator* op : toRemove) {
    op->removeAllInputs();
    op->removeAllOutputs();
    graph.erase(*op);
  }

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(NvDlaIdentifyShuffle, "NvDlaIdentifyShuffle")
}

ModulePass* onnc::CreateNvDlaIdentifyShufflePass()
{
  return new NvDlaIdentifyShuffle();
}