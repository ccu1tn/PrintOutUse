//===- NvDlaIdentifyShufflePass.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaIdentifyShufflePass.h"

#include "Compute/NvDlaShuffle.h"
#include "NvDlaDefine.h"

#include <onnc/Core/PassSupport.h>
//#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Use.h>

using namespace onnc;
using namespace foonvdla;

//===----------------------------------------------------------------------===//
// NvDlaIdentifyShufflePass
//===----------------------------------------------------------------------===//
Pass::ReturnType NvDlaIdentifyShufflePass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType NvDlaIdentifyShufflePass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret   = Pass::kModuleNoChanged;

  //---------------------------------------------------------------------
  // Find out users in the model graph.
  //---------------------------------------------------------------------
  
  std::vector<Use*> user;
  for (auto& op : pCG) {
    if (Use* user1= dyn_cast<Use>(&op)) {
     if (is_shuffle(user1)) { // A channel-shuffle pattern is detected.
        // Save the first node of this pattern into a queue.
        // We will replace this pattern by a single Shuffle IR later on.
       user.push_back(user1);

        // Since a node replacement will happen in the model, the model graph
        // will be changed and thus this function should return kModuleChanged.
        ret |= Pass::kModuleChanged;
      }
    }
  }

  //---------------------------------------------------------------------------
  // Replace every Reshape-Transpose-Reshape pattern with a single Shuffle IR.
  //---------------------------------------------------------------------------
  
  for (Use* user1 : user) {

    // Derive the Tranpose and the second Reshape.
    auto* use  = dyn_cast<Use>(user1->op.getOutput(0)->getUses()[0].getUser());
    //auto* reshape2 = dyn_cast<Reshape>(transpose->getOutput(0)->getUses()[0].getUser());

    //Tensor* input_tensor = user->getInput(0);
    //Tensor* shape1_tensor = reshape1->getInput(1);
    Tensor* user = static_cast<ComputeOperator*>(op.getInput(0)->getDefine());
    //Tensor* reshape1_out_tensor = reshape1->getOutput(0);
    //Tensor* transpose_out = transpose->getOutput(0);
    //Tensor* shape2_tensor = reshape2->getInput(1);
    //auto shape2_initializer = static_cast<ComputeOperator*>(shape2_tensor->getDefine());
    //Tensor* output_tensor = user->getOutput(0);

    // Remove the edges between some operators and their input/output tensors.
    // Remove an edge means to erase the records within an operator's data structure about its input tensors.
    user->showAllInputs();
    user->showAllOutputs();

    // show used in the ONNC IR graph.
    pCG.show(*user);
    
  }

  pCG.topologicalSort();

  return ret;
}

