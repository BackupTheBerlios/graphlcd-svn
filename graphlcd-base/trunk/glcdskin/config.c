#include "config.h"
#include "type.h"

namespace GLCD
{

std::string cSkinConfig::SkinPath(void) const
{
    return ".";
}

std::string cSkinConfig::CharSet(void) const
{
    return "iso-8859-15";
}

std::string cSkinConfig::Translate(const std::string & Text) const
{
    return Text;
}

cType cSkinConfig::GetToken(const tSkinToken & Token) const
{
    return "";
}

int cSkinConfig::GetTokenId(const std::string & Name) const
{
    return 0;
}

} // end of namespace
