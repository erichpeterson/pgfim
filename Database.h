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

#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include "Commandline.h"
#include "Util.h"
#include "Global.h"
#include "Taxonomy.h"
#include "tinyxml.h"

using namespace std;

//----------------------------------------------------------------------
// Class:
//    Database
// 
// Used to represnet the database in memory
//----------------------------------------------------------------------

class Database {
private:
    Taxonomy G;                   // Taxonomy G
	FP ** P;                    // Probabilities for singletons
    map<int, TaxVertex *> headerTable; // Hash of item, which points to item in Taxonomy G
    map<int, list<TaxVertex *> > leafNodes; // Hash of a certain item's decendent leaf nodes
	unsigned int numObjects;    // Number of objects in the database
	unsigned int largestItem;   // Largest item found in the database
    unsigned int numGeneralizedItems;
    void findAllLeafNodes(TaxVertex *, const int &);
    void readDBFromFile();
    void readXML();
    void readXMLHelper(TiXmlNode*);
public:
    Database();
    ~Database();
    void readFiles();
	unsigned int getNumObjects();
    unsigned int getNumGeneralizedItems();
	unsigned int getLargestItem();
	FP ** getP();
    Taxonomy* getG();
    map<int, TaxVertex *>* getHeaderTable();	
    map<int, list<TaxVertex *> >* getLeafNodes();
};

#endif /*DATABASE_H*/
