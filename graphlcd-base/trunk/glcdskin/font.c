#include <syslog.h>

#include "font.h"
#include "skin.h"

namespace GLCD
{

cSkinFont::cSkinFont(cSkin * parent)
:   skin(parent)
{
}

bool cSkinFont::ParseUrl(const std::string & url)
{
    if (url.find("fnt:") == 0)
    {
        type = ftFNT;
        if (url[4] == '/' || url.find("./") == 4 || url.find("../") == 4)
            file = url.substr(4);
        else
        {
            file = skin->Config().SkinPath();
            if (file.length() > 0)
            {
                if (file[file.length() - 1] != '/')
                    file += '/';
            }
            file += "fonts/";
            file += url.substr(4);
        }
        size = 0;
        return font.LoadFNT(file);
    }
    else if (url.find("ft2:") == 0)
    {
        type = ftFT2;
        std::string::size_type pos = url.find(":", 4);
        if (pos == std::string::npos)
        {
            syslog(LOG_ERR, "cFontElement::Load(): No font size specified in %s\n", url.c_str());
            return false;
        }
        std::string tmp = url.substr(pos + 1);
        size = atoi(tmp.c_str());
        if (url[4] == '/' || url.find("./") == 4 || url.find("../") == 4)
            file = url.substr(4, pos - 4);
        else
        {
            file = skin->Config().SkinPath();
            if (file.length() > 0)
            {
                if (file[file.length() - 1] != '/')
                    file += '/';
            }
            file += "fonts/";
            file += url.substr(4, pos - 4);
        }
        return font.LoadFT2(file, skin->Config().CharSet(), size);
    }
    else
    {
        syslog(LOG_ERR, "cSkinFont::ParseUrl(): Unknown font type in %s\n", url.c_str());
        return false;
    }
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
