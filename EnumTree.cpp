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

#include "EnumTree.h"

//----------------------------------------------------------------------
// Method: 
//    EnumTree::EnumTree()
// 
// Default constructor
//----------------------------------------------------------------------
EnumTree::EnumTree() {
    list<int> RootItem;
    RootItem.push_back(-1);
	Root = new TreeVertex(RootItem);
}

//----------------------------------------------------------------------
// Method: 
//    EnumTree::EnumTree(TreeVertex * Root)
// 
// Constructor used to initialize root of the Taxonomy class.
//----------------------------------------------------------------------
EnumTree::EnumTree(TreeVertex * Root) : Root(Root) {}

//----------------------------------------------------------------------
// Method: 
//    EnumTree::EnumTree()
// 
// Destructor used to destory all nodes in a given EnumTree object.
// 
//----------------------------------------------------------------------
EnumTree::~EnumTree() { }

//----------------------------------------------------------------------
// Method: 
//    Tree::FindLastChild(Node * node)
//
// Method used to find the right-most child of a given node.
//
// Arguments:
//    "node" pointer to the node one wishes to find the right-most
//           child of.
// 
// Return Value:
//    Node pointer to the right-most child.
//
//----------------------------------------------------------------------
TreeVertex *
EnumTree::FindLastChild(TreeVertex * vert) {
    TreeVertex * ct = NULL;
    
    if (vert->getLeftChild() == NULL) {
        return ct;
    }
    else
    {
        ct = vert->getLeftChild();
        while (ct->getRightSibling() != NULL) {
            ct = ct->getRightSibling();
        }
        
        return ct;
    }
}
