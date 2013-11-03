// $Id$ 
// $Source$ 
// @HEADER
// ***********************************************************************
// 
//                           Sacado Package
//                 Copyright (2006) Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact David M. Gay (dmgay@sandia.gov) or Eric T. Phipps
// (etphipp@sandia.gov).
// 
// ***********************************************************************
// @HEADER

#ifndef FEAPP_ABSTRACTELEMENT_HPP
#define FEAPP_ABSTRACTELEMENT_HPP

#include <vector>

#include "FEApp_AbstractFunction.hpp"

namespace FEApp {

  /*!
   * \brief Abstract interface for representing a 1-D element
   */
  class AbstractElement {
  public:
  
    //! Default constructor
    AbstractElement() {};

    //! Destructor
    virtual ~AbstractElement() {};

    //! Get the number of nodes the element requires
    virtual unsigned int numNodes() const = 0;

    //! Create the nodes for this element
    virtual void createNodes(double x_left, double x_right,
			     unsigned int first_node_gid) = 0;

    //! Return GID of ith node
    virtual unsigned int nodeGID(unsigned int i) const = 0;
  
    //! Evaluate all shape functions at a set of points in (-1,1)
    virtual void 
    evaluateShapes(const std::vector<double>& xi,
		   std::vector< std::vector<double> >& phi) const = 0;

    //! Evaluate all shape function derivatives at a set of points in (-1,1)
    virtual void
    evaluateShapeDerivs(const std::vector<double>& xi,
			std::vector< std::vector<double> >& dphidxi) const = 0;

    /*! 
     * \brief Evaluate Jacobian of element transformation at a set of points 
     * in (-1,1)
     */
    virtual void 
    evaluateJacobian(const std::vector<double>& xi,
		     std::vector<double>& jac) const = 0;

    //! Map quadrature points to physical space
    virtual void
    evaluateQuadPoints(const std::vector<double>& xi,
		       std::vector<double>& x) const = 0;

  private:

    //! Private to prohibit copying
    AbstractElement(const AbstractElement&);
    
    //! Private to prohibit copying
    AbstractElement& operator=(const AbstractElement&);

  };

}

#endif // FEAPP_ABSTRACTELEMENT_HPP
