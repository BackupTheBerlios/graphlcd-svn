/*
 * GraphLCD skin library
 *
 * object.h  -  skin object class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_OBJECT_H_
#define _GLCDSKIN_OBJECT_H_

#include <stdint.h>

#include <vector>
#include <string>
#include <map>

#include <glcdgraphics/bitmap.h>

#include "type.h"
#include "string.h"

namespace GLCD
{

class cSkin;
class cSkinDisplay;
class cSkinObjects;

struct tPoint
{
	int x, y;
	tPoint(int _x = 0, int _y = 0) { x = _x; y = _y; }
};

struct tSize
{
	int w, h;
	tSize(int _w = 0, int _h = 0) { w = _w; h = _h; }
};


class cSkinObject
{
	friend bool StartElem(const std::string & name, std::map<std::string,std::string> & attrs);
	friend bool CharData(const std::string & text);
	friend bool EndElem(const std::string & name);

public:
	enum eType
  {
    pixel,
    line,
		rectangle,
		ellipse,
		slope,
		image,
		text,
		marquee,
		blink,
		progress,
		scrolltext,
		scrollbar,
		block,
		list,
		item,
#define __COUNT_OBJECT__ (item + 1)
	};

private:
	cSkinDisplay * display;
	cSkin * skin;
	eType type;
	tPoint pos1;
	tPoint pos2;
  eColor color;
  bool filled;
  int radius;
  int arc;
  cSkinString path;

	cSkinObjects * objects; // used for block objects such as <list>

public:
	cSkinObject(cSkinDisplay * parent);
	cSkinObject(const cSkinObject & Src);
	~cSkinObject();

	bool ParseType     (const std::string &Text);
	bool ParseColor    (const std::string &Text);
	bool ParseCondition(const std::string &Text);
	bool ParseAlignment(const std::string &Text);
	bool ParseFontFace (const std::string &Text);

	void SetListIndex(uint Index, int Tab);

	eType Type(void) const { return type; }
	cSkinDisplay * Display(void) const { return display; }
	cSkin * Skin(void) const { return skin; }

	const std::string & TypeName(void) const;
	tPoint Pos(void) const;
	tSize Size(void) const;

	uint32_t NumObjects(void) const;
	const cSkinObject * GetObject(uint32_t Index) const;

  void Render(cBitmap * screen);
};

class cSkinObjects: public std::vector<cSkinObject *>
{
public:
	cSkinObjects(void);
	~cSkinObjects();
};

// recursive dependancy
inline uint32_t cSkinObject::NumObjects(void) const
{
	return objects ? objects->size() : 0;
}

inline const cSkinObject * cSkinObject::GetObject(uint32_t Index) const
{
	return objects ? (*objects)[Index] : NULL;
}

} // end of namespace

#endif
