lib_LTLIBRARIES = libjavaJNI.la
libjavaJNI_la_SOURCES = JavaApp.cpp JavaApp.h
libjavaJNI_la_CPPFLAGS = -I$(top_srcdir)/opensaf/imm  $(SQLITE_CPPFLAGS) -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
libjavaJNI_la_LIBADD = $(SQLITE_LIBS) $(top_srcdir)/opensaf/imm/libimm.la 