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

#include "DynamicCalc.h"

DynamicCalc::DynamicCalc(Database* objDB) {
    this->objDB = objDB;
}

//----------------------------------------------------------------------
// Method:
//    DynamicCalc::ProbCalc(itemset & x, vector<itemset>& L)
//
// Used to calculate the maximum support of a given itemset x. As a side
// effect, it will set the cummulative probability values of x being 
// frequent in j transactions.
//
// Arguments:
//   "x" is the itemset of length k, which needs to have its maximum 
//       support calculated.
//   "L" is a reference to the frequent prob. itemsets of length k - 1.
//       This variable contains the cummulated probability values for
//       each k - 1 itemset.
//
// Return values:
//    The maximum support of the itemset x.
//----------------------------------------------------------------------

FP
DynamicCalc::ProbCalc(list<int> & Itemset) {
    // Check to see if there are any generalized items in Itemset
    // If so, calculate their prob. and populate P
    list<int>::const_iterator itemIt;
    
    map<int, TaxVertex *>::iterator mapIt;
    
    TaxVertex * tmpVertex;
    
    for (itemIt = Itemset.begin(); itemIt != Itemset.end(); itemIt++) {
        mapIt = objDB->getHeaderTable()->find((*itemIt));
        
        tmpVertex = (*mapIt).second;
        
        //cout << tmpVertex->getItem() << endl;
        
        //cout << !tmpVertex->getChildren()->size() << endl;
        
        if (!tmpVertex->getChildren()->empty() && objDB->getP()[tmpVertex->getItem()] == NULL) {
            populateGI(tmpVertex->getItem());
        }
         
    }
    
	FP * matrixA = new FP[objDB->getNumObjects() + 1];
	FP * matrixB;
	
    FP returnVal;
    
    FP prob;
    
	unsigned int i, j;
	for(i = 0; i <= Commandline::minsup; i++) {
		if(i != 0) {
			matrixB = new FP[objDB->getNumObjects() + 1];
		}
        
		for(j = i; j <= objDB->getNumObjects() - Commandline::minsup + i && j <= objDB->getNumObjects(); j++) {
			if(i == 0) {
				matrixA[j] = 1.0;
				continue;
			}
			if(j == i) {
				matrixB[j - 1] = 0.0;
			}
            
            prob = 1.0;
            
            for (list<int>::iterator itemIt = Itemset.begin(); itemIt != Itemset.end(); itemIt++) {
                //cout << (*itemIt) << endl;
                //cout << objDB->getP()[(*itemIt)][j - 1] << endl;
                prob *= objDB->getP()[(*itemIt)][j - 1];
                //cout << prob << endl;
            }
			
            //cout <<  matrixA[j - 1] * prob << endl;
			matrixB[j] = (matrixA[j - 1] * prob) + (matrixB[j - 1] * (1 - prob));
            //cout << matrixB[j] << endl;
            //cout << j << endl;
            
		}
        
		if(i != 0) {
			delete[] matrixA;
			matrixA = matrixB;
		}
		
		if(matrixA[j - 1] < Commandline::tau && i != 0) {
            returnVal = matrixA[j - 1];
			delete[] matrixA;
			return returnVal;
		}
	}
    
    returnVal = matrixA[objDB->getNumObjects()];
    
    //cout << returnVal << endl;
    
    delete[] matrixA;
	return returnVal;
}

void
DynamicCalc::populateGI(const unsigned int& Item) {
    //cout << Item << endl;
    map<int, list<TaxVertex *> >::iterator mapIt;
    mapIt = objDB->getLeafNodes()->find(Item);
    
    list<TaxVertex *>* nodeList = &(*mapIt).second;
    
    FP prob;
    
    objDB->getP()[Item] = new FP[objDB->getNumObjects()];
    
    for (unsigned int i = 0; i < objDB->getNumObjects(); i++) {
        objDB->getP()[Item][i] = 0;
    }
    
    for (unsigned int i = 0; i < objDB->getNumObjects(); i++) {
        prob = 1.0;
        bool flag = false;
        for (list<TaxVertex *>::iterator nodeIt = nodeList->begin(); nodeIt != nodeList->end(); nodeIt++) {
            //cout << (*nodeIt)->getItem() << endl;
            //cout << objDB->getP()[(*nodeIt)->getItem()][i] << endl;
            if (objDB->getP()[(*nodeIt)->getItem()][i] == 1.0) {
                flag = true;
                break;
            }
            else if (objDB->getP()[(*nodeIt)->getItem()][i] != 0) { 
                prob *= 1.0 - objDB->getP()[(*nodeIt)->getItem()][i];
            }
        }
        if (flag) {
            objDB->getP()[Item][i] = 1.0;
            continue;
        }
        
        if (prob == 1) {
            objDB->getP()[Item][i] = 0;
        }
        else {
            //cout << 1.0 - prob << endl;
            //cout << Item << endl;
            objDB->getP()[Item][i] = 1 - prob;
        }
    }
}

