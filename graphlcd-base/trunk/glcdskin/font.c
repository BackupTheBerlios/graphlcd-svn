#include <syslog.h>

#include "font.h"
#include "skin.h"
#include "function.h"

namespace GLCD
{

cSkinFont::cSkinFont(cSkin * Parent)
:   mSkin(Parent),
    mCondition(NULL),
    mDummyDisplay(mSkin),
    mDummyObject(&mDummyDisplay)
{
}

bool cSkinFont::ParseUrl(const std::string & url)
{
    if (url.find("fnt:") == 0)
    {
        mType = ftFNT;
        if (url[4] == '/' || url.find("./") == 4 || url.find("../") == 4)
            mFile = url.substr(4);
        else
        {
            mFile = mSkin->Config().SkinPath();
            if (mFile.length() > 0)
            {
                if (mFile[mFile.length() - 1] != '/')
                    mFile += '/';
            }
            mFile += "fonts/";
            mFile += url.substr(4);
        }
        mSize = 0;
        return mFont.LoadFNT(mFile);
    }
    else if (url.find("ft2:") == 0)
    {
        mType = ftFT2;
        std::string::size_type pos = url.find(":", 4);
        if (pos == std::string::npos)
        {
            syslog(LOG_ERR, "cFontElement::Load(): No font size specified in %s\n", url.c_str());
            return false;
        }
        std::string tmp = url.substr(pos + 1);
        mSize = atoi(tmp.c_str());
        if (url[4] == '/' || url.find("./") == 4 || url.find("../") == 4)
            mFile = url.substr(4, pos - 4);
        else
        {
            mFile = mSkin->Config().SkinPath();
            if (mFile.length() > 0)
            {
                if (mFile[mFile.length() - 1] != '/')
                    mFile += '/';
            }
            mFile += "fonts/";
            mFile += url.substr(4, pos - 4);
        }
        return mFont.LoadFT2(mFile, mSkin->Config().CharSet(), mSize);
    }
    else
    {
        syslog(LOG_ERR, "cSkinFont::ParseUrl(): Unknown font type in %s\n", url.c_str());
        return false;
    }
}

bool cSkinFont::ParseCondition(const std::string & Text)
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

cSkinFonts::cSkinFonts(void)
{
}

cSkinFonts::~cSkinFonts()
{
    iterator it = begin();
    while (it != end())
    {
        delete (*it);
        it++;
    }
}

} // end of namespace
