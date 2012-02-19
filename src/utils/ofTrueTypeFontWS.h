/* ------------------------------------------------------------------

Copyright (c) 2010 NISHIHARA Hiroaki

Creative Commons Attribution 3.0 Unported License
http://creativecommons.org/licenses/by/3.0/deed.en

------------------------------------------------------------------ */


/* ------------------------------------------------------------------

Copyright (c) 2004-2009 OFTeam

MIT License
http://www.openframeworks.cc/license


Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the ?oftware?, to deal in 
the Software without restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED ?S IS? WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------ */


#ifndef _OF_TTF_FONT_WS_H_
#define _OF_TTF_FONT_WS_H_

#include "ofTrueTypeFont.h"
#include <vector>

struct FT_LibraryRec_;
struct FT_FaceRec_;

class ofTrueTypeFontWS : ofTrueTypeFont {
public:
	ofTrueTypeFontWS();
	virtual ~ofTrueTypeFontWS();
  
  void loadFont(string filename, int fontsize,
                bool _bAntiAliased = true,
                bool makeContours = false);
  
	float stringWidth(wstring s);
	float stringHeight(wstring s);
	
	ofRectangle getStringBoundingBox(wstring s, float x, float y);
	
	void drawString(wstring s, float x, float y);
	void drawStringAsShapes(wstring s, float x, float y);
  
  int getLoadedCharsNum();
  
  const bool bFullCharacterSet;
  
private:
  typedef struct FT_LibraryRec_  *FT_Library;
  typedef struct FT_FaceRec_*  FT_Face;
  FT_Library library;
  FT_Face face;
  
  int getCharID(const wchar_t &c);
  void loadEachChar(int char_id);
  std::vector<int> loaded_chars;
  static const int TYPEFACE_UNLOADED;
  static const int NUM_MAX_CHARACTERS;
};

#endif
