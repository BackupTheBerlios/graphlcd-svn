/*
 * GraphLCD skin library
 *
 * function.c  -  skin functions class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#include <syslog.h>

#include "skin.h"
#include "function.h"

namespace GLCD
{

static const char * Internals[] =
{
    "not", "and", "or", "equal", "gt",  "lt", "ge", "le", "ne", "file", "trans", NULL
};

cSkinFunction::cSkinFunction(cSkinObject *Parent)
:   mObject(Parent),
    mSkin(Parent->Skin()),
    mType(string),
    mString(mObject, false),
    mNumber(0),
    mNumParams(0)
{
}

cSkinFunction::cSkinFunction(const cSkinString & String)
:   mObject(String.Object()),
    mSkin(String.Skin()),
    mType(string),
    mString(String),
    mNumber(0),
    mNumParams(0)
{
}

cSkinFunction::cSkinFunction(const cSkinFunction & Src)
:   mObject(Src.mObject),
    mSkin(Src.mSkin),
    mType(Src.mType),
    mString(Src.mString),
    mNumber(Src.mNumber),
    mNumParams(Src.mNumParams)
{
    for (uint32_t i = 0; i < mNumParams; ++i)
        mParams[i] = new cSkinFunction(*Src.mParams[i]);
}

cSkinFunction::~cSkinFunction()
{
    for (uint32_t i = 0; i < mNumParams; ++i)
        delete mParams[i];
}

bool cSkinFunction::Parse(const std::string & Text)
{
    const char *text = Text.c_str();
    const char *ptr = text, *last = text;
    eType type = undefined_function;
    int inExpr = 0;
    cSkinFunction *expr;

    if (*ptr == '\'' || *ptr == '{')
    {
        // must be string
        if (strlen(ptr) < 2
            || (*ptr == '\'' && *(ptr + strlen(ptr) - 1) != '\'')
            || (*ptr == '{' && *(ptr + strlen(ptr) - 1) != '}'))
        {
            syslog(LOG_ERR, "ERROR: Unmatched string end\n");
            return false;
        }

        std::string temp;
        if (*ptr == '\'')
            temp.assign(ptr + 1, strlen(ptr) - 2);
        else
            temp.assign(ptr);

        int pos = -1;
        while ((pos = temp.find("\\'", pos + 1)) != -1)
            temp.replace(pos, 2, "'");

        if (!mString.Parse(temp))
            return false;

        mType = string;
    }
    else if ((*ptr >= '0' && *ptr <= '9') || *ptr == '-' || *ptr == '+')
    {
        // must be number
        char *end;
        int num = strtol(ptr, &end, 10);
        if (end == ptr || *end != '\0')
        {
            syslog(LOG_ERR, "ERROR: Invalid numeric value\n");
            return false;
        }

        mNumber = num;
        mType = number;
    }
    else
    {
        // expression
        for (; *ptr; ++ptr)
        {
            if (*ptr == '(')
            {
                if (inExpr++ == 0)
                {
                    int i;
                    for (i = 0; Internals[i] != NULL; ++i)
                    {
                        if ((size_t)(ptr - last) == strlen(Internals[i])
                            && memcmp(last, Internals[i], ptr - last) == 0)
                        {
                            type = (eType)(INTERNAL + i);
                            break;
                        }
                    }

                    if (Internals[i] == NULL)
                    {
                        syslog(LOG_ERR, "ERROR: Unknown function %.*s", (int)(ptr - last), last);
                        return false;
                    }
                    last = ptr + 1;
                }
            }
            else if (*ptr == ',' || *ptr == ')')
            {
                if (inExpr == 0)
                {
                    syslog(LOG_ERR, "ERROR: Unmatched '%c' in expression", *ptr);
                    return false;
                }

                if (inExpr == 1)
                {
                    expr = new cSkinFunction(mObject);
                    if (!expr->Parse(std::string(last, ptr - last)))
                    {
                        delete expr;
                        return false;
                    }

                    if (mNumParams == MAXPARAMETERS)
                    {
                        syslog(LOG_ERR, "ERROR: Too many parameters to function, maximum is %d",
                            MAXPARAMETERS);
                        return false;
                    }

                    mType = type;
                    mParams[mNumParams++] = expr;
                    last = ptr + 1;
                }

                if (*ptr == ')')
                {
                    if (inExpr == 1)
                    {
                        int params = 0;

                        switch (mType)
                        {
                            case fun_and:
                            case fun_or:
                                params = -1; break;

                            case fun_eq:
                            case fun_ne:
                            case fun_gt:
                            case fun_lt:
                            case fun_ge:
                            case fun_le:
                                ++params;
                            case fun_not:
                            case fun_trans:
                            case fun_file:
                                ++params;
                            default:
                                break;
                        }

                        if (params != -1 && mNumParams != (uint)params)
                        {
                            syslog(LOG_ERR, "ERROR: Text2Skin: Wrong number of parameters to %s, "
                                    "expecting %d", Internals[mType - INTERNAL], params);
                            return false;
                        }
                    }

                    --inExpr;
                }
            }
        }

        if (inExpr > 0)
        {
            syslog(LOG_ERR, "ERROR: Expecting ')' in expression");
            return false;
        }
    }

    return true;
}

cType cSkinFunction::FunFile(const cType & Param) const
{
    cImageCache * cache = mSkin->ImageCache();
    GLCD::cImage * image = cache->Get(Param);
    return image ? (cType) Param : (cType) false;
}

cType cSkinFunction::Evaluate(void) const
{
    switch (mType)
    {
        case string:
            return mString.Evaluate();

        case number:
            return mNumber;

        case fun_not:
            return !mParams[0]->Evaluate();

        case fun_and:
            for (uint i = 0; i < mNumParams; ++i)
            {
                if (!mParams[i]->Evaluate())
                    return false;
            }
            return true;

        case fun_or:
            for (uint i = 0; i < mNumParams; ++i)
            {
                if (mParams[i]->Evaluate())
                    return true;
            }
            return false;

        case fun_eq:
            return mParams[0]->Evaluate() == mParams[1]->Evaluate();

        case fun_ne:
            return mParams[0]->Evaluate() != mParams[1]->Evaluate();

        case fun_gt:
            return mParams[0]->Evaluate() >  mParams[1]->Evaluate();

        case fun_lt:
            return mParams[0]->Evaluate() <  mParams[1]->Evaluate();

        case fun_ge:
            return mParams[0]->Evaluate() >= mParams[1]->Evaluate();

        case fun_le:
            return mParams[0]->Evaluate() <= mParams[1]->Evaluate();

        case fun_file:
            return FunFile(mParams[0]->Evaluate());

        case fun_trans:
            return mSkin->Config().Translate(mParams[0]->Evaluate());

        default:
            //Dprintf("unknown function code\n");
            syslog(LOG_ERR, "ERROR: Unknown function code called (this shouldn't happen)");
            break;
    }
    return false;
}

} // end of namespace
