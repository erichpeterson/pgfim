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

#include "Util.h"

//----------------------------------------------------------------------
// Method: 
//    Util::errorMsg(string msg)
// 
// Outputs error messages to the standard error output stream.
//
// Arguments:
//    "msg" is the message to be outputted.
//----------------------------------------------------------------------
void 
Util::errorMsg(string msg) {
	cerr << msg << endl;
}

//----------------------------------------------------------------------
// Method: 
//    Util::stringSplit(string str, string delim, vector<string>& results)
// 
// Splits a string into pieces, based on the desired delimiter, and
// places the resulting pieces in a vector.
//
// Arguments:
//    "str" is the string to be split.
//    "delim" is the delimiter used to determine on what to split the
//            string.
//    "results" is the vector into which the resuls are placed after
//            after the split.
//----------------------------------------------------------------------
void 
Util::stringSplit(string str, string delim, vector<string>& results) {
	unsigned long cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos )
	{
		if(cutAt > 0)
		{
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
	{
		results.push_back(str);
	}
}