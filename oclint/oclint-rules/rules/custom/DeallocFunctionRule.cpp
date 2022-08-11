
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"


using namespace std;
using namespace clang;
using namespace oclint;


class ContainsSetGetMessageFetch : public RecursiveASTVisitor<ContainsSetGetMessageFetch>
{
public:
    
    string currentClass;
    vector<string> _instances;
    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        Expr *receiverExpr = objCMsgExpr->getInstanceReceiver();
        string selectorString = objCMsgExpr->getSelector().getAsString();
        if (receiverExpr) {
            string type = receiverExpr->getType().getAsString();
            if (type.find(currentClass) != string::npos) {
                _instances.push_back(selectorString);
            }
        }

        return true;
    }
    
    vector<string>& getInstances() {
        return _instances;
    }

};

class ContainsPropertyFetch : public RecursiveASTVisitor<ContainsPropertyFetch>
{
public:

    vector<ObjCPropertyRefExpr*> _instances;

    bool VisitObjCPropertyRefExpr(ObjCPropertyRefExpr* ivarRef) {
        
        if (ivarRef->isMessagingGetter() || ivarRef->isMessagingSetter()) {

            _instances.push_back(ivarRef);
        }
        return true;
    }

    vector<ObjCPropertyRefExpr*>& getInstances() {
        return _instances;
    }

};


class DeallocFunctionRule : public AbstractASTVisitorRule<DeallocFunctionRule> {

private:
    string currentClass;
public:
    virtual const string name() const override
    {
        return "DeallocFunctionRule";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "custom";
    }
    
    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }
    
    
    bool VisitObjCImplementationDecl(ObjCImplementationDecl *decl)
    {
        currentClass =  decl->getNameAsString();
        return true;
    }
    
    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        string selectorName = decl->getSelector().getAsString();
        string targetStr = "dealloc";
        
        if (selectorName.find(targetStr) != string::npos) {
            
            ContainsSetGetMessageFetch messageChecker;
            messageChecker.currentClass = currentClass;
            messageChecker.TraverseDecl(decl);
            
            vector<string> checkerInstances = messageChecker.getInstances();
            
            ContainsPropertyFetch propertyChecker;
            propertyChecker.TraverseDecl(decl);
            
            for (auto& access : propertyChecker.getInstances())
            {
                if (!access->isExplicitProperty()) {
                    return false;
                }
                string ivarName = access->getExplicitProperty()->getNameAsString();
                string getName = access->getSetterSelector().getAsString();
                
                if (vectorContains<string>(ivarName, checkerInstances) || vectorContains<string>(getName, checkerInstances)) {
                    string description = "Prohibit using current object Setter/Getter(self.) in dealloc";
                    addViolation(access, this, description);
                }
                
            }
            return false;
        }
        return true;

    }
    

};

static RuleSet rules(new DeallocFunctionRule());

