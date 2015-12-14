bin_PROGRAMS = ios-genLocalization

CLANG_LIBS = \
                -lclangTooling\
                -lclangFrontendTool\
                -lclangFrontend\
                -lclangDriver\
                -lclangSerialization\
                -lclangCodeGen\
                -lclangParse\
                -lclangSema\
                -lclangStaticAnalyzerFrontend\
                -lclangStaticAnalyzerCheckers\
                -lclangStaticAnalyzerCore\
                -lclangAnalysis\
                -lclangARCMigrate\
                -lclangEdit\
                -lclangAST\
                -lclangLex\
                -lclangBasic

ios_genLocalization_CPPFLAGS =	\
	$(LLVM_CXX_FLAGS)	\
	-I$(top_srcdir)/libhelper	\
        $(PLIST_CFLAGS) \
	-fno-rtti

ios_genLocalization_LDADD =	\
        $(PLIST_LIBS) \
      	$(top_builddir)/libhelper/libhelper.la	\
       	$(CLANG_LIBS)	\
	$(LLVM_LIBS)

ios_genLocalization_SOURCES =	\
	main.cpp	\
       	getLocalizedStringFromFile.cpp

EXTRA_DIST = getLocalizedStringFromFile.h
