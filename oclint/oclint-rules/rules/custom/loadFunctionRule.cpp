#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class loadFunctionRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "loadFunctionRule";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "custom";
    }

    virtual void eachLine(int lineNumber, string line) override
    {

        string targetStr = "+ (void)load";
        string targetStr1 = "+(void)load";

        int currentLineSize = line.size();

        if(line.find(targetStr) != string::npos || line.find(targetStr1) != string::npos)
        {
            string description = "Not recommend rewrite the load method";
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

static RuleSet rules(new loadFunctionRule());

