
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ContainsVersionMessageFetch : public RecursiveASTVisitor<ContainsVersionMessageFetch>
{
public:
    bool _available_10 = false;
    bool VisitObjCAvailabilityCheckExpr(ObjCAvailabilityCheckExpr* expr)
      {
          string _selector = expr->getVersion().getAsString();
          int version = atoi(_selector.c_str());
          if (version > 10) {
              _available_10 = false;
          } else {
              _available_10 = true;
          }
         return true;
      }
};

class ContainsReturnMessageFetch : public RecursiveASTVisitor<ContainsReturnMessageFetch>
{
private:
    template<typename nodeType>
    nodeType* extractFromImplicitCastExpr(Expr *fromExpr)
    {
        ImplicitCastExpr *implicitCastExpr = dyn_cast_or_null<ImplicitCastExpr>(fromExpr);
        if (implicitCastExpr)
        {
            return dyn_cast_or_null<nodeType>(implicitCastExpr->getSubExpr());
        }
        return nullptr;
    }
    
public:
    clang::SourceManager *sourceManager;
    vector<ReturnStmt*> _instances;
    bool available_10;
    int returnLineNum;//外部retur行号
    int inreturnLineNum;//内部retur行号
    bool violation = false;
    bool violation2 = false;
    
    void addParenExprToViolation(Expr *expr)
    {
        FloatingLiteral *trueFloating = dyn_cast_or_null<FloatingLiteral>(expr);
        FloatingLiteral *trueFloating1 = extractFromImplicitCastExpr<FloatingLiteral>(expr);
        IntegerLiteral *integerLiteral = extractFromImplicitCastExpr<IntegerLiteral>(expr);
        
        if (trueFloating || trueFloating1) {
            if (returnLineNum == 0 || returnLineNum > inreturnLineNum || (!violation && !available_10)) {
                double value;
                if (trueFloating) {
                    value = trueFloating->getValueAsApproximateDouble();
                } else {
                    value = trueFloating1->getValueAsApproximateDouble();
                }
                if (value > 1) {
                    violation2 = false;
                } else {
                    violation2 = true;
                }
            }
        } else if (integerLiteral) {
            if (returnLineNum == 0 || returnLineNum > inreturnLineNum || (!violation && !available_10)) {
                if (integerLiteral->getValue() == 1 || integerLiteral->getValue() == 0) {
                    violation2 = true;
                } else {
                    violation2 = false;
                }
            }
        }
    }
    bool VisitReturnStmt(ReturnStmt *returnStmt)
    {
        clang::SourceLocation startLocation = returnStmt->getBeginLoc();
        int startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        
        inreturnLineNum = startLineNumber;
        if (inreturnLineNum == returnLineNum) {
            if (violation) {
                _instances.push_back(returnStmt);
            }

        } else {
            addParenExprToViolation(returnStmt->getRetValue());
            if (violation2) {
                _instances.push_back(returnStmt);
            }
        }
        return true;
    }
    
    vector<ReturnStmt*>& getInstances() {
        return _instances;
    }
};

class ContainsIfReturnMessageFetch : public RecursiveASTVisitor<ContainsIfReturnMessageFetch>
{
private:
    template<typename nodeType>
    nodeType* extractFromImplicitCastExpr(Expr *fromExpr)
    {
        ImplicitCastExpr *implicitCastExpr = dyn_cast_or_null<ImplicitCastExpr>(fromExpr);
        if (implicitCastExpr)
        {
            return dyn_cast_or_null<nodeType>(implicitCastExpr->getSubExpr());
        }
        return nullptr;
    }
    
public:
    int returnLineNum = 0;
    clang::SourceManager *sourceManager;
    bool _available_10 = false;
    bool violation = false;

      void addParenExprToViolation(Expr *expr)
    {
        FloatingLiteral *trueFloating = dyn_cast_or_null<FloatingLiteral>(expr);
        FloatingLiteral *trueFloating1 = extractFromImplicitCastExpr<FloatingLiteral>(expr);
        IntegerLiteral *integerLiteral = extractFromImplicitCastExpr<IntegerLiteral>(expr);
        if (trueFloating || trueFloating1) {
            if (_available_10) { // iOS10.3  10.4要求rowHeight大于1
                double value;
                if (trueFloating) {
                    value = trueFloating->getValueAsApproximateDouble();
                } else {
                    value = trueFloating1->getValueAsApproximateDouble();
                }
                if (value > 1) {
                    violation = false;
                } else {
                    violation = true;
                }
            } else {
                violation = false;
            }
        } else if (integerLiteral) {
            if (_available_10) { // iOS10.3  10.4要求rowHeight大于1
                if (integerLiteral->getValue() == 1 || integerLiteral->getValue() == 0) {
                    violation = true;
                } else {
                    violation = false;
                }
            } else {
                violation = false;
            }
        }
    }
    bool VisitReturnStmt(ReturnStmt *returnStmt)
    {
        clang::SourceLocation startLocation = returnStmt->getBeginLoc();
        int startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        returnLineNum = startLineNumber;
        addParenExprToViolation(returnStmt->getRetValue());
          return true;
      }
};

class ContainsIfMessageFetch : public RecursiveASTVisitor<ContainsIfMessageFetch>
{
public:
    clang::SourceManager *sourceManager;
    int _returnLineNum = 0;
    bool _violation = false;//是否违反了规则
    bool _available_10 = false;
    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Expr *conditionExpr = ifStmt->getCond();
        ContainsVersionMessageFetch versionChecker;
        versionChecker.TraverseStmt(ifStmt);

        ContainsIfReturnMessageFetch ifReturnChecker;
        ifReturnChecker.sourceManager = sourceManager;
        ifReturnChecker._available_10 = versionChecker._available_10;
        ifReturnChecker.TraverseStmt(ifStmt);
        
        _available_10 = versionChecker._available_10;
        _violation = ifReturnChecker.violation;
        _returnLineNum = ifReturnChecker.returnLineNum;
        return true;
    }

};

class TableViewEstimatedHeightRule : public AbstractASTVisitorRule<TableViewEstimatedHeightRule> {

private:
    string currentClass;
public:
    virtual const string name() const override
    {
        return "TableViewEstimatedHeightRule";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "custom";
    }
    
    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }
    
    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
         clang::SourceManager *sourceManager = &_carrier->getSourceManager();
         string selectorName = decl->getSelector().getAsString();
         string targetStr = "tableView:estimatedHeightForRowAtIndexPath:";

         if (selectorName.find(targetStr) != string::npos) {
             
             ContainsIfMessageFetch ifChecker;
             ifChecker.sourceManager = sourceManager;
             ifChecker.TraverseDecl(decl);
             
             ContainsReturnMessageFetch returnChecker;
             returnChecker.sourceManager = sourceManager;
             returnChecker.available_10 = ifChecker._available_10;
             returnChecker.returnLineNum = ifChecker._returnLineNum;
             returnChecker.violation = ifChecker._violation;
             returnChecker.TraverseDecl(decl);
         
             for (auto& access : returnChecker.getInstances())
             {
                 string description = "iOS 10 table view row estimated height Less than 1 may cause crash";
                 addViolation(access, this, description);
             }
             
             return false;
         }
         return true;
         
     }

};

static RuleSet rules(new TableViewEstimatedHeightRule());

