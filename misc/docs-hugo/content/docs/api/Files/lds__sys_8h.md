---
title: ldsCtrlEst_h/lds_sys.h
summary: LDS base type. 

---

# ldsCtrlEst_h/lds_sys.h

LDS base type.  [More...](#detailed-description)



## Namespaces

| Name           |
| -------------- |
| **[lds](/ldsctrlest/docs/api/namespaces/namespacelds/)** <br>Linear Dynamical Systems (LDS) namespace.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)** <br>Linear Dynamical System Type.  |

## Detailed Description



This file declares and partially defines the base type for linear dynamical systems (`[lds::sys_t](/ldsctrlest/docs/api/classes/classlds_1_1sys__t/)`). Note that this class defines the underlying linear dynamics, but does not have output functions.Gaussian- and Poisson-output variants will be built upon this class. 





## Source code

```cpp
//===-- ldsCtrlEst_h/lds_sys.h - LDS ----------------------------*- C++ -*-===//
//
// Copyright 2021 [name of copyright owner]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//

#ifndef LDSCTRLEST_LDS_SYS_H
#define LDSCTRLEST_LDS_SYS_H

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {

class sys_t {
 public:
  sys_t(std::size_t nU, std::size_t nX, data_t& dt, data_t& p0 = DEFAULT_P0,
        data_t& q0 = DEFAULT_Q0);
  sys_t& operator=(const sys_t& sys);

  void simPredict();

  size_t getNx() const { return nX; };

  size_t getNu() const { return nU; };

  armaVec getU() const { return u; };

  armaVec getX() { return x; };

  armaVec getG() const { return g; };

  armaVec getM() const { return m; };

  armaMat getA() const { return A; };

  armaMat getB() const { return B; };

  armaMat getQ() const { return Q; };

  armaMat getQ_m() const { return Q_m; };

  armaMat getP() const { return P; };

  armaMat getP_m() const { return P_m; };

  armaVec getX0() const { return x0; };

  armaMat getP0() const { return P0; };

  armaVec getM0() const { return m0; };

  armaMat getP0_m() const { return P0_m; };

  void setDims(std::size_t& nU, std::size_t& nX);

  void setU(stdVec& uVec);
  void setU(armaVec& u);

  void setA(stdVec& aVec);
  void setA(armaMat& A);

  void setB(stdVec& bVec);
  void setB(armaMat& B);

  void setM(stdVec& mVec);
  void setM(armaVec& m);

  void setG(stdVec& gVec);
  void setG(armaVec& g);

  void setQ(stdVec& qVec);
  void setQ(armaMat& Q);

  void setQ_m(stdVec& qmVec);
  void setQ_m(armaMat& Q_m);

  void setX0(stdVec& x0Vec);
  void setX0(armaVec& x0);

  void setP0(stdVec& p0Vec);
  void setP0(armaMat& P0);

  void setP0_m(stdVec& p0mVec);
  void setP0_m(armaMat& P0_m);

  void reset();

  void printSys();

  bool adaptM;

 protected:
  armaVec u;    
  armaVec x;    
  armaMat P;    
  armaVec m;    
  armaMat P_m;  

  // Parameters:
  armaVec x0;    
  armaMat P0;    
  armaVec m0;    
  armaMat P0_m;  

  armaMat A;    
  armaMat B;    
  armaVec g;    
  armaMat Q;    
  armaMat Q_m;  

  // it should be safe for dt to be a reference. I should not need to control
  // what the set behavior is.
  data_t& dt;  
  data_t& q0;  
  data_t& p0;  

  // dimensions
  std::size_t nX;  
  std::size_t nU;  
  bool szChanged;  

  // max val for elements of P before reset for numerical reasons...
  const data_t plim = 1e2;  

  void predict();

  // TODO(mfbolus): these are very redundant.
  // Should be able to use templates in some way to make this less type-specific
  void reassign(armaVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, stdVec& newVar, data_t defaultVal = 0);

  void limit(stdVec& x, data_t& lb, data_t& ub);
  void limit(armaVec& x, data_t& lb, data_t& ub);
  void limit(armaMat& x, data_t& lb, data_t& ub);

  void defaultQ();

  void checkP();
};  // sys_t
}  // namespace lds

#endif
```


-------------------------------

Updated on  3 March 2021 at 23:06:12 CST
