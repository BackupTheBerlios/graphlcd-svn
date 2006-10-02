#include "display.h"
#include "object.h"
#include "skin.h"
#include "cache.h"

namespace GLCD
{

static const std::string ObjectNames[] =
{
    "pixel",
    "line",
    "rectangle",
    "ellipse",
    "slope",
    "image",
    "progress",
    "text",
    "scrolltext",
    "scrollbar",
    "block",
    "list",
    "item"
};

cSkinObject::cSkinObject(cSkinDisplay * Parent)
:   display(Parent),
    skin(Parent->Skin()),
    type((eType) __COUNT_OBJECT__),
    pos1(0, 0),
    pos2(-1, -1),
    color(GLCD::clrBlack),
    filled(false),
    radius(0),
    arc(0),
    mPath(this, false),
    mDirection(0),
    mCurrent(this, false),
    mTotal(this, false),
    objects(NULL)
{
}

cSkinObject::cSkinObject(const cSkinObject & Src)
:   display(Src.display),
    skin(Src.skin),
    type(Src.type),
    pos1(Src.pos1),
    pos2(Src.pos2),
    color(Src.color),
    filled(Src.filled),
    radius(Src.radius),
    arc(Src.arc),
    mPath(Src.mPath),
    mDirection(Src.mDirection),
    mCurrent(Src.mCurrent),
    mTotal(Src.mTotal),
    objects(NULL)
{
    if (Src.objects)
        objects = new cSkinObjects(*Src.objects);
}

cSkinObject::~cSkinObject()
{
    delete objects;
}

bool cSkinObject::ParseType(const std::string & Text)
{
    for (int i = 0; i < (int) __COUNT_OBJECT__; ++i)
    {
        if (ObjectNames[i] == Text)
        {
            type = (eType) i;
            return true;
        }
    }
    return false;
}

bool cSkinObject::ParseColor(const std::string & Text)
{
    if (Text == "white")
        color = GLCD::clrWhite;
    else if (Text == "black")
        color = GLCD::clrBlack;
    else
        return false;
    return true;
}

const std::string & cSkinObject::TypeName(void) const
{
    return ObjectNames[type];
}

tPoint cSkinObject::Pos(void) const
{
    return tPoint(pos1.x < 0 ? skin->BaseSize().w + pos1.x : pos1.x,
                  pos1.y < 0 ? skin->BaseSize().h + pos1.y : pos1.y);
}

tSize cSkinObject::Size(void) const
{
    tPoint p1(pos1.x < 0 ? skin->BaseSize().w + pos1.x : pos1.x,
              pos1.y < 0 ? skin->BaseSize().h + pos1.y : pos1.y);
    tPoint p2(pos2.x < 0 ? skin->BaseSize().w + pos2.x : pos2.x,
              pos2.y < 0 ? skin->BaseSize().h + pos2.y : pos2.y);
    return tSize(p2.x - p1.x + 1, p2.y - p1.y + 1);
}

void cSkinObject::Render(GLCD::cBitmap * screen)
{
    /*
    if (Object->Condition() != NULL && !Object->Condition()->Evaluate())
        return;
    */
    switch (Type())
    {
        case cSkinObject::image:
        {
            cImageCache * cache = skin->ImageCache();
            GLCD::cImage * image = cache->Get(mPath.Evaluate());
            if (image)
            {
                const GLCD::cBitmap * bitmap = image->GetBitmap();
                if (bitmap)
                {
                    screen->DrawBitmap(Pos().x, Pos().y, *bitmap, color);
                }
            }
            break;
        }

        case cSkinObject::text:
            //DrawText(Object->Pos(), Object->Size(), Object->Fg(), Object->Text(), Object->Font(),
            //    Object->Align());
            break;

        case cSkinObject::pixel:
            screen->DrawPixel(Pos().x, Pos().y, color);
            break;

        case cSkinObject::line:
        {
            int x1 = Pos().x;
            int x2 = Pos().x + Size().w - 1;
            int y1 = Pos().y;
            int y2 = Pos().y + Size().h - 1;
            if (x1 == x2)
                screen->DrawVLine(x1, y1, y2, color);
            else if (y1 == y2)
                screen->DrawHLine(x1, y1, x2, color);
            else
                screen->DrawLine(x1, y1, x2, y2, color);
            break;
        }

        case cSkinObject::rectangle:
            if (radius == 0)
                screen->DrawRectangle(Pos().x, Pos().y, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, filled);
            else
                screen->DrawRoundRectangle(Pos().x, Pos().y, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, filled, radius);
            break;

        case cSkinObject::ellipse:
            screen->DrawEllipse(Pos().x, Pos().y, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, filled, arc);
            break;

        case cSkinObject::slope:
            screen->DrawSlope(Pos().x, Pos().y, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, arc);
            break;

        case cSkinObject::progress:
        {
            int current = mCurrent.Evaluate();
            int total = mTotal.Evaluate();
            printf("current %d total %d\n", current, total);
            if (total == 0)
                total = 1;
            if (current > total)
                current = total;
            if (mDirection == 0)
            {
                int w = Size().w * current / total;
                if (w > 0)
                    screen->DrawRectangle(Pos().x, Pos().y, Pos().x + w - 1, Pos().y + Size().h - 1, color, true);
            }
            else if (mDirection == 1)
            {
                int h = Size().h * current / total;
                if (h > 0)
                    screen->DrawRectangle(Pos().x, Pos().y, Pos().x + Size().w - 1, Pos().y + h - 1, color, true);
            }
            else if (mDirection == 2)
            {
                int w = Size().w * current / total;
                if (w > 0)
                    screen->DrawRectangle(Pos().x + Size().w - w, Pos().y, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, true);
            }
            else if (mDirection == 3)
            {
                int h = Size().h * current / total;
                if (h > 0)
                    screen->DrawRectangle(Pos().x, Pos().y + Size().h - h, Pos().x + Size().w - 1, Pos().y + Size().h - 1, color, true);
            }
            break;
        }

        case cSkinObject::scrolltext:
            //DrawScrolltext(Object->Pos(), Object->Size(), Object->Fg(), Object->Text(), Object->Font(),
            //    Object->Align());
            break;

        case cSkinObject::scrollbar:
            //DrawScrollbar(Object->Pos(), Object->Size(), Object->Bg(), Object->Fg());
            break;

        case cSkinObject::block:
            //for (uint i = 0; i < Object->Objects(); ++i)
            //  DrawObject(Object->GetObject(i));
            break;

        case cSkinObject::list:
        {
#if 0
            Dprintf("list\n");
            const cxObject *item = Object->GetObject(0);
            if (item && item->Type() == cxObject::item)
            {
                txSize areasize = Object->Size();
                uint itemheight = item->Size().h;
                uint maxitems = areasize.h / itemheight;
                uint yoffset = 0;

                SetMaxItems(maxitems); //Dprintf("setmaxitems %d\n", maxitems);
                for (uint i = 0; i < maxitems; ++i, yoffset += itemheight)
                {
                    for (uint j = 1; j < Object->Objects(); ++j)
                    {
                        const cxObject *o = Object->GetObject(j);
                        int maxtabs = 1;

                        if (o->Display()->Type() == cxDisplay::menu)
                            maxtabs = cSkinDisplayMenu::MaxTabs;

                        for (int t = -1; t < maxtabs; ++t)
                        {
                            if (!HasTabText(i, t))
                                continue;

                            int thistab = GetTab(t);
                            int nexttab = GetTab(t + 1);

                            cxObject obj(*o);
                            obj.SetListIndex(i, t);
                            if (obj.Condition() != NULL && !obj.Condition()->Evaluate())
                                continue;

                            obj.mPos1.x += Object->mPos1.x + (t >= 0 ? thistab : 0);
                            obj.mPos1.y += Object->mPos1.y + yoffset;

                            // get end position
                            if (t >= 0 && nexttab > 0)
                            {
                                // there is a "next tab".. see if it contains text
                                int n = t + 1;
                                while (n < cSkinDisplayMenu::MaxTabs && !HasTabText(i, n))
                                    ++n;
                                nexttab = GetTab(n);
                            }

                            if (t >= 0 && nexttab > 0 && nexttab < obj.mPos1.x + obj.Size().w - 1)
                                // there is a "next tab" with text
                                obj.mPos2.x = Object->mPos1.x + o->mPos1.x + nexttab;
                            else
                            {
                                // there is no "next tab", use the rightmost edge
                                obj.mPos2.x += Object->mPos1.x;
                                SetEditableWidth(obj.Size().w);
                            }

                            obj.mPos2.y += Object->mPos1.y + yoffset;

                            std::string text = obj.Text();
                            bool isprogress = false;
                            if (text.length() > 5 && text[0] == '[' && text[text.length() - 1] == ']')
                            {
                                const char *p = text.c_str() + 1;
                                isprogress = true;
                                for (; *p != ']'; ++p)
                                {
                                    if (*p != ' ' && *p != '|')
                                    {
                                        isprogress = false;
                                        break;
                                    }
                                }
                            }

                            if (isprogress)
                            {
                                //Dprintf("detected progress bar tab\n");
                                if (obj.Condition() == NULL || obj.Condition()->Evaluate())
                                {
                                    int total = text.length() - 2;
                                    int current = 0;
                                    const char *p = text.c_str() + 1;
                                    while (*p == '|')
                                        (++current, ++p);

                                    txPoint pos = obj.Pos();
                                    txSize size = obj.Size();

                                    DrawRectangle(txPoint(pos.x, pos.y + 4),
                                                  txSize(size.w, 2), obj.Fg());
                                    DrawRectangle(txPoint(pos.x, pos.y + 4),
                                                  txSize(2, size.h - 8), obj.Fg());
                                    DrawRectangle(txPoint(pos.x, pos.y + size.h - 6),
                                                  txSize(size.w, 2), obj.Fg());
                                    DrawRectangle(txPoint(pos.x + size.w - 2, pos.y + 4),
                                                  txSize(2, size.h - 8), obj.Fg());

                                    pos.x += 4;
                                    pos.y += 8;
                                    size.w -= 8;
                                    size.h -= 16;
                                    DrawProgressbar(pos, size, current, total, obj.Bg(),
                                                    obj.Fg(), NULL, NULL, NULL, NULL);
                                }
                            }
                            else
                                DrawObject(&obj);
                        }
                    }
                }
            }
#endif
            break;
        }

        case cSkinObject::item:
            // ignore
            break;
    }
}

cSkinObjects::cSkinObjects(void)
{
}

cSkinObjects::~cSkinObjects()
{
    for (uint32_t i = 0; i < size(); i++)
        delete operator[](i);
}

} // end of namespace

