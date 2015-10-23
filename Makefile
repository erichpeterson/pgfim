#========================================
# USER RESERVED -- The following are reserved for users to set on the
# command line.  Makefiles should not set these.  These variables are
# for C/C++ compilation, and linking.
CFLAGS		=
JFLAGS		=
LDFLAGS		=

# OPTIMIZE with the -O option.  Override from the command line for
# building debug versions.
#
OPTFLAGS	= -O 

#========================================
# FILE EXTENSIONS.  Extensions and prefixes for different types of
# files change from platform to platform.  Hide these in macros so
# that we can more easily cut and paste between makefiles.
o		= .o
EXE_SFX		= 
SCRIPT_SFX 	= 
LIB_PFX		= lib
LIB_SFX		= .a
LIB_SHARED_SFX	= .so
TMPLIB		= libtemp.a

# FILE TOOLS
AR 	= ar qv
CHMOD 	= chmod
CP	= cp
GREP	= grep
MKDIR 	= mkdir
MUNCH 	= stepmunch
MV	= mv
NM 	= nm
RANLIB	= ranlib
RM 	= rm -f
RMDIR 	= rm -rf
STRIP	= strip
UNZIP 	= unzip
ZIP 	= zip


#========================================
# ANSI C Compile and Link
#
CC		= cc
CC_COMPILE	= $(CC) -c $(OPTFLAGS) $(CFLAGS) $(CC_CFLAGS) $(CC_SYSCFLAGS)
CC_LINK		= $(CC) $(LDFLAGS) $(CC_LDFLAGS) $(CC_SYSLDFLAGS)
CC_CFLAGS 	=
CC_LDFLAGS	=

# Global system things used for compilation, static linking, etc.
CC_SYSCFLAGS 	= 
CC_SYSLDFLAGS 	= 
CC_SYSLIBS	= 


#========================================
# C++ Compile and Link
#
CXX		= g++
CXX_COMPILE	= $(CXX) -c  $(OPTFLAGS) $(CFLAGS) $(CXX_CFLAGS) $(CXX_SYSCFLAGS)
CXX_LINK	= $(CXX) $(LDFLAGS) $(CXX_LDFLAGS) $(CXX_SYSLDFLAGS)
CXX_CFLAGS 	= -DTIXML_USE_STL -DHAVE_LIBARPREC -DHAVE_ARPREC_MP_REAL_H -I/usr/local/include
CXX_LDFLAGS	= -larprec

# The force flags are used for C/C++ compilers that select the
# language based on the file naming conventions.  Some C++ source
# may be in files with C naming conventions.
CXX_FORCE	= 

# System Flags -- Things for static linking or making sure that the
# compiler understands that a file is a C++ file or whatever.  These
# usually change from platform to platform.
CXX_SYSCFLAGS 	=
CXX_SYSLDFLAGS 	= 
CXX_SYSLIBS	= 


# Compilation Rules -- Repeat the rules for all of the different
# naming conventions.
#
.cxx.o:	; $(CXX_COMPILE) $<
.cpp.o:	; $(CXX_COMPILE) $<
.cc.o:	; $(CXX_COMPILE) $<
.C.o:	; $(CXX_COMPILE) $<

.cxx:	
	$(CXX_COMPILE) $<
	$(CXX_LINK) -o $@ $*.o $(LIBRARIES)
.cpp:	
	$(CXX_COMPILE) $<
	$(CXX_LINK) -o $@ $*.o $(LIBRARIES)
.cc:	
	$(CXX_COMPILE) $<
	$(CXX_LINK) -o $@ $*.o $(LIBRARIES)
.C:	
	$(CXX_COMPILE) $<
	$(CXX_LINK) -o $@ $*.o $(LIBRARIES)


# Compile plain C code
.c.o:	; $(CC_COMPILE) $<
.c:	
	$(CC_COMPILE) $<
	$(CC_LINK) -o $@ $*.o $(LIBRARIES)


#========================================
# JAVA Compile and Package
#
JAR		= jar
JAVADOC		= javadoc

JAVAC		= javac
JAVA_COMPILE	= $(JAVAC) $(JFLAGS) $(JAVA_CFLAGS)
JAVA_CFLAGS 	=

.java.class:	; $(JAVA_COMPILE) $<



#========================================
# ROSE Packages 
#
ROSE_CFLAGS	= -I$(ROSE_INCLUDE)
ROSE_LDFLAGS	= -L$(ROSE_LIB)
ROSE_LIBS	= -lrose

EXEC	= PGFIM
OBJECTS = \
	main$o \
	TaxVertex$o \
	Commandline$o \
	Taxonomy$o \
	Database$o \
	DynamicCalc$o \
	Mine$o \
	TreeVertex$o \
	EnumTree$o \
	Util$o \
	tinyxml$o \
	tinyxmlerror$o \
	tinyxmlparser$o \
	tinystr$o 


#========================================
# Standard Symbolic Targets
#
default: $(EXEC)
install: $(EXEC)

clean: 		; - $(RM) *.o $(EXECS)
very-clean: 	clean
spotless: 	very-clean

#========================================
# Executables and other targets
#
$(EXEC): $(OBJECTS)
	$(CXX_LINK) -o $@ $(OBJECTS) $(LIBRARIES)