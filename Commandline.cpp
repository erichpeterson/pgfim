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

#include "Commandline.h"

// Set static variables
double Commandline::tau = -1;
double Commandline::minsup = 1;
string Commandline::output;
string Commandline::inputDB;
string Commandline::inputXML;
string Commandline::exec;
unsigned int Commandline::print = 0;
string Commandline::probCalcMethod;

//----------------------------------------------------------------------
// Method 
//    Commandline::commandline()
//
// Used to print out legal command line arguments to screen. 
//----------------------------------------------------------------------
void 
Commandline::commandLine() {
	cerr << "Command Line Options:\n";
	cerr << "  -minsup minimum support threshold: [0.0-Size of DB] default: 1\n";
	cerr << "  -tau Tau frequent probability threshold: (0.0-1.0] (Required)\n";
	cerr << "  -inputDB Input DB file name (Required)\n";
    cerr << "  -inputXML Input XML taxonomy (Required)\n";
    cerr << "  -calc Frequentness calculation method: {dynamic} default: dynamic\n";
	cerr << "  -output Output file name\n";
	cerr << "  -exec Execution stats file name\n";
	cerr << "  -print Print found itemsets: {0 | 1} (0 = false, 1 = true) default 0\n";
}


//----------------------------------------------------------------------
// Method: 
//    Commandline::getArgs(int argc, char ** const argv)
// 
// Parses through the provided arguments and validates the values
//
// Arguments:
//    "argc" is the number of arguments passed at the command line.
//    "argv" is the char array of strings containing the actual command
//        line argument.
//
// Return value:
//    0 - Success
//    -1 - Failure
//----------------------------------------------------------------------
int 
Commandline::getArgs(int argc, const char ** const argv) {
	int argPos = 1;
	
	while (argPos < argc) {
		if (strcmp(argv[argPos], "-tau") == 0) {
			tau = atof(argv[++argPos]);
			argPos++;
			if (tau <= 0.0 || tau > 1.0) {
				Util::errorMsg("tau must be > 0 and <= 1\n");
				return -1;
			}
			continue;
		}
		else if (strcmp(argv[argPos], "-minsup") == 0) {
			minsup = atoi(argv[++argPos]);
			argPos++;
			if (minsup < 0) {
				Util::errorMsg("minsup must be >= 0\n");
				return -1;
			}
			continue;
		}
		else if (strcmp(argv[argPos], "-inputDB") == 0) {
			inputDB = argv[++argPos];
			argPos++;
			continue;
		}
        else if (strcmp(argv[argPos], "-inputXML") == 0) {
			inputXML = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-output") == 0) {
			output = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-exec") == 0) {
			exec = argv[++argPos];
			argPos++;
			continue;
		}
		else if (strcmp(argv[argPos], "-print") == 0) {
			print = atoi(argv[++argPos]);
			argPos++;
			continue;
		}
        else if(strcmp(argv[argPos], "-calc") == 0) {
            probCalcMethod = argv[++argPos];
            argPos++;
            continue;
        }
		else {
			commandLine();
			return -1;
		}
	}  // end while
	
	if (inputDB == "") {
		Util::errorMsg("-inputDB is required\n");
		return -1;
	}
    if (inputXML == "") {
        Util::errorMsg("inputXML is required\n");
        return -1;
    }
	if (tau == -1) {
		cout << "Please enter a tau value: ";
		cin >> tau;
		if(tau < 0.0 || tau > 1.0) {
			Util::errorMsg("-tau must be between 0 and 1\n"); 
			return -1;
		}
	}
	
	return 0;
}

