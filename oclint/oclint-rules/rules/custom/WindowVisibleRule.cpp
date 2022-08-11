#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class WindowVisibleRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "WindowVisibleRule";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "custom";
    }

    virtual void eachLine(int lineNumber, string line) override
    {

        string targetStr = "makeKeyAndVisible";
        string targetStr1 = "mainWindow makeKeyAndVisible";

        int currentLineSize = line.size();

        if(line.find(targetStr) != string::npos && line.find(targetStr1) == string::npos)
        {
            string description = "Prohibit using 'makeKeyAndVisible' to display window";
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

static RuleSet rules(new WindowVisibleRule());

