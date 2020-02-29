//===- BMScaleLower.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_BMSCALE_LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_BMSCALE_LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {
namespace BM188X {

class BMScaleLower : public Lower
{
public:
  BMScaleLower();

  ~BMScaleLower();

  int isMe(const xNode &pNode) const override;

  ComputeOperator *activate(ComputeGraph &pC, xNode &pN) const override;
};

} // namespace BM188X
} // namespace onnc

#endif
