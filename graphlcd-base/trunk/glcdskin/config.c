#include "config.h"
#include "type.h"

namespace GLCD
{

cSkinConfig::cSkinConfig(void)
:   skinPathFunc(NULL),
    charSetFunc(NULL)
{
}

cSkinConfig::cSkinConfig(const cSkinConfig & Config)
{
    skinPathFunc = Config.skinPathFunc;
    charSetFunc = Config.charSetFunc;
    translateFunc = Config.translateFunc;
    getTokenFunc = Config.getTokenFunc;
}

std::string cSkinConfig::SkinPath(void) const
{
    if (skinPathFunc)
        return skinPathFunc();
    return ".";
}

std::string cSkinConfig::CharSet(void) const
{
    if (charSetFunc)
        return charSetFunc();
    return "iso-8859-15";
}

std::string cSkinConfig::Translate(const std::string & Text) const
{
    if (translateFunc)
        return translateFunc(Text);
    return Text;
}

cType cSkinConfig::GetToken(const tSkinToken & Token) const
{
    if (getTokenFunc)
        return getTokenFunc(Token);
    return "";
}

void cSkinConfig::SetSkinPathFunc(tSkinPathFunc Function)
{
    skinPathFunc = Function;
}

void cSkinConfig::SetCharSetFunc(tCharSetFunc Function)
{
    charSetFunc = Function;
}

void cSkinConfig::SetTranslateFunc(tTranslateFunc Function)
{
    translateFunc = Function;
}

void cSkinConfig::SetGetTokenFunc(tGetTokenFunc Function)
{
    getTokenFunc = Function;
}

} // end of namespace
