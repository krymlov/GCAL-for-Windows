#include "stdafx.h"
#include "LocationRef.h"
#include "level_4.h"
#include "vedic_ui.h"
#include "strings.h"
#include "TTimeZone.h"
#include "TFile.h"
#include "TResultApp.h"
#include "TFileRichList.h"
#include "avc.h"

const char * MSG_ERROR_1 = "Incorrect format for longitude. Correct examples: 35E05, 23W45";
const char * MSG_ERROR_2 = "Incorrect format for latitude. Correct examples: 45N05, 13S45";

extern VCTIME gToday;
extern VCTIME gTomorrow;
extern VCTIME gYesterday;

//void SunPosition(VCTIME vct, EARTHDATA ed, SUNDATA &sun, DAYHOURS DayHours);
int is_daylight_time(VCTIME vc, int nIndex);


const char * AvcGetEarthPosFromString(const char * str, bool bNorth, double &Longitude)
{
	double l = 0.0;
	double sig = 1.0;
	double coma = 10.0;
	bool after_coma = false;
	bool is_deg = false;
	const char * pret = bNorth ? MSG_ERROR_1 : MSG_ERROR_2;
	bool was_comma = false;
	bool was_letter = false;
	bool was_digit = false;
	bool was_sign = false;
	int numsAfterComma = 0;

	const char * s = str;

	while(*s)
	{
		switch(*s)
		{
		case '0': case '1':
		case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9':
			was_digit = true;
			if (after_coma)
			{
				if (is_deg)
				{
					numsAfterComma++;
					if (numsAfterComma > 2)
					{
						return "Minutes are allowed only from range 0 - 59";
					}
					l += (*s - '0') * 5.0 / (coma * 3.0);
				}
				else
				{
					l += (*s - '0') / coma;
				}
				coma *= 10.0;
			}
			else
			{
				l = l*10.0 + (*s - '0');
				if (!bNorth)
				{
					if (l > 90.0)
						return "Latitude is allowed only in range 0 - 90";
				}
				else
				{
					if (l > 180.0)
						return "Longitude is allowed only in range 0 - 180";
				}
			}
			break;
		case 'n': case 'N':
			if (was_letter || was_sign || was_comma)
				return pret;

			was_letter = true;
			if (!bNorth)
			{
				after_coma = true;
				is_deg = true;
				sig = 1.0;
			}
			else
			{
				return pret;
			}
			break;
		case 's': case 'S':
			if (was_letter || was_sign || was_comma)
				return pret;
			was_letter = true;
			if (!bNorth)
			{
				after_coma = true;
				is_deg = true;
				sig = -1.0;
			}
			else
			{
				return pret;
			}
			break;
		case 'e': case 'E':
			if (was_letter || was_sign || was_comma)
				return pret;
			was_letter = true;
			if (bNorth)
			{
				after_coma = true;
				is_deg = true;
				sig = 1.0;
			}
			else
			{
				return pret;
			}
			break;
		case 'w': case 'W':
			if (was_letter || was_sign || was_comma)
				return pret;
			was_letter = true;
			if (bNorth)
			{
				after_coma = true;
				is_deg = true;
				sig = -1.0;
			}
			else
			{
				return pret;
			}
			break;
		case '-':
			if (was_letter || was_digit || was_comma)
				return pret;
			was_sign = true;
			sig = -1.0;
			break;
		case '+':
			if (was_letter || was_digit || was_comma)
				return pret;
			was_sign = true;
			sig = 1.0;
			break;
		case '.': case ',':
			if (was_letter || was_comma)
				return pret;
			was_comma = true;
			after_coma = true;
			is_deg = false;
			break;
		default:
			return pret;
		}
		s++;
	}

	Longitude = l * sig;

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// AppDay functions

void AddTextLine(TString &str, const char * pt)
{
	str += pt;
	str += "\r\n";
}

void AddTextLineRtf(TString &str, const char * pt)
{
	str += "\r\n\\par ";
	str += pt;
}



#include "showset.h"

CShowSetting gss[] =
{
	{0, 0, "ARTI", "Tithi at arunodaya"},//0
	{0, 0, "ARTM", "Arunodaya Time"},//1
	{0, 0, "SRTM", "Sunrise Time"},//2
	{0, 0, "SSTM", "Sunset Time"},//3
	{0, 0, "MRTM", "Moonrise Time"},//4
	{0, 0, "MSTM", "Moonset Time"},//5
	{1, 1, "FEST", "Festivals"},//6
	{0, 0, "KSAY", "Info about ksaya tithi"},//7
	{0, 0, "VRDH", "Info about vriddhi tithi"},//8
	{0, 0, "SLON", "Sun Longitude"},//9
	{0, 0, "MLON", "Moon Longitude"},//10
	{0, 0, "AYAN", "Ayanamsha value"},//11
	{0, 0, "JDAY", "Julian Day"},//12
	{1, 1, "CPUR", "Caturmasya Purnima System"}, //13
	{0, 0, "CPRA", "Caturmasya Pratipat System"}, //14
	{0, 0, "CEKA", "Caturmasya Ekadasi System"}, //15
	{1, 1, "SANI", "Sankranti Info"}, //16
	{1, 1, "EKAI", "Ekadasi Info"}, //17
	{1, 1, "VHDR", "Masa Header Info"}, //18
	{0, 0, "PHDR", "Month Header Info"}, //19
	{0, 0, "EDNS", "Do not show empty days"}, //20
	{0, 0, "SBEM", "Show begining of masa"}, //21
	{1, 1, "F000", "Appearance days of the Lord"},//22
	{1, 1, "F001", "Events in the pastimes of the Lord"},//23
	{1, 1, "F002", "App, Disapp of Recent Acaryas"},//24
	{1, 1, "F003", "App, Disapp of Mahaprabhu's Associates and Other Acaryas"},//25
	{1, 1, "F004", "ISKCON's Historical Events"},//26
	{1, 1, "F005", "Bengal-specific Holidays"},//27
	{1, 1, "F006", "My Personal Events"}, //28
	/* BEGIN GCAL 1.4.3 */
	{1, 1, "TSSR", "Todat Sunrise"},  //29 Today sunrise
	{1, 1, "TSSN", "Today Noon"},  //30 today noon
	{1, 1, "TSSS", "Today Sunset"},  //31 today sunset
	{0, 0, "TSAN", "Sandhya Times"},  //32 today + sandhya times
	{1, 1, "TSIN", "Sunrise Info"},  //33 today sunrise info
	{0, 0, "ASIN", "Noon Time"},  //34 astro - noon time
	{1, 1, "NDST", "Notice about DST"}, //35 notice about the change of the DST
	{1, 1, "DNAK", "Naksatra"}, // 36 naksatra info for each day
	{1, 1, "DYOG", "Yoga"}, //37 yoga info for each day
	{1, 1, "FFLG", "Fasting Flag"},//38
	{1, 1, "DPAK", "Paksa Info"},//39 paksa info
	{0, 0, "FDIW", "First Day in Week"},//40 first day in week
	{0, 0, "DRAS", "Rasi"}, //41 moon rasi for each calendar day
	{0, 0, "OSFA", "Old Style Fasting text"}, //42 old style fasting text
	{0, 0, "MLNT", "Name of month - type"}, //43 month type name 0-vaisnava,1-bengal,2-hindu,3-vedic
	/* END GCAL 1.4.3 */
	{0, 0, "EDBL", "Editable Default Events"}, //44 editable default events
	{0, 0, "TSBM", "Today Brahma Muhurta"},     //45 brahma muhurta in today screen
	{0, 0, "TROM", "Today Rasi of the Moon"}, // 46 rasi of the moon in today screen
	{0, 0, "TNPD", "Today Naksatra Pada details"}, // 47 naksatra pada details in today screen
	{0, 0, "ADCS", "Child Names Suggestions"}, // 48 child name suggestions in Appearance Day screen
	{0, 0, "MNFO", "Masa Name Format"}, // 49 format of masa name
	{0, 0, "EPDR", "Ekadasi Parana details"}, // 50 ekadasi parana details
	{0, 0, "ANIV", "Aniversary show format"} // 51 format of aniversary info
};

int GetShowSetCount()
{
	return sizeof(gss) / sizeof(CShowSetting);
}

int GetShowSetChangesCount()
{
	int i, count = 0;
	int size = GetShowSetCount();

	for(i = 0; i < size; i++)
	{
		if (gss[i].val != gss[i].old_val)
			count++;
		gss[i].old_val = gss[i].val;
	}

	return count;
}

char * GetShowSetName(int i)
{
	return gss[i].text;
}

char * GetShowSetSignature(int i)
{
	return gss[i].sig;
}

int GetShowSetVal(int i)
{
	return gss[i].val;
}

void SetShowSetVal(int i, int val)
{
	gss[i].val = val;
	gss[i].old_val = val;
}

void SetShowSetVal(const char * pszSign, int val)
{
	int i, max = GetShowSetCount();

	for(i = 0; i < max; i++)
	{
		if (_tcscmp(gss[i].sig, pszSign) == 0)
		{
			gss[i].val = val;
			gss[i].old_val = val;
			break;
		}
	}
}

void AvcShowSetReadFile(const char * psz)
{
	TString strA, strB;
	TFile file;

	if (file.Open(psz, "r") == TRUE)
	{
		while(file.ReadPropertyLine(strA, strB))
		{
			if (strA.GetLength() > 1 && strA.GetAt(0) == '#')
			{
			}
			else
			{
				SetShowSetVal(strA, atoi(strB));
			}
		}

		file.Close();
		::DeleteFile(psz);
	}
}

/*void AvcShowSetWriteFile(const char * pszFile)
{
	TFile file;

	if (file.Open(pszFile, "w") == TRUE)
	{
		int i, max = GetShowSetCount();
		TString str;
		for(i = 0; i < max; i++)
		{
			str.Format("#\r\n# %s\r\n#\r\n\r\n%s=%d\r\n\r\n", GetShowSetName(i), GetShowSetSignature(i),
				GetShowSetVal(i));
			file.WriteString(str);
		}
		file.Close();
	}
}*/

int AvcGetShowSetCount()
{
	return GetShowSetCount();
}

Boolean AvcGetNextShowSet(int &nItem, int &nCheck, const char * &pszText)
{
	if (nItem >= GetShowSetCount())
		return FALSE;

	nCheck = gss[nItem].val;
	pszText = gss[nItem].text;
	nItem++;
	return TRUE;
}

void AvcGetDateText(TString &str, VCTIME vc)
{
	if ((vc.day > 0) && (vc.day < 32) && (vc.month > 0) && (vc.month < 13) && (vc.year >= 1500) && (vc.year < 4000))
	{
		if (vc == gToday)
		{
			str.Format("%d %s %d (%s)", vc.day, AvcGetMonthAbr(vc.month), vc.year, gstr[43].c_str());
		}
		else if (vc == gTomorrow)
		{
			str.Format("%d %s %d (%s)", vc.day, AvcGetMonthAbr(vc.month), vc.year, gstr[854].c_str());
		}
		else if (vc == gYesterday)
		{
			str.Format("%d %s %d (%s)", vc.day, AvcGetMonthAbr(vc.month), vc.year, gstr[853].c_str());
		}
		else
		{
			str.Format("%d %s %d", vc.day, AvcGetMonthAbr(vc.month), vc.year);
		}
	}
}

int AvcGetNextPartHash(TString &str, TString &strLine, int & i)
{
	int index = i, length;
	
	if (index >= str.GetLength())
		return 0;

	TCHAR tc;
	strLine.Empty();
	length = str.GetLength();

	while(1)
	{
		if (index >= length)
			break;

		tc = str.GetAt(index);
		if (tc == '#')
		{
			index++;
			break;
		}
		else
		{
			strLine += tc;
		}
		index++;
	}

	i = index;
	return 1;
}

int AvcGetFestivalClass(TString &str)
{
	int i, j, val;

	i = str.Find("[c");

	if (i >= 0)
	{
//		i += 2;
		if ((i + 2) < str.GetLength())
		{
			val = int(str.GetAt(i+2) - '0');
			j = str.Find("]", i);
			if (j >= str.GetLength() || j < 0)
				j = str.GetLength();
			if (j > i)
			{
				str.Delete(i, j - i + 1);
			}
			if (val < 0 || val > 6)
				return -1;
			return val;
		}
		else
			return -1;
	}
	else
	{
		return -1;
	}

}

int AvcClearStringFromTags(TString &str)
{
	int i, j, val = 0;

	i = str.Find("[");

	while (i >= 0)
	{
		if ((i + 2) < str.GetLength())
		{
			j = str.Find("]", i);
			if (j >= str.GetLength() || j < 0)
				j = str.GetLength();
			if (j > i)
			{
				str.Delete(i, j - i + 1);
				val ++;
			}
		}

		i = str.Find("[");
	}

	return val;
}

TString g_appstr[32];

int GCalApp_InitFolders()
{
	char pszBuffer[1024];
	int len, i;
	len = GetModuleFileName(AfxGetInstanceHandle(), pszBuffer, 1020);
	for(i = len-1; i > 0; i--)
	{
		if (pszBuffer[i] == '\\')
		{
			pszBuffer[i+1] = '\0';
			len = i+1;
			break;
		}
	}

	g_appstr[GSTR_APPFOLDER] = pszBuffer;

	g_appstr[GSTR_CONFOLDER] = g_appstr[GSTR_APPFOLDER];
	g_appstr[GSTR_CONFOLDER] += "config\\";
	CreateDirectory(g_appstr[GSTR_CONFOLDER], NULL);

	g_appstr[GSTR_LANFOLDER] = g_appstr[GSTR_APPFOLDER];
	g_appstr[GSTR_LANFOLDER] += "lang\\";
	CreateDirectory(g_appstr[GSTR_LANFOLDER], NULL);

	g_appstr[GSTR_TEMFOLDER] = g_appstr[GSTR_APPFOLDER];
	g_appstr[GSTR_TEMFOLDER] += "temp\\";
	CreateDirectory(g_appstr[GSTR_TEMFOLDER], NULL);

	g_appstr[GSTR_CE_FILE].Format("%scevents.cfg", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_CONF_FILE].Format("%scurrent.cfg", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_LOC_FILE].Format("%slocations.cfg", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_SSET_FILE].Format("%sshowset.cfg", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_LOCX_FILE].Format("%sloc.rl", g_appstr[GSTR_CONFOLDER].c_str());//GCAL 3.0
	g_appstr[GSTR_CEX_FILE].Format("%scev3.rl", g_appstr[GSTR_CONFOLDER].c_str());//GCAL 3.0
	g_appstr[GSTR_CONFX_FILE].Format("%sset.rl", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_TZ_FILE].Format("%stz.rl", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_COUNTRY_FILE].Format("%sctrs.rl", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_TEXT_FILE].Format("%sstrings.rl", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_TIPS_FILE].Format("%stips.txt", g_appstr[GSTR_CONFOLDER].c_str());
	g_appstr[GSTR_HELP_FILE].Format("%sgcal.chm", g_appstr[GSTR_APPFOLDER].c_str());
	return 1;
}

const char * GCalApp_GetFileName(int n)
{
	return g_appstr[n].c_str();
}

int AvcComboMasaToMasa(int nComboMasa)
{
	return (nComboMasa == 12) ? 12 : ((nComboMasa + 11) % 12);
}

int AvcMasaToComboMasa(int nMasa)
{
	return (nMasa == 12) ? 12 : ((nMasa + 1) % 12);
}

int AvcGetLocationAsText(TString &str, EARTHDATA earth)
{
	str.Format("%s: %s  %s: %s  %s: %s", 
		gstr[10].c_str(), AvcGetTextLatitude(earth.latitude_deg), 
		gstr[11].c_str(), AvcGetTextLongitude(earth.longitude_deg),
		gstr[12].c_str(), AvcGetTextTimeZone(earth.tzone));

	return 1;
}

const char * AvcDoubleToString(double d)
{
	static TCHAR ts[128];

	sprintf(ts, "%f", d);

	return ts;
}

const char * AvcIntToString(int d)
{
	static TCHAR ts[32];

	sprintf(ts, "%d", d);

	return ts;
}

const char * AvcDateToString(VCTIME vc)
{
	static TCHAR ts[32];

	sprintf(ts, "%d %s %04d", vc.day, AvcGetMonthAbr(vc.month), vc.year);

	return ts;
}

int AvcGetNextLine(TString &str, TString &strLine, int & i)
{
	int index = i, length;
	
	if (index >= str.GetLength())
		return 0;

	TCHAR tc;
	strLine.Empty();
	length = str.GetLength();

	while(1)
	{
		if (index >= length)
			break;

		tc = str.GetAt(index);
		if (tc == '\r');
		else if (tc == '\n')
		{
			index++;
			break;
		}
		else
		{
			strLine += tc;
		}
		index++;
	}

	i = index;
	return 1;
}

Boolean ConditionEvaluate(VAISNAVADAY * pd, int nClass, int nValue, TString &strText, Boolean defaultRet)
{
	static char * pcstr[] = {"", "", "", "", "", "", "", "", "", "", 
		"[c0]", "[c1]", "[c2]", "[c3]", "[c4]", "[c5]", "", ""};

	switch(nClass)
	{
	// mahadvadasis
	case 1:
		if (nValue == EV_NULL)
			return ((pd->nMhdType != EV_NULL) && (pd->nMhdType != EV_SUDDHA));
		else
			return (pd->nMhdType == nValue);
	// sankrantis
	case 2:
		if (nValue == 0xff)
			return (pd->sankranti_zodiac >= 0);
		else
			return (pd->sankranti_zodiac == nValue);
	// tithi + paksa
	case 3:
		return (pd->astrodata.nTithi == nValue);
	// naksatras
	case 4:
		return (pd->astrodata.nNaksatra == nValue);
	// yogas
	case 5:
		return (pd->astrodata.nYoga == nValue);
	// fast days
	case 6:
		if (nValue == 0)
			return (pd->nFastType != FAST_NULL);
		else
			return (pd->nFastType == (0x200 + nValue));
	
	// week day
	case 7:
		return (pd->date.dayOfWeek == nValue);
	// tithi
	case 8:
		return (pd->astrodata.nTithi % 15 == nValue);
	// paksa
	case 9:
		return (pd->astrodata.nPaksa == nValue);
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		if (nValue == 0xffff)
		{
			return (pd->festivals.Find(pcstr[nClass]) >= 0);
		}
		else
		{
			if (pd->astrodata.nMasa == 12)
				return FALSE;
			if (abs(pd->astrodata.nTithi + pd->astrodata.nMasa*30 - nValue + 200) > 2)
				return FALSE;
			if (pd->festivals.Find(strText) >= 0)
				return TRUE;
		}
		return FALSE;
	case 15:
		if (nValue == 0xffff)
		{
			return (strText.Find(pcstr[15]) >= 0);
		}
		else
		{
			// difference against 10-14 is that we cannot test tithi-masa date
			// because some festivals in this category depends on sankranti
			if (pd->festivals.Find(strText) >= 0)
				return TRUE;
		}
		return FALSE;
	default:
		return defaultRet;
	}
}

int AvcGetTextLineCount(VAISNAVADAY * pvd)
{
	int i, nFestClass;
	int nCount = 0;
	TString str2;

	nCount++;

	if (GetShowSetVal(17) == 1)
	{
		if (pvd->ekadasi_parana)
		{
			nCount++;
		}
	}

	if (GetShowSetVal(6) == 1)
	{
		if (pvd->festivals)
		{
			i = pvd->GetHeadFestival();
			while(pvd->GetNextFestival(i, str2))
			{
				if (str2.GetLength() > 1)
				{
					nFestClass = pvd->GetFestivalClass(str2);
					if (nFestClass < 0 || GetShowSetVal(22 + nFestClass) == 1)
					{
						nCount++;
					}
				}
			}
		}
	}

	if (GetShowSetVal(16) == 1 && pvd->sankranti_zodiac >= 0)
	{
		nCount++;
	}

	if (GetShowSetVal(7) == 1 && pvd->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
	{
		nCount++;
	}

	if (GetShowSetVal(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
	{
		if (pvd->is_vriddhi)
		{
			nCount++;
		}
	}

	if (pvd->nCaturmasya & CMASYA_CONT_MASK)
	{
		nCount++;
	}

	if ((GetShowSetVal(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
	{
		nCount++;
		if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
		{
			nCount++;
		}
	}

	if ((GetShowSetVal(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
	{
		nCount++;
		if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
		{
			nCount++;
		}
	}

	if ((GetShowSetVal(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
	{
		nCount++;
		if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
		{
			nCount++;
		}
	}

	// tithi at arunodaya
	if (GetShowSetVal(0) == 1)//m_dshow.m_tithi_arun)
	{
		nCount++;
	}

	//"Arunodaya Time",//1
	if (GetShowSetVal(1) == 1)//m_dshow.m_arunodaya)
	{
		nCount++;
	}
	//"Sunrise Time",//2
	//"Sunset Time",//3
	if (GetShowSetVal(2) == 1)//m_dshow.m_sunrise)
	{
		nCount++;
	}
	if (GetShowSetVal(3) == 1)//m_dshow.m_sunset)
	{
		nCount++;

	}
	//"Moonrise Time",//4
	if (GetShowSetVal(4) == 1)
	{
		nCount++;
	}
	//"Moonset Time",//5
	if (GetShowSetVal(5) == 1)
	{
		nCount++;
	}
	///"Sun Longitude",//9
	if (GetShowSetVal(9) == 1)//m_dshow.m_sun_long)
	{
		nCount++;
	}
	//"Moon Longitude",//10
	if (GetShowSetVal(10) == 1)//m_dshow.m_sun_long)
	{
		nCount++;
	}
	//"Ayanamsha value",//11
	if (GetShowSetVal(11) == 1)//m_dshow.m_sun_long)
	{
		nCount++;
	}
	//"Julian Day",//12
	if (GetShowSetVal(12) == 1)//m_dshow.m_sun_long)
	{
		nCount++;
	}

	return nCount;
}

const char * FormatDate(VCTIME vc, VATIME va)
{
	static char sz[128];

	sprintf(sz, "%d %s %d\r\n%s, %s Paksa, %s Masa, %d",
		vc.day, AvcGetMonthAbr(vc.month), vc.year,
		GetTithiName(va.tithi%15), GetPaksaName(va.tithi/15), GetMasaName(va.masa), va.gyear);

	return sz;
}

int CalcEndDate(EARTHDATA m_earth, VCTIME vcStart, VATIME vaStart, VCTIME &vcEnd, VATIME &vaEnd, int nType, int nCount)
{
	switch(nType)
	{
	case 1:
		vcEnd = vcStart;
		if (nCount > 30240) nCount = 30240;
		vcEnd += nCount;
		VCTIMEtoVATIME(vcEnd, vaEnd, m_earth);
		break;
	case 2:
		vcEnd = vcStart;
		if (nCount > 4320) nCount = 4320;
		vcEnd += nCount*7;
		VCTIMEtoVATIME(vcEnd, vaEnd, m_earth);
		break;
	case 3:
		vcEnd = vcStart;
		if (nCount > 1080) nCount = 1080;
		vcEnd.month += nCount;
		while(vcEnd.month > 12)
		{
			vcEnd.year++;
			vcEnd.month -= 12;
		}
		VCTIMEtoVATIME(vcEnd, vaEnd, m_earth);
		break;
	case 4:
		vcEnd = vcStart;
		if (nCount > 90) nCount = 90;
		vcEnd.year += nCount;
		VCTIMEtoVATIME(vcEnd, vaEnd, m_earth);
		break;
	case 5:
		vaEnd = vaStart;
		if (nCount > 30240) nCount = 30240;
		vaEnd.tithi += nCount;
		while(vaEnd.tithi >= 30)
		{
			vaEnd.tithi-=30;
			vaEnd.masa++;
		}
		while(vaEnd.masa >= 12)
		{
			vaEnd.masa -= 12;
			vaEnd.gyear++;
		}
		VATIMEtoVCTIME(vaEnd, vcEnd, m_earth);
		break;
	case 6:
		vaEnd = vaStart;
		if (nCount > 1080) nCount = 1080;
		vaEnd.masa = AvcMasaToComboMasa(vaEnd.masa);
		if (vaEnd.masa == ADHIKA_MASA)
		{
			vcEnd = vcStart;
			vcEnd.month += nCount;
			while(vcEnd.month > 12)
			{
				vcEnd.year++;
				vcEnd.month -= 12;
			}
			VCTIMEtoVATIME(vcEnd, vaEnd, m_earth);
			vaEnd.tithi = vaStart.tithi;
			VATIMEtoVCTIME(vaEnd, vcEnd, m_earth);
		}
		else
		{
			vaEnd.masa += nCount;
			while(vaEnd.masa >= 12)
			{
				vaEnd.masa -= 12;
				vaEnd.gyear++;
			}
			vaEnd.masa = AvcComboMasaToMasa(vaEnd.masa);
			VATIMEtoVCTIME(vaEnd, vcEnd, m_earth);
		}
		break;
	case 7:
		vaEnd = vaStart;
		if (nCount > 90) nCount = 90;
		vaEnd.gyear += nCount;
		VATIMEtoVCTIME(vaEnd, vcEnd, m_earth);
		break;
	}

	return 1;
}

int AvcLoadStrings(const char * pszFile)
{
	int v = 0;
	TFileRichList rf;

	if (rf.Open(pszFile, "rt") != 0)
	{
		int index = 0;
		while(rf.ReadLine())
		{
			if (atoi(rf.GetTag())==78)
			{
				index = atoi(rf.GetField(0));
				if (index >= 0 && index < 900)
				{
					gstr[index] = rf.GetField(1);
					v++;
				}
			}
		}
		rf.Close();
	}
	else
	{
		return -1;
	}

	return v;
}

int AvcSaveStrings(const char * pszFile)
{
	int i, j, v = 0;
	// a[x][0] je zaciatocny index
	// a[x][1] je konecny index skupiny (vratane)
	int a[3][2] =
	{
		{ 0, 128 },
		{ 135, 199 },
		{ 561, 899 }
	};
	TFileRichList trf;

	if (trf.Open(pszFile, "wt"))
	{
		// save 0 - 128
		// save 135 - 199
		// save 561 - 899
		for(j = 0; j < 3; j++)
		{
			for(i = a[j][0]; i <= a[j][1]; i++)
			{
				if (gstr[i].GetLength() > 0)
				{
					trf.Clear();
					trf.AddTag(78);
					trf.AddInt(i);
					trf.AddText(gstr[i].c_str());
					trf.WriteLine();
					v++;
				}
			}
		}
		trf.Close();
	}
	else
	{
		return -1;
	}

	return v;
}