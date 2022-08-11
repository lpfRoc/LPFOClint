
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TableViewUpdatesRule : public AbstractASTVisitorRule<TableViewUpdatesRule> {
private:
    bool isTableViewSelector(ObjCMessageExpr *objCMsgExpr, string &selectorString)
    {
        vector<string> dictGetSelectors;
        dictGetSelectors.push_back("beginUpdates");
        dictGetSelectors.push_back("endUpdates");
        string superType = "";
        string type = "";
        ObjCInterfaceDecl *interface = objCMsgExpr->getReceiverInterface();
        if (interface) {
            type = interface->getNameAsString();
            ObjCInterfaceDecl *superInterface = interface->getSuperClass();
            if (superInterface) {
                superType = superInterface->getNameAsString();
            }
        }
        return (type == "UITableView" || superType == "UITableView") &&
            vectorContains<string>(selectorString, dictGetSelectors);
        
    }
    
public:
    virtual const string name() const override
    {
        return "TableViewUpdatesRule";
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
    
    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        string selectorString = objCMsgExpr->getSelector().getAsString();
 
        if (isTableViewSelector(objCMsgExpr, selectorString))
        {
            string description = "Not recommend use beginUpdates/endUpdates of tableView,it may cause crash";
             addViolation(objCMsgExpr, this, description);
        }

        return true;
    }
    

};

static RuleSet rules(new TableViewUpdatesRule());

