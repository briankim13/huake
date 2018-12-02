#include <stdio.h>
#include <time.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include "fssimplewindow.h"
#include "ysglfontdata.h"

char *MyFgets(char str[],int maxn,FILE *fp)
{
	auto r=fgets(str,maxn,fp);
	if(nullptr!=r)
	{
		for(int i=strlen(str)-1; 0<=i; --i)
		{
			if(str[i]<' ')
			{
				str[i]=0;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		str[0]=0;
	}
	return r;
}

int ParseString(int wordTop[],int wordLen[],int maxlen,char input[])
{
	if(0==maxlen)
	{
		return 0;
	}

	int state=0;
	int wordCount=0;
	for(int i=0; 0!=input[i]; ++i)
	{
		if(0==state)
		{
			if(' '<input[i])
			{
				wordTop[wordCount]=i;
				wordLen[wordCount]=1;
				state=1;
				++wordCount;
			}
		}
		else if(1==state)
		{
			if(input[i]<=' ')
			{
				state=0;
				if(maxlen<=wordCount)
				{
					break;
				}
			}
			else
			{
				++wordLen[wordCount-1];
			}
		}
	}

	return wordCount;
}

void ExtractWord(char word[],int maxlen,char input[],int top,int len)
{
	int nChar;
	if(maxlen-1>len)
	{
		nChar=len;
	}
	else
	{
		nChar=maxlen-1;
	}

	for(int i=0; i<nChar; ++i)
	{
		word[i]=input[top+i];
	}
	word[nChar]=0;
}



class Data
{
public:
	char * txtName;
	char * txtScore;
	int * statePtr;
};



template <class T>
class GenericArray
{
private:
	int len;
	T *dat;
public:
	GenericArray();
	~GenericArray();
	void CleanUp(void);

	void Resize(int newLen);

	int GetLength(void) const;
	T *GetPointer(void);
	const T *GetPointer(void) const;
};

template <class T>
GenericArray<T>::GenericArray()
{
	len=0;
	dat=nullptr;
}
template <class T>
GenericArray<T>::~GenericArray()
{
	CleanUp();
}
template <class T>
void GenericArray<T>::CleanUp(void)
{
	if(nullptr!=dat)
	{
		delete [] dat;
	}
	dat=nullptr;
	len=0;
}

template <class T>
void GenericArray<T>::Resize(int newLen)
{
	auto newDat=new T [newLen];

	for(int i=0; i<len && i<newLen; ++i)
	{
		newDat[i]=dat[i];
	}

	CleanUp();

	dat=newDat;
	len=newLen;
}

template <class T>
int GenericArray<T>::GetLength(void) const
{
	return len;
}

template <class T>
T *GenericArray<T>::GetPointer(void)
{
	return dat;
}
template <class T>
const T *GenericArray<T>::GetPointer(void) const
{
	return dat;
}



class TextString : public GenericArray <char>
{
public:
    TextString();
    TextString(const char from[]);
    TextString(const TextString &str);
    ~TextString();

    void Set(const char from[]);
    void Add(const char from[]);
    void Add(const char c);
    void BackSpace(void);
    const char *Fgets(FILE *fp);
    void DeleteLastControlCode(void);
    const char *GetPointer(void) const;
    const int Strlen(void) const;
    void Print(void) const;
    TextString &operator=(const TextString &from);
};

TextString::TextString()
{
}

TextString::TextString(const TextString &incoming)
{
	Set(incoming.GetPointer());
}
TextString::TextString(const char from[])
{
	Set(from);
}

TextString::~TextString()
{
	CleanUp();
}

void TextString::Set(const char from[])
{
	auto str=GenericArray::GetPointer();
	if(from!=str)
	{
		CleanUp();
		if(nullptr!=from)
		{
			int l=strlen(from);
			Resize(l+1);
			str=GenericArray::GetPointer();
			strcpy(str,from);
		}
	}
}

void TextString::Add(const char from[])
{
	for(int i=0; 0!=from[i]; ++i)
	{
		Add(from[i]);
	}
}

void TextString::Add(const char c)
{
	// If l=1:
	// str[0]  str[1]
	//  'a'      0
	// newStr[0]   newStr[1]   newStr[2]
	//   'a'           c          0

	int l=Strlen();

	Resize(l+1+1);

	auto str=GenericArray::GetPointer();
	str[l  ]=c;
	str[l+1]=0;
} 

void TextString::BackSpace(void)
{
	if(0<Strlen())
	{
		int l=Strlen();
		Resize(l);
		auto str=GenericArray::GetPointer();
		str[l-1]=0;
	}
}

const char *TextString::Fgets(FILE *fp)
{
	CleanUp();

	char s[16];
	bool eof=true;
	while(nullptr!=fgets(s,15,fp))
	{
		eof=false;
		for(int i=0; s[i]!=0; ++i)
		{
			if('\n'==s[i])
			{
				return GetPointer();
			}
			else
			{
				Add(s[i]);
			}
		}
	}

	if(eof==true)
	{
		return nullptr;
	}
	return GetPointer();
}

//void TextString::DeleteLastControlCode(void);

const char *TextString::GetPointer(void) const
{
	auto str=GenericArray::GetPointer();
	if(nullptr==str)
	{
		return "";
	}
	else
	{
		return str;
	}
}

const int TextString::Strlen(void) const
{
	auto str=GenericArray::GetPointer();
	if(nullptr==str)
	{
		return 0;
	}
	else
	{
		return strlen(str);
	}
}

void TextString::Print(void) const
{
	printf("%s\n",GetPointer());
}

TextString &  TextString::operator=(const TextString &   from)
{
	this->Set(from.GetPointer());
	return *this;
}

class TextInput
{
protected:
	TextString title;
	TextString str;

public:
	TextInput();
	~TextInput();
	void CleanUp(void);

	bool Run(const char title[]);
	void Draw(void) const;
	const TextString &GetString(void) const;
};
TextInput::TextInput()
{
}
TextInput::~TextInput()
{
	CleanUp();
}
void TextInput::CleanUp(void)
{
	title.CleanUp();
	str.CleanUp();
}

void RenderTextInput(void *incoming)
{
	TextInput *txtPtr=(TextInput *)incoming;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);
	txtPtr->Draw();
	FsSwapBuffers();
}

bool TextInput::Run(const char title[])
{
	FsRegisterOnPaintCallBack(RenderTextInput,this);

	CleanUp();
	this->title.Set(title);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			str.CleanUp();
			return false;
		}
		if(FSKEY_ENTER==key)
		{
			return true;
		}
		if(FSKEY_BS==key)
		{
			str.BackSpace();
		}
		// if(FSKEY_ESC==key)
		// {
		// 	break;
		// }

		auto c=FsInkeyChar();
		if(' '<=c && c<128)
		{
			str.Add(c);
		}

		FsPushOnPaintEvent();
		FsSleep(10);
	}

	FsRegisterOnPaintCallBack(nullptr,nullptr);
}

const TextString &TextInput::GetString(void) const
{
	return str;
}


void TextInput::Draw(void) const
{
	glColor3ub(255,255,255);

	glRasterPos2d(32,32);
	YsGlDrawFontBitmap12x16(title.GetPointer());

	auto cpy=str;
	switch(time(nullptr)%2)
	{
	case 0:
		cpy.Add("|");
		break;
	default:
		cpy.Add("_");
		break;
	}

	glRasterPos2d(32,48);
	YsGlDrawFontBitmap12x16(cpy.GetPointer());
}

class RowEle
{
public:
	int idx;
	int scoreInt;
	char scoreStr[256];
	char name[256];
};

void Render(void *incoming)
{
	Data *datPtr = (Data *) incoming;

	if(*datPtr->statePtr == 1)
	{
		int wid,hei;
        FsGetWindowSize(wid,hei);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0f, 0.0f, 0.0f);

        glViewport(0,0,wid,hei); // (x0,y0, width,hei)

        // Sort text file
		char fName[256] = "score.txt";

		int lNum=1;
		char strLine[256];
		RowEle strInfo[256];	

		FILE *fp2 = fopen(fName, "r");
		
		while(nullptr != MyFgets(strLine, 255, fp2))
		{
			int nWord;
			int wordTop[80], wordLen[80];
			nWord = ParseString(wordTop, wordLen, 80, strLine);

			for(int i = 0; i < nWord; ++i)
			{
				char word[256];
				ExtractWord(word, 256, strLine, wordTop[i], wordLen[i]);

				if((lNum % 2) == 1)
				{
					strcpy(strInfo[((lNum - 1) / 2)].name, word);
					strInfo[((lNum - 1) / 2)].idx = ((lNum - 1) / 2);
					printf("%d\t", strInfo[((lNum - 1) / 2)].idx);
					printf("%s\t", strInfo[((lNum - 1) / 2)].name);
				}
				else if((lNum % 2) == 0)
				{
					strcpy(strInfo[((lNum - 1) / 2)].scoreStr, word);
					strInfo[((lNum - 1) / 2)].scoreInt = atoi(word);
					printf("%d\n", strInfo[((lNum - 1) / 2)].scoreInt);
				}

				++lNum;
			}
		}
		lNum -= 1;
		printf("the number of line is %d.\n", lNum);

		fclose(fp2);

		for(int i = 0; i < lNum - 1; ++i)
		{
			for(int j = i + 1; j < lNum; ++j)
			{
				if(strInfo[i].scoreInt > strInfo[j].scoreInt)
				{
					auto tmp = strInfo[i].idx;
					strInfo[i].idx = strInfo[j].idx;
					strInfo[j].idx = tmp;
					printf("%d\t%d\n", strInfo[i].idx, strInfo[j].idx);
				}
			}
		}
		for(int i = 0; i < lNum; ++i)
		{
			printf("%d\t", strInfo[i].idx);
		}
		printf("\n");

        // Read text file
        char strName[256][256];
		char strScore[256][256];

		int wNum = 1;

		FILE *fp3 = fopen(fName, "r");

		// glColor3f(1.0f, 1.0f, 1.0f);
        
        while(nullptr != MyFgets(strLine, 255, fp3))
		{
			int nWord;
			int wordTop[80], wordLen[80];
			nWord = ParseString(wordTop, wordLen, 80, strLine);

			for(int i = 0; i < nWord; ++i)
			{
				char word[256];
				ExtractWord(word, 256, strLine, wordTop[i], wordLen[i]);

				if((wNum % 2) == 1)
				{
					strcpy(strName[((wNum - 1) / 2)], word);
					printf("%s\t", word);
					printf("%s\n", strName[((wNum - 1) / 2)]);
					// glRasterPos2d(32, 32 + 16 * ((wNum - 1) / 2));
        			// YsGlDrawFontBitmap12x16(strName[((wNum - 1) / 2)]);
        			// printf("%s\t", strName[i]);
				}
				else if((wNum % 2) == 0)
				{
					strcpy(strScore[((wNum - 1) / 2)], word);
					printf("%s\t", word);
					printf("%s\n", strScore[((wNum - 1) / 2)]);
					// glRasterPos2d(32 + 160, 32 + 16 * ((wNum - 1) / 2));
        			// YsGlDrawFontBitmap12x16(strScore[((wNum - 1) / 2)]);
        			// printf("%s\t", strScore[i]);
				}
				++wNum;
			}
		}

		wNum -= 1;
		printf("%d\n", wNum);

        glColor3f(1.0f, 1.0f, 1.0f);

        for(int i = 0; i < (wNum / 2); ++i)
        {
        	glRasterPos2d(32, 32 + 16 * i);
        	YsGlDrawFontBitmap12x16(strName[i]);
        	printf("%s\t", strName[i]);

        	glRasterPos2d(32 + 160, 32 + 16 * i);
        	YsGlDrawFontBitmap12x16(strScore[i]);
        	printf("%s\n", strScore[i]);
        }
        fclose(fp3);
        *datPtr->statePtr += 1;
        
        FsSwapBuffers();
	}
	else
	{
		FsSwapBuffers();
	}
}

int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	char fName[256] = "score.txt";

	Data dat;

	int gamestate = 0;
	dat.statePtr = &gamestate;
	// score is the time record of the new player.

	TextInput txt;
	if(true == txt.Run("Enter Text>>") && gamestate == 0)
	{
		printf("You entered: %s\n",txt.GetString().GetPointer());
		// TextString temp;
		// temp.Set(txt.GetString().GetPointer());
		// printf("%s\n", temp.GetPointer());
		// char tempstr[256];
		// strcpy(tempstr, temp.GetPointer());
		// printf("%s\n", tempstr);
		// strcat(tempstr, "\t30\n");
		// printf("%s\n", tempstr);
		FILE *fp1 = fopen(fName, "a+");
		// fprintf(fp1, tempstr);
		fprintf(fp1, txt.GetString().GetPointer());
		fprintf(fp1, "\t");
		fprintf(fp1, "30\n");

		fclose(fp1);

		gamestate += 1;
	}
	
	FsRegisterOnPaintCallBack(Render, &dat);

	while(1)
	{
		if(gamestate == 1)
		{
			FsPollDevice();

			int key=FsInkey();
			if(FSKEY_ESC==key)
			{
				break;
			}

			FsPushOnPaintEvent();
	        FsSleep(10);
		}
	}

	return 0;
}

