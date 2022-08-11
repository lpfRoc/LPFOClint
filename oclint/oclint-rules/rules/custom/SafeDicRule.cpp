
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class SafeDicRule : public AbstractASTVisitorRule<SafeDicRule> {
private:
    bool isDictionaryGetSelector(Expr *expr, string &selectorString)
    {
        vector<string> dictGetSelectors;
        dictGetSelectors.push_back("objectForKey:");
        dictGetSelectors.push_back("objectForKeyedSubscript:");
        string type = expr->getType().getAsString();
        return (type.find("NSDictionary") != string::npos || type.find("NSMutableDictionary") != string::npos) &&
                 vectorContains<string>(selectorString, dictGetSelectors);
    }
    
public:
    virtual const string name() const override
    {
        return "SafeDicRule";
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

        Expr *receiverExpr = objCMsgExpr->getInstanceReceiver();
        string selectorString = objCMsgExpr->getSelector().getAsString();

        if (receiverExpr && isDictionaryGetSelector(receiverExpr, selectorString))
        {
            string description = "Please use a security dictionary to access";
             addViolation(objCMsgExpr, this, description);
        }

        return true;
    }
    

};

static RuleSet rules(new SafeDicRule());
