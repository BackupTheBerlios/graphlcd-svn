#include <syslog.h>

#include "variable.h"
#include "skin.h"
#include "function.h"

namespace GLCD
{

cSkinVariable::cSkinVariable(cSkin * Parent)
:   mSkin(Parent),
    mValue(0),
    mCondition(NULL),
    mDummyDisplay(mSkin),
    mDummyObject(&mDummyDisplay)
{
}

bool cSkinVariable::ParseValue(const std::string & Text)
{
    if (isalpha(Text[0]) || Text[0] == '#' || Text[0] == '\'')
    {
        cSkinFunction * func = new cSkinFunction(&mDummyObject);
        if (func->Parse(Text))
        {
            mValue = func->Evaluate();
            delete func;
            return true;
        }
        delete func;
    }
    char * e;
    const char * t = Text.c_str();
    long l = strtol(t, &e, 10);
    if (e == t || *e != '\0')
    {
      return false;
    }
    mValue = l;
    return true;
}

bool cSkinVariable::ParseCondition(const std::string & Text)
{
    cSkinFunction *result = new cSkinFunction(&mDummyObject);
    if (result->Parse(Text))
    {
        delete mCondition;
        mCondition = result;
        return true;
    }
    return false;
}

cSkinVariables::cSkinVariables(void)
{
}

cSkinVariables::~cSkinVariables()
{
    iterator it = begin();
    while (it != end())
    {
        delete (*it);
        it++;
    }
}

} // end of namespace
