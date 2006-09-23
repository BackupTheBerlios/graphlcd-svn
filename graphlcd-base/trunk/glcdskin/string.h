/*
 * GraphLCD skin library
 *
 * string.h  -  skin string class
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * based on text2skin
 *
 */

#ifndef _GLCDSKIN_STRING_H_
#define _GLCDSKIN_STRING_H_

#include <string>
#include <vector>

#include "type.h"

namespace GLCD
{

enum eSkinAttrib
{
  aNone,
  aNumber,
  aString,
  aClean,
  aRest

#define __COUNT_ATTRIB__ (aRest + 1)
};

struct tSkinAttrib
{
  eSkinAttrib Type;
  std::string Text;
  int         Number;

  tSkinAttrib(const std::string & a): Type(aString), Text(a), Number(0) {}
  tSkinAttrib(int n): Type(aNumber), Text(""), Number(n) {}
  tSkinAttrib(eSkinAttrib t): Type(t), Text(""), Number(0) {}
  tSkinAttrib(void): Type(aNone), Text(""), Number(0) {}

  friend bool operator== (const tSkinAttrib & A, const tSkinAttrib & B);
  friend bool operator<  (const tSkinAttrib & A, const tSkinAttrib & B);
};

inline bool operator== (const tSkinAttrib & A, const tSkinAttrib & B)
{
  return A.Type == B.Type
      && A.Text == B.Text
      && A.Number == B.Number;
}

inline bool operator<  (const tSkinAttrib & A, const tSkinAttrib & B)
{
  return A.Type == B.Type
         ? A.Text == B.Text
         ? A.Number < B.Number
           : A.Text < B.Text
         : A.Type < B.Type;
}

struct tSkinToken
{
  std::string Name;
  uint        Offset;
  tSkinAttrib Attrib;
  int         Index;
  int         Tab;

  tSkinToken(void): Index(-1), Tab(-1) {}
  tSkinToken(std::string n, uint o, const std::string & a):
    Name(n), Offset(o), Attrib(a), Index(-1), Tab(-1) {}

  friend bool operator< (const tSkinToken & A, const tSkinToken & B);

  static std::string Token(const tSkinToken & Token);
};

inline bool operator< (const tSkinToken & A, const tSkinToken & B)
{
  return A.Name == B.Name
         ? A.Attrib == B.Attrib
           ? A.Index == B.Index
             ? A.Tab < B.Tab
             : A.Index < B.Index
           : A.Attrib < B.Attrib
         : A.Name < B.Name;
}

class cSkinObject;
class cSkin;

class cSkinString
{
private:
  typedef std::vector<cSkinString*> tStringList;
  static tStringList mStrings;

  cSkinObject *           mObject;
  cSkin *                 mSkin;
  std::string             mText;
  std::string             mOriginal;
  std::vector<tSkinToken> mTokens;
  bool                    mTranslate;

public:
  static void Reparse(void);

  cSkinString(cSkinObject *Parent, bool Translate);
  ~cSkinString();

  bool Parse(const std::string & Text, bool Translate = false);
  cType Evaluate(void) const;

  void SetListIndex(uint Index, int Tab);

  cSkinObject * Object(void) const { return mObject; }
  cSkin * Skin(void) const { return mSkin; }
};

inline void cSkinString::SetListIndex(uint Index, int Tab)
{
  for (uint i = 0; i < mTokens.size(); ++i)
  {
    mTokens[i].Index = Index;
    mTokens[i].Tab = Tab;
  }
}

} // end of namespace

#endif
