#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CollectionViewCellInitRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "CollectionViewCellInitRule";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "controversial";
    }

    virtual void eachLine(int lineNumber, string line) override
    {

        string targetStr = "[UICollectionViewCell new]";
        string targetStr1 = "[[UICollectionViewCell alloc] init]";

        int currentLineSize = line.size();

        if(line.find(targetStr) != string::npos || line.find(targetStr1) != string::npos)
        {
            string description = "Prohibit using [UICollectionViewCell new] or [[UICollectionViewCell alloc] init] to create UICollectionViewCell,Please use dequeueReusableCellWithReuseIdentifier:forIndexPath:";
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

static RuleSet rules(new CollectionViewCellInitRule());


