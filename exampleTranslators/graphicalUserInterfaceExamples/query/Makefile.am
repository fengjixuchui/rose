include $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs

if ROSE_USE_QROSE
bin_PROGRAMS = query

query_SOURCES = query.cpp

# Use the names of the libraries as the list of dependences
query_DEPENDENCIES = $(ROSE_LIBS_WITH_PATH)

# Use names set in ROSE/config/Makefile.for.ROSE.includes.and.libs
AM_CPPFLAGS = $(ROSE_INCLUDES)
AM_LDFLAGS = $(ROSE_RPATHS)
LDADD    = $(ROSE_LIBS)


EXTRA_DIST = CMakeLists.txt $(TESTCODES)

CLEANFILES = 

clean-local:
	rm -rf Templates.DB ii_files ti_files rose_*.C

check-local:
	rm -f test_01.o
	$(MAKE) $(TEST_Objects)

TESTCODES =
#     test_01.cpp

# Build the list of object files
TEST_Objects = ${TESTCODES:.C=.o}

ROSE_FLAGS =

# This rule generates a dependency that forces the "make check" 
# test to always compile the test code
FORCE_TEST_CODES_TO_RUN:

$(TEST_Objects): $(TESTCODES) FORCE_TEST_CODES_TO_RUN query
	: remove this so that we can force the examples to be run
	rm -f $(TEST_Objects)
#	DQ (8/13/2005): I have commented out these test since they don't terminate 
#	automatically and as a result are difficult to handle with the "make check" rule.
	echo "testing of query GUI commented out since it won't terminate automatically"
#	./query -I$(srcdir) $(ROSE_FLAGS) -c $(srcdir)/../$(@:.o=.C)
	: remove this so that we can force the examples to be run
	rm -f $(TEST_Objects)

test: query
	./query -I$(srcdir) $(ROSE_FLAGS) -c $(srcdir)/../test_01.cpp
endif

