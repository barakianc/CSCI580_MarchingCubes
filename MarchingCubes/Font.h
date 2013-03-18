#ifndef __FONT_H__
#define __FONT_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <string>

using namespace std;

class Font
{
private:
	HDC hDC;
	unsigned int base;

public:
	Font()
	{
		hDC = NULL;
	}

	~Font()
	{
		KillFont();
	}

	void SetHDC(HDC h)
	{
		hDC = h;
	}

	void BuildFont(string fontName, int size)
	{
		HFONT font;
		HFONT oldFont;

		if(hDC == NULL)
			return;

		base = glGenLists(96);

		font = CreateFont(size,
						0,
						0,
						0,
						FW_NORMAL,
						FALSE,
						FALSE,
						FALSE,
						ANSI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						FF_DONTCARE | DEFAULT_PITCH,
						//fontName.c_str());
						"Arial");

		oldFont = (HFONT)SelectObject(hDC, font);
		wglUseFontBitmaps(hDC, 32, 96, base);
		SelectObject(hDC, oldFont);
		DeleteObject(font);
	}

	void KillFont()
	{
		glDeleteLists(base, 96);
	}

	void glPrint(const char* fmt, ...)
	{
		char text[256];
		va_list ap;

		if(fmt == NULL)
			return;

		va_start(ap, fmt);
			//vsprintf(text, fmt, ap);
			vsprintf_s(text, sizeof(char) * 256, fmt, ap);
		va_end(ap);

		glPushAttrib(GL_LIST_BIT);
		glListBase(base - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}

	int GetStringWidth(const char* str)
	{
		int length, width;

		length = strlen(str);

		GetCharWidth32(hDC, str[0], str[length], &width);

		return width;
	}
};

#endif