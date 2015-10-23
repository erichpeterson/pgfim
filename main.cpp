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

#include <iostream>
#include <sys/resource.h>
#include "Mine.h"
#include "Database.h"
#include "Commandline.h"

using namespace std;

int main (int argc, const char ** const argv)
{
    // Timing variables
	float  UserTime, SysTime;
	
	struct rusage MyTime1, MyTime2;
	
	// Start timing for building the tree
	getrusage(RUSAGE_SELF,&MyTime1);
    // Read in command line arguments
	if (Commandline::getArgs(argc, argv) != 0) {
		exit(1);
	}
    
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
    mp::mp_init(numDigitsPrecision1 + 2); 
    mp::mpsetprec(numDigitsPrecision1 ); 
    mp::mpsetoutputprec(numDigitsPrecision1 ); 
    cout.precision(numDigitsPrecision1 ) ; 
#endif	
#endif

    Database objDB;
    
    objDB.readFiles();
    
    Mine objMine(&objDB);
    
    objMine.start_mining();
    
#ifdef HAVE_ARPREC_MP_REAL_H 
#ifdef HAVE_LIBARPREC
    mp::mp_finalize();
#endif
#endif
    
    // End timing 
	getrusage(RUSAGE_SELF, &MyTime2);
	
	UserTime =
	((float) (MyTime2.ru_utime.tv_sec  - MyTime1.ru_utime.tv_sec)) +
	((float) (MyTime2.ru_utime.tv_usec - MyTime1.ru_utime.tv_usec)) * 1e-6;
	SysTime =
	((float) (MyTime2.ru_stime.tv_sec  - MyTime1.ru_stime.tv_sec)) +
	((float) (MyTime2.ru_stime.tv_usec - MyTime1.ru_stime.tv_usec)) * 1e-6;
    
    if(Commandline::exec != "") {
		ofstream execFile;
		execFile.open(Commandline::exec.c_str(), ios::app);
		
		if(!execFile.is_open()) {
			Util::errorMsg("Could not open the exec file.");
			exit(1);
		}
		
		execFile << UserTime + SysTime << "\t";
	}
    
    return 0;
}

