// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

#ifndef ROL_NEWOPTIMIZATIONSOLVER_DEF_HPP
#define ROL_NEWOPTIMIZATIONSOLVER_DEF_HPP

namespace ROL {

template<typename Real>
NewOptimizationSolver<Real>::NewOptimizationSolver( Ptr<NewOptimizationProblem<Real>> &opt,
                                                    ParameterList                     &list )
  : opt_(opt), problemType_(opt_->getProblemType()) {
  switch (problemType_) {
    case TYPE_U:  algoU_ = AlgorithmUFactory<Real>(list); break;
    case TYPE_B:  algoB_ = AlgorithmBFactory<Real>(list); break;
    case TYPE_E:  algoE_ = AlgorithmEFactory<Real>(list); break;
    case TYPE_EB: algoG_ = AlgorithmGFactory<Real>(list); break;
    case TYPE_LAST:
      ROL_TEST_FOR_EXCEPTION(true,std::invalid_argument,
        "Error in NewOptimizationSolver::solve() : Unsupported problem type");
  }
}

template<typename Real>
std::vector<std::string> NewOptimizationSolver<Real>::getOutput(void) const {
  return output_;
}

template<typename Real>
int NewOptimizationSolver<Real>::solve(const Ptr<StatusTest<Real>> &status,
                                       const bool combineStatus) {
  nullstream bhs;
  return solve(bhs,status,combineStatus);
}

template<typename Real>
int NewOptimizationSolver<Real>::solve( std::ostream &outStream,
                                        const Ptr<StatusTest<Real>> &status,
                                        const bool combineStatus ) {
  switch (problemType_) {
    case TYPE_U:
      if (status != nullPtr) algoU_->setStatusTest(status,combineStatus);
      output_ = algoU_->run(*opt_,outStream);
      break;
    case TYPE_B:
      if (status != nullPtr) algoB_->setStatusTest(status,combineStatus);
      output_ = algoB_->run(*opt_,outStream);
      break;
    case TYPE_E:
      if (status != nullPtr) algoE_->setStatusTest(status,combineStatus);
      output_ = algoE_->run(*opt_,outStream);
      break;
    case TYPE_EB:
      if (status != nullPtr) algoG_->setStatusTest(status,combineStatus);
      output_ = algoG_->run(*opt_,outStream);
      break;
    case TYPE_LAST:
      ROL_TEST_FOR_EXCEPTION(true,std::invalid_argument,
        "Error in NewOptimizationSolver::solve() : Unsupported problem type");
  }
  // TODO: Interrogate AlgorithmState and StatusTest to generate a return code
  //       that indicates why the solver has stopped

  // Return an integer code
  return 0;
}

template<typename Real>
Ptr<const AlgorithmState<Real>> NewOptimizationSolver<Real>::getAlgorithmState(void) const {
  switch (problemType_) {
    case TYPE_U:  return algoU_->getState();
    case TYPE_B:  return algoB_->getState();
    case TYPE_E:  return algoE_->getState();
    case TYPE_EB: return algoG_->getState();
    case TYPE_LAST:
      ROL_TEST_FOR_EXCEPTION(true,std::invalid_argument,
        "Error in NewOptimizationSolver::getAlgorithmState() : Unsupported problem type");
  }
}

template<typename Real>
void NewOptimizationSolver<Real>::reset(void) {
  switch (problemType_) {
    case TYPE_U:  algoU_->reset(); break;
    case TYPE_B:  algoB_->reset(); break;
    case TYPE_E:  algoE_->reset(); break;
    case TYPE_EB: algoG_->reset(); break;
    case TYPE_LAST:
      ROL_TEST_FOR_EXCEPTION(true,std::invalid_argument,
        "Error in NewOptimizationSolver::reset() : Unsupported problem type");
  }
}

} // namespace ROL

#endif // ROL_NEWOPTIMIZATIONSOLVER_DEF_HPP


