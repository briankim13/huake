// // #include "fssimplewindow.h"
// // #include "ysglfontdata.h"

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>



// // char *MyFgets(char str[],int maxn,FILE *fp)
// // {
// // 	auto r=fgets(str,maxn,fp);
// // 	if(nullptr!=r)
// // 	{
// // 		for(int i=strlen(str)-1; 0<=i; --i)
// // 		{
// // 			if(str[i]<' ')
// // 			{
// // 				str[i]=0;
// // 			}
// // 			else
// // 			{
// // 				break;
// // 			}
// // 		}
// // 	}
// // 	else
// // 	{
// // 		str[0]=0;
// // 	}
// // 	return r;
// // }


// // int ParseString(int wordTop[],int wordLen[],int maxlen,char input[])
// // {
// // 	if(0==maxlen)
// // 	{
// // 		return 0;
// // 	}

// // 	int state=0;
// // 	int wordCount=0;
// // 	for(int i=0; 0!=input[i]; ++i)
// // 	{
// // 		if(0==state)
// // 		{
// // 			if(' '<input[i])
// // 			{
// // 				wordTop[wordCount]=i;
// // 				wordLen[wordCount]=1;
// // 				state=1;
// // 				++wordCount;
// // 			}
// // 		}
// // 		else if(1==state)
// // 		{
// // 			if(input[i]<=' ')
// // 			{
// // 				state=0;
// // 				if(maxlen<=wordCount)
// // 				{
// // 					break;
// // 				}
// // 			}
// // 			else
// // 			{
// // 				++wordLen[wordCount-1];
// // 			}
// // 		}
// // 	}

// // 	return wordCount;
// // }


// // class Parser
// // {
// // protected:
// // 	int nw;
// // 	int *wTop,*wLen;
// // 	char *str;

// // public:
// // 	Parser();
// // 	~Parser();
// // 	void CleanUp(void);

// // 	int Parse(char str[]);
// // 	void GetWord(char wd[],int maxlen,int idx);
// // };

// // Parser::Parser()
// // {
// // 	nw=0;
// // 	str=nullptr;
// // 	wTop=nullptr;
// // 	wLen=nullptr;
// // }
// // Parser::~Parser()
// // {
// // 	CleanUp();
// // }
// // void Parser::CleanUp(void)
// // {
// // 	nw=0;
// // 	if(nullptr!=str)
// // 	{
// // 		delete [] str;
// // 		str=nullptr;
// // 	}
// // 	if(nullptr!=wTop)
// // 	{
// // 		delete [] wTop;
// // 		wTop=nullptr;
// // 	}
// // 	if(nullptr!=wLen)
// // 	{
// // 		delete [] wLen;
// // 		wLen=nullptr;
// // 	}
// // }
// // int Parser::Parse(char incoming[])
// // {
// // 	int maxlen=(strlen(str)+1)/2;
// // 	CleanUp();

// // 	str=new char [strlen(incoming)+1];
// // 	strcpy(str,incoming);
// // 	wTop=new int [maxlen];
// // 	wLen=new int [maxlen];
// // 	return ParseString(wTop,wLen,maxlen,str);
// // }


// // class Score
// // {
// // protected:
// // 	int nScore;
// // 	char strName[256];

// // public:
// // 	void ReadFile(char fName[]);
// // };

// // void Score::ReadFile(char fName[])
// // {
// // 	FILE *fp=fopen(fName,"r");
// // 	if(nullptr!=fp)
// // 	{
// // 		char str[256];
// // 		if(nullptr!=fgets(str,255,fp))
// // 		{
// // 			nScore = atoi(str);
// // 			printf("%d scores will be shown.\n", nScore);

// // 			int n=0;
			
// // 			for(int i=0; i<nScore; ++i)
// // 			{
// // 				if(nullptr!=fgets(str,255,fp))
// // 				{
// // 					int nw,wTop[2],wLen[2];
// // 					if(2<=ParseString(wTop,wLen,2,str))
// // 					{
// // 						strName=(str+wTop[0]);
// // 						// strScore[n]=(str+wTop[1]);
// // 						++n;
// // 					}
// // 				}
// // 			}

// // 			fclose(fp);
// // 		}
// // 	}
// // }

// // void Render(void *incoming)
// // {
// // 	Score *score=(Score *)incoming;

// // 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
// // 	printf("%s", score->strName);
// // 	FsSwapBuffers();
// // }

// // int main(void)
// // {
// // 	FsOpenWindow(0,0,800,600,1);

// // 	FsChangeToProgramDir();

// // 	Score score;
// // 	score.ReadFile("score.txt");

// // 	FsRegisterOnPaintCallBack(Render,&score);
// // 	for(;;)
// // 	{
// // 		FsPollDevice();
// // 		auto key=FsInkey();
// // 		if(FSKEY_ESC==key)
// // 		{
// // 			break;
// // 		}

// // 		FsPushOnPaintEvent();
// // 		FsSleep(10);
// // 	}

// // 	return 0;
// // }

// #include <stdio.h>
// #include <string.h>



// char *MyFgets(char str[],int maxn,FILE *fp)
// {
// 	auto r=fgets(str,maxn,fp);
// 	if(nullptr!=r)
// 	{
// 		for(int i=strlen(str)-1; 0<=i; --i)
// 		{
// 			if(str[i]<' ')
// 			{
// 				str[i]=0;
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		str[0]=0;
// 	}
// 	return r;
// }


// int ParseString(int wordTop[],int wordLen[],int maxlen,char input[])
// {
// 	if(0==maxlen)
// 	{
// 		return 0;
// 	}

// 	int state=0;
// 	int wordCount=0;
// 	for(int i=0; 0!=input[i]; ++i)
// 	{
// 		if(0==state)
// 		{
// 			if(' '<input[i])
// 			{
// 				wordTop[wordCount]=i;
// 				wordLen[wordCount]=1;
// 				state=1;
// 				++wordCount;
// 			}
// 		}
// 		else if(1==state)
// 		{
// 			if(input[i]<=' ')
// 			{
// 				state=0;
// 				if(maxlen<=wordCount)
// 				{
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				++wordLen[wordCount-1];
// 			}
// 		}
// 	}

// 	return wordCount;
// }

// int main(void)
// {
// 	char fName[256];
// 	printf("File Name>");
// 	MyFgets(fName,255,stdin);

// 	FILE *fp=fopen(fName,"r");
// 	char str[256];
// 	while(nullptr!=MyFgets(str,255,fp))
// 	{
// 		printf("%s",str);
// 	}
// 	fclose(fp);

// 	return 0;
// }

#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;



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

class strArray
{
	char str[256];
};

int main(void)
{
	char fName[256];
	printf("File Name>");
	MyFgets(fName,255,stdin);

	int wNum=1;

	FILE *fp=fopen(fName,"r");
	char str[256];

	while(nullptr!=MyFgets(str,255,fp))
	{
		int nWord;
		int wordTop[80],wordLen[80];
		nWord=ParseString(wordTop,wordLen,80,str);
		
		for(int i=0; i<nWord; ++i)
		{
			++wNum;
		}
	}
	printf("%d\n", wNum);
	// strArray strName[wNum/2];
	// strArray strScore[wNum/2];
		
	wNum=1;

	while(nullptr!=MyFgets(str,255,fp))
	{
		int nWord;
		int wordTop[80],wordLen[80];
		nWord=ParseString(wordTop,wordLen,80,str);
		// wNum = 1;

		for(int i=0; i<nWord; ++i)
		{
			char word[256];
			ExtractWord(word,256,str,wordTop[i],wordLen[i]);
			// if(wNum%2==1)
			// {
			// 	strName[(wNum-1)/2]=word;
			// }
			// else
			// {
			// 	strScore[(wNum-1)/2]=word;
			// }
			printf("%d:%s\n",wNum,word);
			++wNum;
		}
	}
	fclose(fp);

	return 0;
}

