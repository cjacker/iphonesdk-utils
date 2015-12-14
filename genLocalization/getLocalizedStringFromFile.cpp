#include <iostream>

#include "llvm/Config/llvm-config.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Host.h"

//#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "clang/Basic/LangOptions.h"
#include "clang/Basic/FileSystemOptions.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Basic/FileManager.h"

#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/FileSystemOptions.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Basic/FileManager.h"

#include "clang/Frontend/Utils.h"

#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/FrontendOptions.h"

#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/Builtins.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"
#include "clang/AST/Decl.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Ownership.h"
#include "clang/AST/DeclGroup.h"

#include "clang/Parse/Parser.h"

#include "clang/Parse/ParseAST.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/Utils.h"


std::vector<std::string> getLocalizedStringFromFile(std::string filename, std::string triple, std::vector<std::string> includepaths)
{
    clang::DiagnosticOptions diagnosticOptions;

    clang::TextDiagnosticPrinter *pTextDiagnosticPrinter =
        new clang::TextDiagnosticPrinter(
            llvm::outs(), &diagnosticOptions);

    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> pDiagIDs;
    clang::DiagnosticsEngine *pDiagnosticsEngine =
        new clang::DiagnosticsEngine(pDiagIDs,
            &diagnosticOptions,
            pTextDiagnosticPrinter);

    clang::LangOptions languageOptions;
    clang::FileSystemOptions fileSystemOptions;
    clang::FileManager fileManager(fileSystemOptions);

    clang::SourceManager sourceManager(
        *pDiagnosticsEngine,
        fileManager);


    const std::shared_ptr<clang::TargetOptions> targetOptions = std::make_shared<clang::TargetOptions>();
    targetOptions->Triple = triple;//"arm-apple-darwin11";//llvm::sys::getDefaultTargetTriple();

    clang::TargetInfo *pTargetInfo = 
        clang::TargetInfo::CreateTargetInfo(
            *pDiagnosticsEngine,
            targetOptions
            );
 
    llvm::IntrusiveRefCntPtr<clang::HeaderSearchOptions> headerSearchOptions(new clang::HeaderSearchOptions());
    for(unsigned long i = 0 ; i < includepaths.size(); i++) {
        headerSearchOptions->AddPath(includepaths.at(i),
                clang::frontend::Angled,
                false,
                false);
    }

    clang::HeaderSearch headerSearch(headerSearchOptions,
                                     sourceManager,
                                     *pDiagnosticsEngine,
                                     languageOptions,
                                     pTargetInfo);

/*    headerSearchOptions->AddPath("/usr/share/iPhoneOS5.0.sdk/usr/include/c++/4.2.1/tr1/",
            clang::frontend::Angled,
            false,
            false,
            false);
    headerSearchOptions->AddPath("/usr/share/iPhoneOS5.0.sdk/usr/include/c++/4.2.1",
            clang::frontend::Angled,
            false,
            false,
            false);
*/
    clang::CompilerInstance compInst;

    llvm::IntrusiveRefCntPtr<clang::PreprocessorOptions> pOpts (new clang::PreprocessorOptions());
    clang::Preprocessor preprocessor(
        pOpts,
        *pDiagnosticsEngine,
        languageOptions,
        sourceManager,
        headerSearch,
        compInst);

    preprocessor.Initialize(*pTargetInfo);

    clang::FrontendOptions frontendOptions;

    clang::RawPCHContainerReader Reader;

    clang::InitializePreprocessor(
        preprocessor,
        *pOpts,
        Reader,
        frontendOptions);

    clang::ApplyHeaderSearchOptions( headerSearch,//preprocessor.getHeaderSearchInfo(),
	*headerSearchOptions,//compInst.getHeaderSearchOpts(),
	preprocessor.getLangOpts(),
	preprocessor.getTargetInfo().getTriple());


    const clang::FileEntry *pFile = fileManager.getFile(filename);
    //sourceManager.setMainFileID( sourceManager.createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_System));
    sourceManager.setMainFileID( sourceManager.createFileID( pFile, clang::SourceLocation(), clang::SrcMgr::C_User));
    //sourceManager.createMainFileID(pFile);
    preprocessor.EnterMainSourceFile();
    pTextDiagnosticPrinter->BeginSourceFile(languageOptions, &preprocessor);

/*
 * identifier 'localizedStringForKey'
 * colon ':'
 * l_paren '('
 * unknown '@'
 * string_literal '"UIAlertView"'
 * r_paren ')'
 *
*/

    int found = 0;
    std::string trans_str;
    std::vector<std::string> trans_list;
    clang::Token token;
    do {
        preprocessor.Lex(token);
        if( pDiagnosticsEngine->hasErrorOccurred())
        {
            break;
        }
        // if we meet localizedStringForKey, that means we need record it params.
        if(token.is(clang::tok::identifier) && token.isNot(clang::tok::raw_identifier)) {
            const clang::IdentifierInfo *II = token.getIdentifierInfo();
            if ( II && (strcmp(II->getName().data(), "localizedStringForKey") == 0))
                found = 1;
        }

        // only string literal we should handle.
        if(found == 1 && token.is(clang::tok::string_literal)) {
               std::string got = preprocessor.getSpelling(token); //here, we got a string with "" 
               got = got.substr(1,got.length()-2); //remove ""
               trans_str += got; // string may split to multiline, we save them all.
        }
        // when match ), that means we got what we need.
        // push it to vector and wait next found == 1;
        if(found == 1 && token.is(clang::tok::r_paren) && token.isNot(clang::tok::raw_identifier)) {
            trans_list.push_back(trans_str);
            trans_str.clear();            
            found = 0;
        }
    } while( token.isNot(clang::tok::eof));
    pTextDiagnosticPrinter->EndSourceFile();

    /*for(int i = 0; i < trans_list.size(); i++)
        std::cout<<trans_list[i]<<std::endl;
    */
    return trans_list;
}
