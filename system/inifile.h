#ifndef _INIFILE_H_
#define _INIFILE_H_

class TIniFile
{
	int		handle;
	int		length;
	bool 	flagOpen;
	char	*buf;

public:
	TIniFile(const char *pszName=NULL);
	~TIniFile();
	bool IsOpen(){return flagOpen;};
	int Open(const char *pszName);
	int Close(void);
	bool SectionExist(const char *szSection);
	int GetValue(const char *szSection, const char *pszValueName, char *pszValue, int nValueLen);
	int GetInt(const char *szSection, const char *pszValueName, int nDefaultValue);
	int GetIP(const char *szSection, const char *pszValueName, void *paIP, int max_ip=1);
	int GetAInt(const char *szSection, const char *pszValueName, void *pIP, int max_int=1);
	
	
};

unsigned long InetAddr(const char *szAddr);

#endif
