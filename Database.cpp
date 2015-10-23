/* Copyright (c) 2011, 2012 Erich A. Peterson
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * The names of its contributors may not be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL ERICH A. PETERSON BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Database.h"

//---------------------------------------------------------------------
// Method:
//    Database::Database()
//
// Default Constructor
//---------------------------------------------------------------------
Database::Database() : G(Taxonomy()), numObjects(0), largestItem(0), numGeneralizedItems(0) { }

//---------------------------------------------------------------------
// Method:
//    Database::Database()
//
// Default Destructor
//---------------------------------------------------------------------
Database::~Database() { 
    for (int i = 0; i < largestItem + numGeneralizedItems + 1; i++) {
        if (P[i] != NULL) {
             delete [] P[i];
        }
    }
    delete [] P;
}

//---------------------------------------------------------------------
// Method:
//    Database::readFiles()
//
// Used to read in the DB file and XML taxomony.
//---------------------------------------------------------------------
void
Database::readFiles() {
    readXML();
    
    for (map<int, TaxVertex *>::iterator mapIt = headerTable.begin(); mapIt != headerTable.end(); mapIt++) {
        //cout << "Itemset: " << (*mapIt).first << endl;
        if (!(*mapIt).second->getChildren()->empty()) {
            findAllLeafNodes((*mapIt).second, (*mapIt).first);
        }
    }
    
    /*for (map<int, list<TaxVertex *> >::iterator mapIt = leafNodes.begin(); mapIt != leafNodes.end(); mapIt++) {
        if ((*mapIt).second.size() == 1) {
            numGeneralizedItems--;
        }
    }*/
    
    //cout << leafNodes.size() << endl;
    
    numGeneralizedItems = (unsigned int)leafNodes.size() - 1;
    
    readDBFromFile();
    
    //cout << "Num Gen. Itemsets: " << numGeneralizedItems << endl;
    
    /*map<int, list<TaxVertex *> >::iterator mapIt2;
    
    for (mapIt2 = leafNodes.begin(); mapIt2 != leafNodes.end(); mapIt2++) {
        cout << "Generalized Item: " << (*mapIt2).first << endl;
        map<int, TaxVertex *>::iterator tmpMap = headerTable.find((*mapIt2).first);
        
        list<TaxVertex *>::iterator nodeIt;
        for(nodeIt = (*mapIt2).second.begin(); nodeIt != (*mapIt2).second.end(); nodeIt++) {
            cout << "Item: " << (*nodeIt)->getItem() << endl;
            
        }
    }*/
}

//-------------------------------------------------------------------
// Method:
//    Database::readXML(const unsigned int & pass)
//
// Used to read in taxonomy of items from XML file.
//
// Arguments:
//    "pass" the pass number: 1 is the first pass to count the number
//           of generalized items in the taxonomy; 2 is to read in the
//           taxonomy into memory (Tree G)
//
//---------------------------------------------------------------------
void
Database::readXML() {
    TiXmlDocument doc(Commandline::inputXML);
    
	bool loadOkay = doc.LoadFile();
	if (loadOkay) {  
        readXMLHelper(&doc);
    } 
    else {
        Util::errorMsg("XML taxonomy file could not be loaded.\n");
        exit(1);
    }
}

//-------------------------------------------------------------------
// Method:
//    Database::readXMLHelper(TiXmlNode* pParent, const unsigned int & pass)
//
// Used as a helper to recursively read in taxonomy of items from XML file.
//
// Arguments:
//    "pParent" Current node in taxonomy
//    "pass" the pass number: 1 is the first pass to count the number
//           of generalized items in the taxonomy; 2 is to read in the
//           taxonomy into memory (Tree G)
//---------------------------------------------------------------------
void
Database::readXMLHelper(TiXmlNode* pParent) {
    if ( !pParent ) return;
    
	TiXmlNode* pChild;

	int t = pParent->Type();

	switch (t) {    
        case TiXmlNode::TINYXML_ELEMENT:
            if (pParent->ValueStr() == "vertex") {
                TiXmlAttribute * pAttrib = pParent->ToElement()->FirstAttribute();
                
                while (pAttrib) {
                    if (pAttrib->NameTStr() == "value") {
                        TaxVertex * newTaxVertex = new TaxVertex(atoi(pAttrib->Value()));
                        
                        if (pAttrib->ValueStr() == "-1") {
                            G.setRoot(newTaxVertex);
                            headerTable[atoi(pAttrib->Value())] = newTaxVertex;
                            break;
                        }
                        //numGeneralizedItems++;
                        //cout << pAttrib->Value() << endl;
                                                
                        headerTable[atoi(pAttrib->Value())] = newTaxVertex;
                        break;
                    }
                    pAttrib=pAttrib->Next();
                }
            }
            else if (pParent->ValueStr() == "edge") {
                TiXmlAttribute * pAttrib = pParent->ToElement()->FirstAttribute();
                
                TaxVertex * To;
                TaxVertex * From;
                
                while (pAttrib) {
                    if (pAttrib->NameTStr() == "to" || pAttrib->NameTStr() == "from") {
                        map<int, TaxVertex *>::iterator mapIt;
                        mapIt = headerTable.find(atoi(pAttrib->Value()));
                        //cout << pAttrib->ValueStr() << endl;
                        if (mapIt != headerTable.end()) {
                            if (pAttrib->NameTStr() == "to") {
                                To = (*mapIt).second;
                            }
                            else {
                                From = (*mapIt).second;
                            }
                        }
                    }
                    pAttrib=pAttrib->Next();
                }
                From->addChild(To);
            }
            break;
        default:
            break;
	}
    
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
		readXMLHelper(pChild);
	}
}

//----------------------------------------------------------------------
// Method:
//    Database::findAllLeafNodes(const int & item)
//
// Finds all leaf nodes for the given cNode and places them in the
// hash table leafNodes
//
// Arguments:
//     "cNode" the current node being pointed to
//     "item" the item for which all decedent nodes are being found
//----------------------------------------------------------------------
void
Database::findAllLeafNodes(TaxVertex * cVertex, const int & item) {
    if (!cVertex->getChildren()->empty()) {
        TaxVertex * tmpVertex;
        list<TaxVertex *>::iterator vertexIt;
        //cout << "Itemset: " << cVertex->getItemset().front() << endl;
        for (vertexIt = cVertex->getChildren()->begin(); vertexIt != cVertex->getChildren()->end(); vertexIt++) {
            //cout << "Itemset: " << (*vertexIt)->getItemset().front() << endl;
            tmpVertex = (*vertexIt);
            findAllLeafNodes(tmpVertex, item);
        }
    }
    else {
        map<int, list<TaxVertex *> >::iterator mapIt;
        mapIt = leafNodes.find(item);
        if (mapIt != leafNodes.end()) {
            list<TaxVertex *> listLeafs;
            listLeafs = (*mapIt).second;
            bool flag = true;
            for (list<TaxVertex *>::iterator leafIt = listLeafs.begin(); leafIt != listLeafs.end(); leafIt++) {
                if ((*leafIt)->getItem() == item) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                (*mapIt).second.push_back(cVertex);
            }
        }
        else {
            list<TaxVertex *> tmpList;
            tmpList.push_back(cVertex);
            leafNodes.insert(pair<int, list<TaxVertex *> >(item, tmpList));
        }
    }
}

//----------------------------------------------------------------------
// Method:
//    Database::readDBFromFile(const unsigned int & numGeneralizedItems)
//
// Used to read in contents of input file into an in memory representation
// of the database (P). It also populates C, a vector containing a
// list of unique singleton items.
//
// Arguments:
//     "numGeneralizedItems" the number of generalized items in the 
//                           taxonomy, read in from the frist pass
//----------------------------------------------------------------------
void
Database::readDBFromFile() {
	string line;
	string item;
	largestItem = 0;
	
	unsigned int objCount = 0;

	ifstream inputFile(Commandline::inputDB.c_str());
	
	if(!inputFile.is_open()) {
		Util::errorMsg("Could not open the input file.");
		exit(1);
	}
	
	while(!inputFile.eof()) {
		getline(inputFile, line);
		objCount++;
		
		istringstream ss(line);
		
		while (ss.good()) {
			ss >> item;
			
			if((unsigned int)atoi(item.c_str()) > largestItem) {
				largestItem = (unsigned int)atoi(item.c_str());
			}
		}
	}
	
	numObjects = objCount;
	
	inputFile.clear();
	inputFile.seekg(0, ios::beg);
	
	unsigned int * seenItem = new unsigned int[largestItem + 1];
    
	for(int i = 0; i < largestItem + 1; i++) {
		seenItem[i] = 0;
	}
    

	P = new FP*[largestItem + numGeneralizedItems + 2];
    
    for (unsigned int i = 0; i < largestItem + numGeneralizedItems + 2; i++) {
        P[i] = NULL;
    }
	
	for(int i = 0; i < numObjects && !inputFile.eof(); i++) {
		getline(inputFile, line);
		istringstream ss(line);
		
		while(ss.good()) {
			vector<string> splitResults;
			ss >> item;
			
			Util::stringSplit(item, ":", splitResults);
            			
			if(seenItem[(unsigned int)atoi(splitResults[0].c_str())] == 0) {
				
				seenItem[(unsigned int)atoi(splitResults[0].c_str())] = 1;				
			
				P[(unsigned int)atoi(splitResults[0].c_str())] = new FP[numObjects];
                
                for (unsigned int i = 0; i < numObjects; i++) {
//#ifdef HAVE_ARPREC_MP_REAL_H 
//#ifdef HAVE_LIBARPREC
//                    P[(unsigned int)atoi(splitResults[0].c_str())][i] = FP("0");
//#else
                    P[(unsigned int)atoi(splitResults[0].c_str())][i] = 0;
//#endif
//#endif
                    
                    
                }
			}

			
			P[(unsigned int)atoi(splitResults[0].c_str())][i] = atof(splitResults[1].c_str());
		}
	}

	
	delete[] seenItem;
}

//----------------------------------------------------------------------
// Method:
//    Database::getNumObjects()
//
// Returns the number of objects in the database.   
//----------------------------------------------------------------------

unsigned int 
Database::getNumObjects() {
	return numObjects;
}


//----------------------------------------------------------------------
// Method:
//    Database::getP()
//
// Returns a reference to the two-dimensional array P.   
//----------------------------------------------------------------------
FP **
Database::getP() {
	return P;
}

//----------------------------------------------------------------------
// Method:
//    Database::getLargestItem()
//
// Returns the largest item seen in the database.   
//----------------------------------------------------------------------
unsigned int
Database::getLargestItem() {
	return largestItem;
}

unsigned int
Database::getNumGeneralizedItems() {
    return numGeneralizedItems;
}

//----------------------------------------------------------------------
// Method:
//    Database::getG()
//
// Returns the taxonomy G.  
//----------------------------------------------------------------------
Taxonomy*
Database::getG() {
    return &G;
}

map<int, TaxVertex *>* 
Database::getHeaderTable() {
    return &headerTable;
}


map<int, list<TaxVertex *> >* 
Database::getLeafNodes() {
    return &leafNodes;
}




