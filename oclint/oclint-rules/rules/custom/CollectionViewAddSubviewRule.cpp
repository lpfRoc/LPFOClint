
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CollectionViewAddSubviewRule : public AbstractASTVisitorRule<CollectionViewAddSubviewRule> {
private:
    bool isCollectionSelector(ObjCMessageExpr *objCMsgExpr, string &selectorString)
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
        return (type == "UICollectionViewCell" || superType == "UICollectionViewCell") &&
            vectorContains<string>(selectorString, dictGetSelectors);
        
    }
    
public:
    virtual const string name() const override
    {
        return "CollectionViewAddSubviewRule";
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
 
        if (isCollectionSelector(objCMsgExpr, selectorString))
        {
            string description = "Not recommend directly add subView to collectionView,should be add to contentView";
             addViolation(objCMsgExpr, this, description);
        }

        return true;
    }
    

};

static RuleSet rules(new CollectionViewAddSubviewRule());
