
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TableViewAddSubviewRule : public AbstractASTVisitorRule<TableViewAddSubviewRule> {
private:
    bool isTableViewSelector(ObjCMessageExpr *objCMsgExpr, string &selectorString)
    {
        vector<string> dictGetSelectors;
        dictGetSelectors.push_back("addSubview:");
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
        return (type == "UITableViewCell" || superType == "UITableViewCell") &&
            vectorContains<string>(selectorString, dictGetSelectors);
        
    }
    
public:
    virtual const string name() const override
    {
        return "TableViewAddSubviewRule";
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
    
    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        string selectorString = objCMsgExpr->getSelector().getAsString();
 
        if (isTableViewSelector(objCMsgExpr, selectorString))
        {
            string description = "Prohibit directly add subView to tableView,should be add to contentView";
             addViolation(objCMsgExpr, this, description);
        }

        return true;
    }
    

};

static RuleSet rules(new TableViewAddSubviewRule());
