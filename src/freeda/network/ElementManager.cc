#include "Circuit.h"
#include "../network/ADInterface.h"
//#include "../elements/element_headers.h"
//#include "../inout/environment.h"

extern const char* freeda_version;
extern void sanityCheck();

ElementManager* ElementManager::em = NULL;

ElementManager::ElementManager()
{
  // Do nothing
}

ElementManager::~ElementManager()
{
  // Do nothing
}

// Only one element manager can exits.
ElementManager* ElementManager::getElementManager()
{
  if (!em)
    em = new ElementManager;
  return em;
}

Element* ElementManager::createElement(const string& elem_type,
const string& iname)
throw(string&)
{
  //  string instance_name = elem_type + ":" + iname;
  Element* new_elem = NULL;
//  // Find the type of element (code automatically generated).
//	#include "../elements/create_element.cc"

//  if (new_elem)
//    return new_elem;
//  else
//    throw("Element type " + elem_type + " does not exist.");
    return new_elem;
}
