//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4GDMLSetup.cc,v 1.13 2007-12-18 10:28:49 ztorzsok Exp $
// GEANT4 tag $ Name:$
//
// class G4GDMLSetup Implementation
//
// Original author: Zoltan Torzsok, November 2007
//
// --------------------------------------------------------------------

#include "G4GDMLSetup.hh"

G4String G4GDMLSetup::getSetup(const G4String& ref) {

   if (setupMap.find(ref) == setupMap.end())
      G4Exception("GDML: Referenced setup '"+ref+"' was not found!");

   return setupMap[ref];
}

void G4GDMLSetup::setupRead(const xercesc::DOMElement* const element) {

   G4String name;

   const xercesc::DOMNamedNodeMap* const attributes = element->getAttributes();
   XMLSize_t attributeCount = attributes->getLength();

   for (XMLSize_t attribute_index=0;attribute_index<attributeCount;attribute_index++) {

      xercesc::DOMNode* attribute_node = attributes->item(attribute_index);

      if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE) continue;

      const xercesc::DOMAttr* const attribute = dynamic_cast<xercesc::DOMAttr*>(attribute_node);   

      const G4String attName  = xercesc::XMLString::transcode(attribute->getName());
      const G4String attValue = xercesc::XMLString::transcode(attribute->getValue());

      if (attName=="name") name = attValue;
   }

   for (xercesc::DOMNode* iter = element->getFirstChild();iter != 0;iter = iter->getNextSibling()) {

      if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;

      const xercesc::DOMElement* const child = dynamic_cast<xercesc::DOMElement*>(iter);

      const G4String tag = xercesc::XMLString::transcode(child->getTagName());

      if (tag == "world") setupMap[name] = GenerateName(refRead(child));
   }
}
