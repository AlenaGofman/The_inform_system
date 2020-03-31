#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<ctype.h>
#include<syslog.h>
#include<netinet/in.h>
#include"inifile.h"
#include"types.h"

//-------------------------------------------------------------------------
static bool IsSpaceChar(char ch)
{
	return ch==' ' || ch=='\t' || ch==',' || ch=='(' || ch==')'
		|| ch=='/' || ch=='-';
}

//-------------------------------------------------------------------------
int Inet_aton(const char *szAddr, u32 *inaddr)
{
	u32     ip_addr = 0;

	const char *p = szAddr;
	ip_addr = ((BYTE)atoi(p)<<24);
	p = strchr(p, '.');
	if(p==NULL)
		return 0;
	p++;

	ip_addr |= ((BYTE)atoi(p)<<16);
	p = strchr(p, '.');
	if(p==NULL)
		return 0;
	p++;

	ip_addr |= ((BYTE)atoi(p)<<8);
	p = strchr(p, '.');
	if(p==NULL)
		return 0;
	p++;
	ip_addr |= (BYTE)atoi(p);
	if(inaddr)
		*inaddr = htonl(ip_addr);

	return 1;
}

//------------------------------------------
unsigned long InetAddr(const char *szAddr)
{
	unsigned long ipaddr = 0;
	if(szAddr==NULL)
		return (long)-1;

	const char *p = szAddr;
	
	while(*p)
	{
		char ch = *p;
		if(ch!=' ')
			break;

		p++;
	}

	unsigned k = 0;

	for(unsigned i=0; i<4; )
	{
		unsigned char ch = *p++;
		if(ch>='0' && ch<='9')
		{
			k = k*10 + ch-'0';
			continue;
		}

		ipaddr <<= 8;
		ipaddr |= (k & 0xFF);

		if(ch==0)
			break;

		if(ch=='.')
		{
			k = 0;
			i++;
		}
	}

	return htonl(ipaddr);
}

//------------------------------------------------------
static int AtoL(const char *sz)
{
	int n = 0;
	if(sz)
	{
		while(*sz==' ')
			sz++;

		if(sz[0]=='0' && sz[1]=='x')
			n = strtoll(sz+2, NULL, 16);
		else
			n = atol(sz);
	}
	return n;
}

//----------------------------------------------------------------------
static char *StrNStr(const char *str, const char *str_find)
{
	while(*str)
	{
		const char *p1 = str_find;

		if(toupper(*str) == toupper(*p1))
		{
			const char *p = str;
			while(1)
			{
				if(*p1 == 0)
					return (char*)str;

				if(toupper(*p++) != toupper(*p1++))
					break;
			}
		}
		str++;
	}

	return NULL;
}

//----------------------------------------------------------------------
static int StrCmpZ(const char *str1, const char *str2)
{
	for(unsigned i=0; ; i++)
	{
		if(str1[i]==0 || str2[i]==0)
			break;
			
		if(str1[i]<str2[i])
			return -1;

		if(str1[i]>str2[i])
			return 1;
	}

	return 0;
}

//----------------------------------------------------------------------
static char *SkipSpace(const char *szStr)
{
	while(1)
	{
		char ch = *szStr++;
		if(ch==0)
			break;

		if(ch!=' ' && ch!=9)
			break;
	}

	return (char*)szStr-1;
}

//-----------------------------------------------------
static char *NextLine(char *szStr)
{
	while(1)
	{
		char ch = *szStr++;
		if(ch==0)
			return szStr-1;

		if(ch==0x0A || ch==0x0D)
			break;
	}

	while(1)
	{
		char ch = *szStr++;
		if(ch==0)
			return szStr-1;

		if(ch!=0x0A && ch!=0x0D)
			break;
	}

	return szStr-1;
}

//-------------------------------------------------------
TIniFile::TIniFile(const char *pszName)
{
	handle = -1;
	buf = NULL;
	flagOpen = 0;
	if(pszName)
		flagOpen = Open(pszName)>=0;

	return;
};

TIniFile::~TIniFile()
{
	if(buf)
		delete buf;

	return;
};

//-------------------------------------------------------
int TIniFile::Open(const char *pszName)
{
	handle = open(pszName, O_RDONLY);
	if(handle<0)
		return -1;

	length = lseek(handle, 0, SEEK_END);
	if(length<=0)
		return -1;

	lseek(handle, 0, SEEK_SET);
	buf = new char[length+16];
	length = read(handle, buf, length);
	close(handle);
	buf[length] = 0;

	char flagComment = 0;
	char flagSpace = 0;
	int i, j;
	for(i=0, j=0; i<length; i++)
	{
		char ch = buf[i];

		if(flagComment==0)
		{
			if(ch==';' || ch=='#')
				flagComment = 1;
		}

		if(ch=='\n')
		{
			flagSpace = 0;
			flagComment = 0;
		}

		if(flagComment==0)
		{
			if(flagSpace==1 && !(ch==' ' || ch=='\t'))
				flagSpace = 0;

			if(flagSpace==0)
				buf[j++] = ch;

			if(flagSpace==0 && (ch==' ' || ch=='\t'))
				flagSpace = 1;
		}
	}

	buf[j] = 0;

	return 0;
}

//--------------------------------------------------------------------
bool TIniFile::SectionExist(const char *pszSection)
{
	if(buf==NULL)
		return 0;

	return StrNStr(buf, pszSection)!=NULL;
}

//--------------------------------------------------------------------
int TIniFile::GetValue(const char *pszSection, const char *pszValueName,
									char *pszValue, int nValueLen)
{
	if(buf==NULL)
		return -1;

	char *p = buf;
	char skip_line = 0;
	if(pszSection && pszSection[0])
	{
		unsigned len = strlen(pszSection);
		char sz[128];
		if(len>sizeof(sz)-2)
			len = sizeof(sz)-2;

		if(pszSection[0]=='[')
		{
			strcpy(sz, pszSection);
		}
		else
		{
			sz[0] = '[';
			strcpy(sz+1, pszSection);
			sz[len+1] = ']';
			sz[len+2] = 0;
		}
		
		p = StrNStr(buf, sz);
		if(p==NULL)
			return -1;

		skip_line = 1;
	}

	while(1)
	{
		int	i;
		if(skip_line)
			p = NextLine(p);

		skip_line = 1;
		p = SkipSpace(p);
		if(*p==0 || *p=='[')
			return 1;

		if(StrCmpZ(p, pszValueName)!=0)
			continue;

		p = SkipSpace(p+strlen(pszValueName));
		if(*p==0)
			return 1;

		if(*p!='=')
			continue;

		p = SkipSpace(p+1);
		if(*p==0)
			return 1;

		if(*p=='\n' || *p=='\r')
			continue;

		for(i=0; i<nValueLen; i++)
		{
			char ch = p[i];
			if(ch=='\n' || ch=='\r' || ch==';' || ch=='#' || ch==0)
				break;

			pszValue[i] = ch;
		}

		pszValue[i] = 0;
		break;
	}

	return 0;
}

//--------------------------------------------------------------------------
int TIniFile::GetInt(const char *szSection, const char *szValueName, int nDefaultValue)
{
	if(buf==NULL)
		return nDefaultValue;

	char	sz[80];
	sz[0] = 0;
	if(GetValue(szSection, szValueName, sz, sizeof(sz)-1))
		return nDefaultValue;
	return AtoL(sz);
}

//--------------------------------------------------------------------------
int TIniFile::GetIP(const char *szSection, const char *pszValueName, void *pIP, int max_ip)
{
	if(buf==NULL)
		return 0;

	char	szValue[120];
	if(GetValue(szSection, pszValueName, szValue, sizeof(szValue)))
		return 0;

	char *sz = szValue;
	int i = 0;
	for(i=0; *sz && i<max_ip; i++)
	{
		char *p = sz;
		while(*p && !IsSpaceChar(*p)) //*p!=' ' && *p!='\t')
			p++;
			
		char	ch_save = *p;
		*p = 0;
			
		u32 addr;
		Inet_aton(sz, &addr);
				
		((u32*)pIP)[i] = addr;
		
		*p = ch_save;
		sz = p;
		while(IsSpaceChar(*sz))
			sz++;
	}

	return i;
}

//--------------------------------------------------------------------------
int TIniFile::GetAInt(const char *szSection, const char *pszValueName, void *pInt, int max_int)
{
	if(buf==NULL)
		return 0;

	char	szValue[120];
	if(GetValue(szSection, pszValueName, szValue, sizeof(szValue)))
		return 0;

	char *sz = szValue;
	int i = 0;
	for(i=0; *sz && i<max_int; i++)
	{
		char *p = sz;
		while(*p && !IsSpaceChar(*p))
			p++;
			
		int n = AtoL(sz);
		((int*)pInt)[i] = n;

		sz = p;		
		while(*sz && IsSpaceChar(*sz))
			sz++;
	}

	return i;
}
