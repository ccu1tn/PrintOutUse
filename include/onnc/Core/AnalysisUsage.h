//===- AnalysisUsage.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_ANALYSIS_USAGE_H
#define ONNC_CORE_ANALYSIS_USAGE_H
#include <onnc/Core/Pass.h>

#include <type_traits>
#include <vector>

namespace onnc {

/** \class AnalysisUsage
 *  \brief AnalysisUsage represents the analysis usage information of a pass.
 *
 *  An AnalysisUsage object analyzes that the pass REQUIRES (must ran before the
 *  pass runs).
 */
class AnalysisUsage
{
private:
  typedef std::vector<Pass::AnalysisID> IDList;

public:
  typedef IDList::iterator iterator;
  typedef IDList::const_iterator const_iterator;

public:
  AnalysisUsage() { }

  AnalysisUsage& addRequiredID(Pass::AnalysisID pID);

  template <
    typename PassType,
    typename = typename std::enable_if<
      std::is_base_of<Pass, PassType>::value
    >::type
  >
  AnalysisUsage& addRequired() {
    static_assert(std::is_default_constructible<PassType>::value, "passed type should be default constructible");

    return addRequiredID(PassType::id());
  }

  iterator begin() { return m_Required.begin(); }

  iterator end()   { return m_Required.end(); }

  const_iterator begin() const { return m_Required.begin(); }

  const_iterator end() const   { return m_Required.end(); }

  bool isEmpty() const { return m_Required.empty(); }

private:
  IDList m_Required;
};

} // namespace of onnc

#endif
