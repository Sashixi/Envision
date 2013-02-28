#ifndef CLANGASTCONSUMER_H
#define CLANGASTCONSUMER_H

//ENVISION - CPPIMPORT
#include "clangastvisitor.h"

//ENVISION
#include "OOModel/src/allOOModelNodes.h"

//CLANG
#include "clang/AST/ASTConsumer.h"

class ClangAstConsumer : public clang::ASTConsumer
{
public:
    ClangAstConsumer(Model::Model* model,OOModel::Project *currentProject = nullptr);
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef D) override;

protected:
    ClangAstVisitor* astVisitor_;

};

#endif // CLANGASTCONSUMER_H
