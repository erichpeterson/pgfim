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

#include "Mine.h"

//----------------------------------------------------------------------
// Method:
//    Mine::Mine(Database& objDB)
//
// Constructor to initialize a Mine object.
//
// Arguments:
//   "objDB" is the Database object used to initialize this object.
//----------------------------------------------------------------------
Mine::Mine(Database* objDB) : objDB(objDB), Tree(EnumTree()) { }

//----------------------------------------------------------------------
// Method:
//    Mine::start_mining()
//
// Main data mining algorithm.
//----------------------------------------------------------------------
void 
Mine::start_mining() {	
    IProbCalc *objProbCalc;
    
    if (Commandline::probCalcMethod == "" || Commandline::probCalcMethod == "dynamic" ) {
        objProbCalc = new DynamicCalc(objDB);
    }
    
    list<TaxVertex *>::iterator verIt;
    TreeVertex * lastTreeVert;
    
    for (verIt = objDB->getG()->getRoot()->getChildren()->begin(); verIt != objDB->getG()->getRoot()->getChildren()->end(); verIt++) {
        list<int> tmpList;
        tmpList.push_back((*verIt)->getItem());
        
        //cout << (*verIt)->getItem() << endl;
        FP tau = objProbCalc->ProbCalc(tmpList);
        
        if (tau >= Commandline::tau) {
            if (Commandline::print == 1) {
                cout << (*verIt)->getItem() << endl;
            }
            
            TreeVertex * newTreeVert = new TreeVertex(tmpList, Tree.Root);
            
            //cout << newTreeVert->getItemset().size() << endl;
            
            if (Tree.Root->getLeftChild() == NULL) {
                Tree.Root->setLeftChild(newTreeVert);
            }
            else {
                lastTreeVert->setRightSibling(newTreeVert);
            }
            lastTreeVert = newTreeVert;
        }        
    }
    SETExtend(Tree.Root, objProbCalc); 
    
    delete objProbCalc;
}

//----------------------------------------------------------------------
// Method:
//    Mine::SETExtend(EnumTree * cTreeVertex)
//
// Method used to extend the enumeration tree.
//----------------------------------------------------------------------
void
Mine::SETExtend(TreeVertex * cTreeVertex, IProbCalc* objProbCalc) {
    for (TreeVertex * tmpTreeVert = cTreeVertex->getLeftChild(); tmpTreeVert != NULL; tmpTreeVert = tmpTreeVert->getRightSibling()) {
        GenTaxChild(tmpTreeVert, objProbCalc);
        GenJoinChild(tmpTreeVert, objProbCalc);
        if (tmpTreeVert->getLeftChild() != NULL) {
            SETExtend(tmpTreeVert, objProbCalc);
        }
    }
}

void
Mine::GenTaxChild(TreeVertex * cTreeVertex, IProbCalc* objProbCalc) {
    TreeVertex * lastTreeVert;
    
    int lastItem = cTreeVertex->getItemset()->back();
    map<int, TaxVertex *>::iterator mapIt;
    mapIt = objDB->getHeaderTable()->find(lastItem);
    
    TaxVertex * lookupTreeVert = (*mapIt).second;
    
    /*for (list<int>::const_iterator itemIt = lookupNode->Itemset.begin(); itemIt != lookupNode->Itemset.end(); itemIt++) {
        cout << (*itemIt) << " " << endl;
    }*/
    
    list<TaxVertex *>::iterator lookupTaxVertIt;
    for (lookupTaxVertIt = lookupTreeVert->getChildren()->begin(); lookupTaxVertIt != lookupTreeVert->getChildren()->end(); lookupTaxVertIt++) {
        
        list<int>::iterator itemIt;
        
        itemIt = cTreeVertex->getItemset()->end();
        
       /* for (list<int>::iterator it = cTreeVertex->getItemset().begin(); it != cTreeVertex->getItemset().end();it++) {
            cout << (*it) << " " << endl;
        }*/
        
        list<int> newTreeVertItemset;
        
        //cout << cTreeVertex->getItemset().size() << endl;
        if (cTreeVertex->getItemset()->size() >= 2) {
            itemIt--;
            itemIt--;
            
            while (itemIt != cTreeVertex->getItemset()->begin()) {
                newTreeVertItemset.push_front((*itemIt));
                itemIt--;
            }
            newTreeVertItemset.push_front((*itemIt));
        }
        
        newTreeVertItemset.push_back((*lookupTaxVertIt)->getItem());
        
        FP tau = objProbCalc->ProbCalc(newTreeVertItemset);
        
        if (tau >= Commandline::tau) {
            if (Commandline::print == 1) {
                for (list<int>::iterator itemIt = newTreeVertItemset.begin(); itemIt != newTreeVertItemset.end(); itemIt++) {
                    cout << (*itemIt) << " ";
                }
                cout << endl;
            }
            
            TreeVertex * newTreeVert = new TreeVertex(newTreeVertItemset, cTreeVertex);
            
            /*for (list<int>::iterator it = newTreeVert->getItemset()->begin(); it != newTreeVert->getItemset()->end(); it++) {
                cout << (*it) << " ";
            }
            
            cout << endl;
            
            cout << newTreeVert->getItemset()->size() << endl;
             */
            
            if (cTreeVertex->getLeftChild() == NULL) {
                cTreeVertex->setLeftChild(newTreeVert);
            }
            else {
                lastTreeVert->setRightSibling(newTreeVert);
            }
            lastTreeVert = newTreeVert;
        }
    }
}

void
Mine::GenJoinChild(TreeVertex * cTreeVertex, IProbCalc* objProbCalc) {
    TreeVertex * lastChildVert = EnumTree::FindLastChild(cTreeVertex);
    
    for (TreeVertex * cVertexSiblings = cTreeVertex->getRightSibling(); cVertexSiblings != NULL; cVertexSiblings = cVertexSiblings->getRightSibling()) {
        list<int> newVertItemset;
        
        for (list<int>::iterator itemIt = cTreeVertex->getItemset()->begin(); itemIt != cTreeVertex->getItemset()->end(); itemIt++) {
            newVertItemset.push_back((*itemIt));
        }
    
        newVertItemset.push_back(cVertexSiblings->getItemset()->back());
            
        FP tau = objProbCalc->ProbCalc(newVertItemset);
        
        if (tau >= Commandline::tau) {
            if (Commandline::print == 1) {
                for (list<int>::iterator itemIt = newVertItemset.begin(); itemIt != newVertItemset.end(); itemIt++) {
                    cout << (*itemIt) << " ";
                }
                cout << endl;
            }
            
            TreeVertex * newTreeVert = new TreeVertex(newVertItemset, cTreeVertex);
            
            /*for (list<int>::iterator it = newTreeVert->getItemset()->begin(); it != newTreeVert->getItemset()->end(); it++) {
                cout << (*it) << " ";
            }
            
            cout << endl;
            
            cout << newTreeVert->getItemset()->size() << endl;
             */
            
            if (cTreeVertex->getLeftChild() == NULL) {
                cTreeVertex->setLeftChild(newTreeVert);
            }
            else {
                lastChildVert->setRightSibling(newTreeVert);
            }
            lastChildVert = newTreeVert;
        }
    }
}

