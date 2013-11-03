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

#ifndef FEAPP_MESH_HPP
#define FEAPP_MESH_HPP

#include <list>

#include "Teuchos_RCP.hpp"

#include "FEApp_AbstractElement.hpp"

namespace FEApp {

  class Mesh {
  public:

    typedef std::list< Teuchos::RCP<FEApp::AbstractElement> > MeshList;
    
    typedef MeshList::const_iterator const_iterator;

    typedef MeshList::iterator iterator;

    //! Constructor 
    Mesh();

    //! Destructor
    ~Mesh();

    //! Add a new element
    void addElement(const Teuchos::RCP<FEApp::AbstractElement>& element);

    //! Return number of elements
    unsigned int numElements() const;

    //! First element
    iterator begin();

    //! First element
    const_iterator begin() const;

    //! Last element
    iterator end();

    //! Last element
    const_iterator end() const;

  private:

    //! Private to prohibit copying
    Mesh(const Mesh&);

    //! Private to prohibit copying
    Mesh& operator=(const Mesh&);

  protected:

    //! List of elements
    MeshList elements;

  };

}

#endif // FEAPP_MESH_HPP
