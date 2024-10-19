/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

#include <stdafx.h>



#include "LocationRef.h"
#include "XmlFile.h"
#include "level_4.h"
#include "strings.h"
#include "vedic_ui.h"
#include "level_5_days.h"
#include "TTimeZone.h"
#include "TResultApp.h"
#include "TResultEvent.h"
#include "TResultMasaList.h"


#define DAYS_TO_ENDWEEK(i) (21-(i-g_firstday_in_week))%7
#define DAYS_FROM_BEGINWEEK(i) (i-g_firstday_in_week+14)%7
#define DAY_INDEX(i) (i+g_firstday_in_week)%7

int GetShowSetVal(int i);
void CalcAppDay(CLocationRef &, VCTIME, TResultApp &);
int CalcCalendar(TResultCalendar &, CLocationRef &, VCTIME, int);
int CalcMasaList(TResultMasaList &, CLocationRef &, int, int);
void CalcEvents(TResultEvents &, CLocationRef &, VCTIME, VCTIME, UInt32);

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

void WriteAppDayHTML(TResultApp &app, FILE *F)
{
	DAYDATA &d = app.details;
	TString str;
	VCTIME vc = app.event;
	EARTHDATA m_earth = (EARTHDATA)app.location;

	fprintf(F, "<html><head><title>Appearance day</title>");
	fprintf(F, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:11pt;\n}\n\ntd.hed {\n  font-size:11pt;\n  font-weight:bold;\n");
	fprintf(F, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(F, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(F, "</head>\n\n<body>\n");
	fprintf(F, "<h2 align=center>Appearance day Calculation</h2>");
	fprintf(F, "<table align=center><tr><td valign=top>\n\n");
	fprintf(F, "<table align=center>");
	fprintf(F, "<tr><td colspan=3 class=hed>Details</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d %s %d</td></tr>\n", gstr[7].c_str(), vc.day, AvcGetMonthAbr(vc.month), vc.year);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d:%02d</td></tr>\n\n", gstr[8].c_str(), vc.GetHour(), vc.GetMinuteRound());
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[9].c_str(), app.location.m_strName.c_str());
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[10].c_str(), AvcGetTextLatitude(app.location.m_fLatitude));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[11].c_str(), AvcGetTextLongitude(app.location.m_fLongitude));
	fprintf(F, "<tr><td colspan=2>%s</td><td> ", gstr[12].c_str());
	fprintf(F, AvcGetTextTimeZone(app.location.m_fTimezone));
	fprintf(F, "</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>DST</td><td>N/A</td></tr>\n");
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[13].c_str(), GetTithiName(d.nTithi));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %.2f %%</td></tr>\n", gstr[14].c_str(), d.nTithiElapse);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[15].c_str(), GetNaksatraName(d.nNaksatra));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %.2f %%</td></tr>\n", gstr[16].c_str(), d.nNaksatraElapse);
	fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[20].c_str(), GetPaksaName(d.nPaksa));
	if (app.b_adhika == true)
	{
		fprintf(F, "<tr><td colspan=2>%s</td><td> %s %s</td></tr>\n", gstr[22].c_str(), GetMasaName(d.nMasa), gstr[21].c_str());
	}
	else
		fprintf(F, "<tr><td colspan=2>%s</td><td> %s</td></tr>\n", gstr[22].c_str(), GetMasaName(d.nMasa));
	fprintf(F, "<tr><td colspan=2>%s</td><td> %d</td></tr>\n\n", gstr[23].c_str(), d.nGaurabdaYear);

	fprintf(F, "</table></td><td valign=top><table>");
	fprintf(F, "<tr><td colspan=3 class=hed>%s</td></tr>\n", gstr[24].c_str());

	//fprintf(F, "<table align=center>");
	for(int o = 0; o < TRESULT_APP_CELEBS; o++)
	{
		fprintf(F, "<tr><td>Gaurabda %3d</td><td>&nbsp;&nbsp;:&nbsp;&nbsp;</td><td><b>%d %s %d</b></td></tr>", app.celeb_gy[o] , app.celeb_date[o].day,
			AvcGetMonthAbr(app.celeb_date[o].month), 
			app.celeb_date[o].year);
	}
	fprintf(F, "</table>");
	fprintf(F, "</td></tr></table>\n\n");
	fprintf(F, "<hr align=center width=\"50%%\">\n<p style=\'text-align:center;font-size:8pt\'>Generated by %s</p>", gstr[130].c_str());
	fprintf(F, "</body></html>");

}

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

int WriteEventsHTML(TResultEvents &inEvents, FILE * f)
{

	unsigned int i;
	TString str, temp;

	fprintf(f, "<html>\n<head>\n<title>Core Events</title>\n\n");
	fprintf(f, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:11pt;\n}\n\ntd.hed {\n  font-size:11pt;\n  font-weight:bold;\n");
	fprintf(f, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(f, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(f, "</head>\n");
	fprintf(f, "<body>\n\n");
	fprintf(f, "<h1 align=center>Events</h1>\n<p align=center>From %d %s %d to %d %s %d.</p>\n\n", 
		inEvents.m_vcStart.day,
		AvcGetMonthAbr(inEvents.m_vcStart.month),
		inEvents.m_vcStart.year,
		inEvents.m_vcEnd.day,
		AvcGetMonthAbr(inEvents.m_vcEnd.month),
		inEvents.m_vcEnd.year);

	fprintf(f, "<p align=center>%s</p>\n", inEvents.m_location.m_strFullName.c_str());

	VCTIME prevd;
	int prevt = -1;

	prevd.day = 0;
	prevd.month = 0;
	prevd.year = 0;

	fprintf(f, "<table align=center><tr>\n");
	for(i = 0; i < inEvents.n_count; i++)
	{
		TDayEvent & dnr = inEvents[i];

		if (inEvents.b_sorted)
		{
			if (prevd.day != dnr.Time.day || prevd.month != dnr.Time.month || prevd.year != dnr.Time.year)
			{
				fprintf(f, "<td class=\"hed\" colspan=2>%d %s %d </td></tr>\n<tr>", dnr.Time.day, AvcGetMonthAbr(dnr.Time.month), dnr.Time.year);
			}
			prevd = dnr.Time;
		}
		else
		{
			if (prevt != dnr.nType)
			{
				switch(dnr.nType)
				{
				case 10:
				case 11:
				case 12:
				case 13:
					if (prevt < 10 || prevt >= 14)
					{
						fprintf(f, "<td class=\"hed\" colspan=3>SUNRISE, SUNSET</td></tr>\n<tr>\n");
					}
					break;
				case 20:
					fprintf(f, "<td class=\"hed\" colspan=3>TITHIS</td></tr>\n<tr>\n");
					break;
				case 21:
					fprintf(f, "<td class=\"hed\" colspan=3>NAKSATRAS</td></tr>\n<tr>\n");
					break;
				case 22:
					fprintf(f, "<td class=\"hed\" colspan=3>SANKRANTIS</td></tr>\n<tr>\n");
					break;
				case 23:
					fprintf(f, "<td class=\"hed\" colspan=3>SUN-MOON CONJUNCTIONS</td></tr>\n<tr>\n");
					break;
				}
			}
			prevt = dnr.nType;
		}

		if (!inEvents.b_sorted)
		{
			fprintf(f, "<td>%d %s %d </td>", dnr.Time.day, AvcGetMonthAbr(dnr.Time.month), dnr.Time.year);
		}

		switch(dnr.nType)
		{
		case 10:
			fprintf(f, "<td>Arunodaya</td><td>%02d:%02d:%02d</td></tr><tr>\n",
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 11:
			fprintf(f, "<td>Sunrise</td><td>%02d:%02d:%02d</td></tr><tr>\n",
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 12:
			fprintf(f, "<td>Noon</td><td>%02d:%02d:%02d</td></tr><tr>\n",
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 13:
			fprintf(f, "<td>Sunset</td><td>%02d:%02d:%02d</td></tr><tr>\n",
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 20:
			fprintf(f, "<td>%s Tithi</td><td>%02d:%02d:%02d</td></tr><tr>\n", 
				GetTithiName(dnr.nData),
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 21:
			fprintf(f, "<td>%s Naksatra</td><td>%02d:%02d:%02d</td></tr><tr>\n", 
				GetNaksatraName(dnr.nData),
				dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 22:
			fprintf(f, "<td>%s Sankranti</td><td>%02d:%02d:%02d</td></tr><tr>\n", 
				GetSankrantiName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		case 23:
			fprintf(f, "<td>Conjunction in %s rasi</td><td>%02d:%02d:%02d</td></tr><tr>\n", 
				GetSankrantiName(dnr.nData)
				, dnr.Time.GetHour(), dnr.Time.GetMinute(), dnr.Time.GetSecond());
			break;
		default:
			break;
		}
	}

	fprintf(f, "</tr></table>\n");
	fprintf(f, "<hr align=center width=\"50%%\">\n<p align=center>Generated by %s</p>", gstr[130].c_str());
	fprintf(f, "</body>\n</html>\n");

	return 1;
}

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

int WriteMasaListHTML(TResultMasaList &mlist, FILE * f)
{
	TString stt;
	TString stt2;

	fprintf(f, "<html>\n<head>\n<title>Masa List</title>\n\n");
	fprintf(f, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:11pt;\n}\n\ntd.hed {\n  font-size:11pt;\n  font-weight:bold;\n");
	fprintf(f, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(f, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(f, "</head>\n");
	fprintf(f, "<body>\n\n");

	fprintf(f, "<p style=\'text-align:center\'><span style=\'font-size:14pt\'>Masa List</span></br>%s: %s</p>\n", gstr[40].c_str(), mlist.m_location.m_strFullName.c_str());
	fprintf(f, "<p align=center>%s %d %s %d %s %d %s %d</p>\n", gstr[41].c_str(), mlist.vc_start.day, AvcGetMonthAbr(mlist.vc_start.month), mlist.vc_start.year
		, gstr[42].c_str(), mlist.vc_end.day, AvcGetMonthAbr(mlist.vc_end.month), mlist.vc_end.year);
	fprintf(f, "<hr width=\"50%%\">");

	fprintf(f, "<table align=center>");
	fprintf(f, "<tr><td class=\"hed\" style=\'text-align:left\'>MASA NAME&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td class=\"hed\">START</td><td class=\"hed\">END</td></tr>");
	int i;
	for(i = 0; i < mlist.n_countMasa; i++)
	{
		fprintf(f, "<tr>");
		fprintf(f, "<td>%s %d&nbsp;&nbsp;&nbsp;&nbsp;</td>", GetMasaName(mlist.arr[i].masa), mlist.arr[i].year);
		fprintf(f, "<td>%d %s %d</td>", mlist.arr[i].vc_start.day, AvcGetMonthAbr(mlist.arr[i].vc_start.month), mlist.arr[i].vc_start.year);
		fprintf(f, "<td>%d %s %d</td>", mlist.arr[i].vc_end.day, AvcGetMonthAbr(mlist.arr[i].vc_end.month), mlist.arr[i].vc_end.year);
		fprintf(f, "</tr>");
	}
	fprintf(f, "</table>");
	fprintf(f, "<hr width=\"50%%\">\n<p align=center>Generated by %s</p>", gstr[130].c_str());
	fprintf(f, "</body></html>");
	return 1;
}

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

int WriteCalendarHTML(TResultCalendar &daybuff, FILE * fout)
{
	int k;
	TString str, st;
	TString daystr;
	VCTIME date;
	TFileXml xml;
	VAISNAVADAY * pvd;
	int nPrevMasa = -1;
	int nPrevPaksa = -1;

	xml = fout;

	xml << "<html><head><title>\n";
//	xml << "\t<request name=\"Calendar\" version=\"" << gstr[130] << "\">\n";
//	xml << "\t\t<arg name=\"longitude\" val=\"" << daybuff.m_Location.m_fLongitude << "\" />\n";
//	xml << "\t\t<arg name=\"latitude\" val=\"" << daybuff.m_Location.m_fLatitude << "\" />\n";
//	xml << "\t\t<arg name=\"timezone\" val=\"" << daybuff.m_Location.m_fTimezone << "\" />\n";
//	xml << "\t\t<arg name=\"startdate\" val=\"" << daybuff.m_vcStart << "\" />\n";
//	xml << "\t\t<arg name=\"daycount\" val=\"" << daybuff.m_vcCount << "\" />\n";
//	xml << "\t\t<arg name=\"dst\" val=\"" << daybuff.m_Location.m_nDST << "\" />\n";
//	xml << "\t</request>\n";
//	xml << "\t<result name=\"Calendar\">\n";
//	if (daybuff.m_Location.m_nDST > 0)
//		xml << "\t<dstsystem name=\"" << TTimeZone::GetTimeZoneName(daybuff.m_Location.m_nDST) << "\" />\n";

	xml << "Calendar " << daybuff.m_vcStart.year << "</title>";
	xml << "<style>\n";
	xml << "<!--\nbody {\n";
	xml << "  font-family:Verdana;\n";
	xml << "  font-size:11pt;\n}\n\n";
	xml << "td.hed {\n";
	xml << "  font-family:Verdana;\n";
	xml << "  font-size:9pt;\n";
	xml << "  font-weight:bold;\n";
	xml << "  background:#aaaaaa;\n";
	xml << "  color:white;\n";
	xml << "  text-align:center;\n";
	xml << "  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n";
	xml << "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n";
	xml << "</head>\n<body>";

	for (k = 0; k < daybuff.m_vcCount; k++)
	{
		pvd = daybuff.GetDay(k);
		if (pvd)
		{
			if (nPrevMasa != pvd->astrodata.nMasa)
			{
				if (nPrevMasa != -1)
					xml << "\t</table>\n";
				xml << "<p style=\'text-align:center;font-weight:bold\'><span style =\'font-size:14pt\'>" << GetMasaName(pvd->astrodata.nMasa) << " Masa";
				if (nPrevMasa == ADHIKA_MASA)
					xml << " " << gstr[109];
				xml << "</span>";
				xml << "<br><span style=\'font-size:10pt;\'>Gaurabda " << pvd->astrodata.nGaurabdaYear;
				xml << "<br>" << daybuff.m_Location.m_strFullName << "</font>";
				xml << "</span></p>\n<table align=center>";
				xml << "<tr><td  class=\"hed\"colspan=2>";
				xml << "DATE</td><td class=\"hed\">TITHI</td><td class=\"hed\">P</td><td class=\"hed\">NAKSATRA</td><td class=\"hed\">YOGA</td><td class=\"hed\">FAST</td></tr>";
			}

			nPrevMasa = pvd->astrodata.nMasa;

			if (((pvd->ekadasi_parana) || (pvd->festivals.GetLength() > 0) ||
				(pvd->sankranti_zodiac >= 0) || (GetShowSetVal(7) == 1 && pvd->was_ksaya) ||
				(GetShowSetVal(8) == 1 && pvd->is_vriddhi) ||
				((GetShowSetVal(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK)) ||
				((GetShowSetVal(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK)) ||
				((GetShowSetVal(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))) == FALSE
				&& (GetShowSetVal(20) != 0))
					 continue;

			// date data
			xml << "<tr>";
			xml << "<td>" << pvd->date << "</td><td>";
			gstr[pvd->date.dayOfWeek].Left(2, st);
			xml << st.c_str() << "</td>\n";

			// sunrise data
			//xml << "\t\t<sunrise time=\"" << pvd->astrodata.sun.rise << "\">\n";

			//xml << "\t\t\t<tithi name=\"";
			xml << "<td>\n";
			xml << GetTithiName(pvd->astrodata.nTithi);
			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) 
				|| (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					if (pvd->nMhdType == EV_NULL)
					{
						xml << " " << gstr[58];
					}
					else
					{
						xml << " " << gstr[59];
					}
				}
			}
			xml << "</td>\n";

			//str.Format("\" elapse=\"%.1f\" index=\"%d\"/>\n"
			//	,pvd->astrodata.nTithiElapse, pvd->astrodata.nTithi % 30 + 1 );
			//xml << str;

			str.Format("<td>%c</td>\n", GetPaksaChar(pvd->astrodata.nPaksa) );
			xml << str;

			str.Format("<td>%s</td>\n", GetNaksatraName(pvd->astrodata.nNaksatra));
			xml << str;

			str.Format("<td>%s</td>\n", GetYogaName(pvd->astrodata.nYoga) );
			xml << str;


			xml << "<td>" << ((pvd->nFastType!=FAST_NULL)?"FAST</td>":"</td>");
			//xml << "\t\t</sunrise>\n";

			//xml << "\t\t<dst offset=\"" << pvd->nDST << "\" />\n";
			// arunodaya data
			//xml << "\t\t<arunodaya time=\"" << pvd->astrodata.sun.arunodaya << "\">\n";
			//xml << "\t\t\t<tithi name=\"" << GetTithiName(pvd->astrodata.nTithiArunodaya) << "\" />\n";
			//xml << "\t\t</arunodaya>\n";

			str.Empty();

			//xml << "\t\t<noon time=\"" << pvd->astrodata.sun.noon << "\" />\n";

			//xml << "\t\t<sunset time=\"" << pvd->astrodata.sun.set << "\" />\n";

			// moon data
			//xml << "\t\t<moon rise=\"" << pvd->moonrise << "\" set=\"" << pvd->moonset << "\" />\n";

			xml << "</tr>\n\n<tr>\n<td></td><td></td><td colspan=4>";
			if (pvd->ekadasi_parana)
			{
				double h1, m1, h2, m2;
				m1 = modf(pvd->eparana_time1, &h1);
				if (pvd->eparana_time2 >= 0.0)
				{
					m2 = modf(pvd->eparana_time2, &h2);
					str.Format("Break fast %02d:%02d - %02d:%02d %s<br>\n", int(h1), int(m1*60), int(h2), int(m2*60),
						pvd->nDST == 1 ? "(DST applied)" : "(Local Time)");
				}
				else
				{
					str.Format("Break fast after %02d:%02d<br>\n", int(h1), int(m1*60) );
				}
				xml << str;
			}
			str.Empty();

			if (pvd->festivals.GetLength() > 0)
			{
				int i = pvd->GetHeadFestival();
				int nFestClass;
				TString str2;
				while(pvd->GetNextFestival(i, str2))
				{
					if (str2.GetLength() > 1)
					{
						nFestClass = pvd->GetFestivalClass(str2);
						str.Format("%s<br>\n", str2.c_str());
						xml << str;
					}
				}
			}

			if (pvd->nFastType != FAST_NULL)
			{
				switch (pvd->nFastType)
				{
				case FAST_EKADASI:
					//xml << "\t\t<fast type=\"Fasting for ";
					//xml << pvd->ekadasi_vrata_name;
					//xml << "\" mark=\"*\"/>\n";
					//break;
				case FAST_NOON:
					//xml << "\t\t<fast type=\"(Fast till noon)\"  mark=\"*\"/>\n";
					//xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
					//break;
				case FAST_SUNSET:
					//xml << "\t\t<fast type=\"(Fast till sunset)\" mark=\"*\" />\n";
					//xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
					//break;
				case FAST_MOONRISE:
					//xml << "\t\t<fast type=\"(Fast till moonrise)\" mark=\"*\" />\n";
					//xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
					//break;
				case FAST_DUSK:
					//xml << "\t\t<fast type=\"(Fast till dusk)\" mark=\"*\" />\n";
					//xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
					//break;
				case FAST_MIDNIGHT:
					//xml << "\t\t<fast type=\"(Fast till midnight)\" mark=\"*\" />\n";
					//xml << "\t\t<fast type=\"\" mark=\"*\" />\n";
					break;
				default:
					//xml << "\t\t<fast type=\"\" mark=\"\" />\n";
					break;
				}
			}

			if (pvd->sankranti_zodiac >= 0)
			{
				//double h1, m1, s1;
				//m1 = modf(pvd->sankranti_day.shour*24, &h1);
//				s1 = modf(m1*60, &m1);
				str.Format("<font color=\"blue\">%s Sankranti (<i>%d %s %d  %02d:%02d:%02d</i>)</font><br>\n"
					, GetSankrantiName(pvd->sankranti_zodiac), pvd->sankranti_day.day,
					AvcGetMonthAbr(pvd->sankranti_day.month), pvd->sankranti_day.year,
					pvd->sankranti_day.GetHour()
					, pvd->sankranti_day.GetMinute(), pvd->sankranti_day.GetSecond());
				xml << str;
			}

			if (GetShowSetVal(7) == 1 && pvd->was_ksaya)
			{
				double h1, m1, h2, m2;
				VCTIME ksayaDate;
				xml << "Previous tithi is ksaya from ";
				m1 = modf(pvd->ksaya_time1*24, &h1);
				ksayaDate = pvd->date;
				if (pvd->ksaya_day1 < 0.0)
					ksayaDate.PreviousDay();
				str.Format("%d %s, %02d:%02d", ksayaDate.day, AvcGetMonthAbr(ksayaDate.month), int(h1), int(m1*60));
				xml << str;

				m2 = modf(pvd->ksaya_time2*24, &h2);
				str.Format("to %d %s, %02d:%02d<br>\n", ksayaDate.day, AvcGetMonthAbr(ksayaDate.month), int(h2), abs(int(m2*60)));
				xml << str;
			}

			if (GetShowSetVal(8) == 1 && pvd->is_vriddhi)
			{
				xml << "Second day of vriddhi tithi<br>\n";
			}

			if ((GetShowSetVal(14) == 1) && (pvd->nCaturmasya & CMASYA_PRAT_MASK))
			{
				xml << gstr[107 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
							   + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str();
				xml << "[PRATIPAT SYSTEM]<br>\n";
				if ((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
				{
					xml << gstr[110 + ((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str();
				}
			}

			if ((GetShowSetVal(13) == 1) && (pvd->nCaturmasya & CMASYA_PURN_MASK))
			{
				xml << gstr[107 + (pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)
							   + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str();
				xml << "[PURNIMA SYSTEM]<br>";
				if ((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
				{
					xml << gstr[110 + ((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str();
				}
			}

			if ((GetShowSetVal(15) == 1) && (pvd->nCaturmasya & CMASYA_EKAD_MASK))
			{
				xml << gstr[107 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
							   + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str();
				xml << "[EKADASI SYSTEM]<br>";
				if ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
				{
					xml << gstr[110 + ((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str();
				}
			}
			xml << "\t</tr>\n\n";

		}
		date.shour = 0;
		date.NextDay();
	}
	xml << "\t</table>\n\n";

	xml<< "<hr align=center width=\"65%%\">\n";
	xml << "<p align=center>Generated by " << gstr[130] << "</p>\n";
	
	xml << "</body>\n</html>\n";

	return 1;
}

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/

int WriteCalendarHtmlTable(TResultCalendar &daybuff, FILE * fout)
{
	int g_firstday_in_week = GetShowSetVal(40);
	int k, y, lwd;
	TString str, st;
	VCTIME date;
	TFileXml xml;
	VAISNAVADAY * pvd;
	int nPrevMasa = -1;
	int prevMas = -1;
	int brw = 0;

	// first = 1
	//int i_end[7] = {0, 6, 5, 4, 3, 2, 1}; //(6-(i-first))%7
	//int i_beg[7] = {6, 0, 1, 2, 3, 4, 5}; //(i-first)%7
	xml = fout;

	xml << "<html>\n<head>\n<title>Calendar " << daybuff.m_vcStart << "</title>\n";

	xml << "<style>\n<!--\np.MsoNormal, li.MsoNormal, div.MsoNormal\n	{mso-style-parent:\"\";";
	xml << "margin:0in;margin-bottom:.0001pt;mso-pagination:widow-orphan;font-size:8.0pt;font-family:Arial;";
	xml << "mso-fareast-font-family:Arial;}";
	xml << "p.month\n{mso-style-name:month;\nmso-margin-top-alt:auto;\nmargin-right:0in;\nmso-margin-bottom-alt:auto;\nmargin-left:0in;\nmso-pagination:widow-orphan;\nfont-size:17.0pt;font-family:Arial;mso-fareast-font-family:Arial;}\n";
	xml << ".text\n{mso-style-name:text;\nmso-margin-top-alt:auto;\nmargin-right:0in;\nmso-margin-bottom-alt:auto;\nmargin-left:0in;\n	mso-pagination:widow-orphan;\nfont-size:6.0pt;\nmso-bidi-font-size:6.0pt;\nfont-family:Arial;	mso-fareast-font-family:\"Arial\";mso-bidi-font-family:\"Arial\";}\n";
	xml << ".tnote\n{mso-style-name:text;\nmso-margin-top-alt:auto;\nmargin-right:0in;\nmso-margin-bottom-alt:auto;\nmargin-left:0in;\n	mso-pagination:widow-orphan;\nfont-size:7.0pt;\nmso-bidi-font-size:7.0pt;\nfont-family:Arial;	mso-fareast-font-family:Arial;mso-bidi-font-family:Arial;}\n";
	xml << ".tithiname\n{mso-style-name:text;\nmso-margin-top-alt:auto;\nmargin-right:0in;\nmso-margin-bottom-alt:auto;\nmargin-left:0in;\n	mso-pagination:widow-orphan;\nfont-size:8.0pt;\nmso-bidi-font-size:8.0pt;\nfont-family:Arial;	mso-fareast-font-family:\"Arial\";mso-bidi-font-family:\"Arial\";}\n";
	xml << ".dayt\n	{mso-style-name:dayt;\nfont-size:12.0pt;\nmso-ansi-font-size:12.0pt;\nfont-family:Arial;\nmso-ascii-font-family:Arial;\nmso-hansi-font-family:Arial;\nfont-weight:bold;\nmso-bidi-font-weight:normal;}\n";
	xml << "span.SpellE\n{mso-style-name:\"\";\nmso-spl-e:yes;}\n";
	xml << "span.GramE\n{mso-style-name:\"\";\nmso-gram-e:yes;}\n";
	xml << "-->\n</style>\n";

	xml << "</head>\n\n<body>\n\n";
/*	xml << "\t<request name=\"Calendar\" version=\"" << gstr[130] << "\">\n";
	xml << "\t\t<arg name=\"longitude\" val=\"" << daybuff.m_Location.m_fLongitude << "\" />\n";
	xml << "\t\t<arg name=\"latitude\" val=\"" << daybuff.m_Location.m_fLatitude << "\" />\n";
	xml << "\t\t<arg name=\"timezone\" val=\"" << daybuff.m_Location.m_fTimezone << "\" />\n";
	xml << "\t\t<arg name=\"startdate\" val=\"" << daybuff.m_vcStart << "\" />\n";
	xml << "\t\t<arg name=\"daycount\" val=\"" << daybuff.m_vcCount << "\" />\n";
	xml << "\t\t<arg name=\"dst\" val=\"" << daybuff.m_Location.m_nDST << "\" />\n";
	xml << "\t</request>\n";
	xml << "\t<result name=\"Calendar\">\n";
	if (daybuff.m_Location.m_nDST > 0)
		xml << "\t<dstsystem name=\"" << TTimeZone::GetTimeZoneName(daybuff.m_Location.m_nDST) << "\" />\n";
*/
	for (k = 0; k < daybuff.m_vcCount; k++)
	{
		pvd = daybuff.GetDay(k);
		if (pvd)
		{
			BOOL bSemicolon = false;
			BOOL bBr = false;
			lwd = pvd->date.dayOfWeek;
			if (nPrevMasa != pvd->date.month)
			{
				int y;
				if (nPrevMasa != -1)
				{
					for(y = 0; y < DAYS_TO_ENDWEEK(lwd); y++)
					{
						xml << "<td style=\'border:solid windowtext 1.0pt;mso-border-alt:solid windowtext .5pt;padding:3.0pt 3.0pt 3.0pt 3.0pt\'>&nbsp;</td>";
					}
					xml << "</tr></table>\n<p>&nbsp;</p>";
				}
				xml << "\n<table width=\"100%\" border=0 frame=bottom cellspacing=0 cellpadding=0><tr><td width=\"60%\"><p class=month>" << gstr[pvd->date.month + 759].c_str() << " "<< pvd->date.year;
				xml << "</p></td><td><p class=tnote align=right>";
				xml << daybuff.m_Location.m_strName;
				xml << "<br>Timezone: ";
				xml << TTimeZone::GetTimeZoneName(daybuff.m_Location.m_nDST);
				xml << "</p>";
				xml << "</td></tr></table><hr>";
				nPrevMasa = pvd->date.month;
				xml << "\n<table width=\"100%\" bordercolor=black cellpadding=0 cellspacing=0>\n<tr>\n";
				for(y = 0; y < 7; y++)
				{
					xml << "<td width=\"14%\" align=center style=\'font-size:10.0pt;border:none\'>" << gstr[DAY_INDEX(y)].c_str() << "</td>\n";
				}
				xml << "<tr>\n";
				for(y=0; y < DAYS_FROM_BEGINWEEK(pvd->date.dayOfWeek); y++)
					xml << "<td style=\'border:solid windowtext 1.0pt;mso-border-alt:solid windowtext .5pt;padding:3.0pt 3.0pt 3.0pt 3.0pt\'>&nbsp;</td>";
			}
			else
			{
				if (pvd->date.dayOfWeek == g_firstday_in_week)
					xml << "<tr>\n";
			}

			// date data
			xml << "\n<td valign=top style=\'border:solid windowtext 1.0pt;mso-border-alt:solid windowtext .5pt;padding:3.0pt 3.0pt 3.0pt 3.0pt\' bgcolor=\"";
			switch (pvd->nFastType)
			{
			case FAST_EKADASI:
				xml << "#ffffbb";
				break;
			case FAST_NOON:
			case FAST_SUNSET:
			case FAST_MOONRISE:
			case FAST_DUSK:
			case FAST_MIDNIGHT:
			case FAST_DAY:
				xml << "#bbffbb";
				break;
			default:
				xml << "white";
				break;
			}
			xml << "\"><table width=\"100%\" border=0><tr><td><p class=text><span class=dayt>" << pvd->date.day << "</span></td><td>";

			// sunrise data
//			xml << "\t\t<sunrise time=\"" << pvd->astrodata.sun.rise << "\">\n";

//			xml << "hi name=\"";
			xml << "<span class=\"tithiname\">";
			xml << GetTithiName(pvd->astrodata.nTithi);
			if ((pvd->astrodata.nTithi == 10) || (pvd->astrodata.nTithi == 25) 
				|| (pvd->astrodata.nTithi == 11) || (pvd->astrodata.nTithi == 26))
			{
				if (pvd->ekadasi_parana == false)
				{
					if (pvd->nMhdType == EV_NULL)
					{
						xml << " " << gstr[58];
					}
					else
					{
						xml << " " << gstr[59];
					}
				}
			}
			xml << "</span>";
			xml << "</td></tr></table>\n";
			brw = 0;
			xml << "<span class=\"text\">\n";
//			str.Format("\" elapse=\"%.1f\" index=\"%d\"/>\n"
//				,pvd->astrodata.nTithiElapse, pvd->astrodata.nTithi % 30 + 1 );
//			xml << str;

//			str.Format("\t\t\t<naksatra name=\"%s\" elapse=\"%.1f\" />\n"
//				,GetNaksatraName(pvd->astrodata.nNaksatra), pvd->astrodata.nNaksatraElapse );
//			xml << str;

//			str.Format("\t\t\t<yoga name=\"%s\" />\n", GetYogaName(pvd->astrodata.nYoga) );
//			xml << str;

//			str.Format("\t\t\t<paksa id=\"%c\" name=\"%s\"/>\n", GetPaksaChar(pvd->astrodata.nPaksa), GetPaksaName(pvd->astrodata.nPaksa) );
//			xml << str;

//			xml << "\t\t</sunrise>\n";

//			xml << "\t\t<dst offset=\"" << pvd->nDST << "\" />\n";
			// arunodaya data
//			xml << "\t\t<arunodaya time=\"" << pvd->astrodata.sun.arunodaya << "\">\n";
//			xml << "\t\t\t<tithi name=\"" << GetTithiName(pvd->astrodata.nTithiArunodaya) << "\" />\n";
//			xml << "\t\t</arunodaya>\n";

			str.Empty();

//			xml << "\t\t<noon time=\"" << pvd->astrodata.sun.noon << "\" />\n";

//			xml << "\t\t<sunset time=\"" << pvd->astrodata.sun.set << "\" />\n";

			// moon data
//			xml << "\t\t<moon rise=\"" << pvd->moonrise << "\" set=\"" << pvd->moonset << "\" />\n";

			if (pvd->ekadasi_parana)
			{
				double h1, m1, h2, m2;
				m1 = modf(pvd->eparana_time1, &h1);
				if (pvd->eparana_time2 >= 0.0)
				{
					m2 = modf(pvd->eparana_time2, &h2);
					str.Format("Break fast %02d:%02d-%02d:%02d", int(h1), int(m1*60), int(h2), int(m2*60));
				}
				else
				{
					str.Format("Break fast after %02d:%02d", int(h1), int(m1*60) );
				}
				if (brw)
					xml << "<br>\n";
				brw = 1;
				xml << str;
				bBr = true;
				bSemicolon = true;
			}
			str.Empty();

			if (pvd->festivals.GetLength() > 0)
			{
				int i = pvd->GetHeadFestival();
				int nFestClass;
				TString str2;
				if (brw)
					xml << "<br>\n";
				brw = 1;
				while(pvd->GetNextFestival(i, str2))
				{
					if (str2.GetLength() > 1)
					{
						nFestClass = pvd->GetFestivalClass(str2);
//						str.Format("", str2.c_str(), nFestClass);
						if (bSemicolon)
							xml << "; ";
						xml << str2.c_str();
						bSemicolon=true;
						bBr=false;
					}
				}
			}

			if (pvd->sankranti_zodiac >= 0)
			{
				str.Format("<i>%s Sankranti</i>\n", GetSankrantiName(pvd->sankranti_zodiac));
				if (brw)
					xml << "<br>\n";
				brw = 1;
				xml << str;
				bBr = true;
			}

/*			if (pvd->was_ksaya)
			{
				double h1, m1, h2, m2;
				m1 = modf(pvd->ksaya_time1*24, &h1);
				m2 = modf(pvd->ksaya_time2*24, &h2);
				str.Format("\t\t<ksaya from=\"%02d:%02d\" to=\"%02d:%02d\" />\n", int(h1), abs(int(m1*60)), int(h2), abs(int(m2*60)));
				xml << str;
			}

			if (pvd->is_vriddhi)
			{
				xml << "\t\t<vriddhi sd=\"yes\" />\n";
			}
			else
			{
				xml << "\t\t<vriddhi sd=\"no\" />\n";
			}
*/
/*			if (pvd->nCaturmasya & CMASYA_PURN_MASK)
			{
				xml << "\t\t<caturmasya day=\"" 
					<< (((pvd->nCaturmasya & CMASYA_PURN_MASK_DAY)) > 1 ? "last" : "first")
					<< " day of \" month=\"" 
					<< int((pvd->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 4) 
					<< "\" system=\"PURNIMA\" />\n";
			}

			if (pvd->nCaturmasya & CMASYA_PRAT_MASK)
			{
				xml << "\t\t<caturmasya day=\"" 
					<< (((pvd->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8) > 1 ? "last" : "first")
					<< "\" month=\"" 
					<< int((pvd->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 12)
					<< "\" system=\"PRATIPAT\" />\n";
			}

			if (pvd->nCaturmasya & CMASYA_EKAD_MASK)
			{
				str.Format("\t<caturmasya day=\"%s\" month=\"%d\" system=\"EKADASI\" />\n"
					, ((pvd->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16) > 1 ? "last" : "first"
					, int((pvd->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 20));
				xml << str;
			}
*/
			if (prevMas != pvd->astrodata.nMasa)
			{
				if (brw)
					xml<<"<br>\n";
				brw = 1;
				xml << "<b>[" << GetMasaName(pvd->astrodata.nMasa) << " Masa]</b>";
				prevMas = pvd->astrodata.nMasa;
			}
			xml << "</span>";
			xml << "</td>\n\n";

		}
		date.shour = 0;
		date.NextDay();
	}
/*	for(y = (pvd->date.dayOfWeek + 6)%7; y < 7; y++)
	{
		xml << "<td></td>";
	}
*/
	for(y = 1; y < DAYS_TO_ENDWEEK(lwd); y++)
	{
		xml << "<td style=\'border:solid windowtext 1.0pt;mso-border-alt:solid windowtext .5pt;padding:3.0pt 3.0pt 3.0pt 3.0pt\'>&nbsp;</td>";
	}
	xml << "</tr>\n</table>\n";

	
	xml << "</body>\n</html>\n";

	return 1;
}

const char * GetDayHumanTitle(VCTIME & vc)
{
	VCTIME tod;
	int n;

	tod.Today();
	n = int(tod.GetJulian() - vc.GetJulian());
	if (n == -1)
		return "[Tomorrow]";
	if (n == 1)
		return "[Yesterday]";
	if (n == 0)
		return "[Today]";
	return "";
}

const char * gcal_html_getDayBackground(VAISNAVADAY * p)
{
	if (p == NULL)
		return "white";
	if (p->nFastType == FAST_EKADASI)
		return "#FFFFBB";
	if (p->nFastType != 0)
		return "#BBFFBB";
	return "white";
}

/******************************************************************************************/
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/*                                                                                        */
/******************************************************************************************/


void WriteTodayInfoHTML(VCTIME vc, CLocationRef & loc, FILE * f)
{
	TString str2, str3, str4;

	int k = 0, nFestClass;

	TResultCalendar db;

	VCTIME vc2 = vc;
	vc2.tzone = loc.m_fTimezone;
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	vc2.PreviousDay();
	db.CalculateCalendar(loc, vc2, 9);

	int i = db.FindDate(vc);
	VAISNAVADAY * p = db.GetDay(i);

	if (p == NULL)
		return;

	fprintf(f, "<html>\n<head>\n<title></title>");
	fprintf(f, "<style>\n<!--\nbody {\n  font-family:Verdana;\n  font-size:9.5pt;\n}\n\ntd.hed {\n  font-size:9.5pt;\n  font-weight:bold;\n");
	fprintf(f, "  background:#aaaaaa;\n  color:white;\n  text-align:center;\n  vertical-align:center;\n  padding-left:15pt;\n  padding-right:15pt;\n");
	fprintf(f, "  padding-top:5pt;\n  padding-bottom:5pt;\n}\n-->\n</style>\n");
	fprintf(f, "</head>\n");
	fprintf(f, "<body>\n");
	fprintf(f, "<h2>%d %s %d  %s</h2>\n", vc.day, AvcGetMonthAbr(vc.month), vc.year, GetDayHumanTitle(vc));
	fprintf(f, "<h4>%s</h4>\n", loc.m_strFullName.c_str());
	fprintf(f, "<p>  %s, %s %s<br>  %s %s, %d Gaurabda</p>",
		GetTithiName(p->astrodata.nTithi), GetPaksaName(p->astrodata.nPaksa), gstr[20].c_str(), 
		GetMasaName(p->astrodata.nMasa), gstr[22].c_str(), p->astrodata.nGaurabdaYear);

	int prevCountFest = 0;

	if (p->ekadasi_parana)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br>");
		p->GetTextEP(str2);
		fprintf(f, "<span style=\'color:red;font-weight:bold\'>%s</span>", str2.c_str());
		prevCountFest++;
	}

	// adding mahadvadasi
	// adding spec festivals

	if (p->festivals.GetLength() > 0)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		int i = p->GetHeadFestival();
		while(p->GetNextFestival(i, str2))
		{
			if (str2.GetLength() > 1)
			{
				nFestClass = p->GetFestivalClass(str2);
				if (nFestClass < 0 || GetShowSetVal(22 + nFestClass) == 1)
				{
					if (prevCountFest > 0)
						fprintf(f, "<br>");
					fprintf(f, "%s", str2.c_str());
					prevCountFest++;
				}
			}
		}
	}


	if (GetShowSetVal(16) == 1 && p->sankranti_zodiac >= 0)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br>");
		//double h1, m1;
		//m1 = modf(p->sankranti_day.shour*24, &h1);
		fprintf(f, "<span style=\'color:#110033\'>%s %s (%s %s %s %d %s, %02d:%02d %s)</span>"
			, GetSankrantiName(p->sankranti_zodiac)
			, gstr[56].c_str()
			, gstr[111].c_str(), GetSankrantiNameEn(p->sankranti_zodiac)
			, gstr[852].c_str()
			, p->sankranti_day.day, AvcGetMonthAbr(p->sankranti_day.month)
			, p->sankranti_day.GetHour(), p->sankranti_day.GetMinuteRound()
			, AvcGetDSTSignature(p->nDST));
		prevCountFest++;
	}

	if (GetShowSetVal(7) == 1 && p->was_ksaya)//(m_dshow.m_info_ksaya) && (pvd->was_ksaya))
	{
		double h, m;
		VCTIME dd;
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br>");
		m = modf(p->ksaya_time1*24, &h);
		dd = p->date;
		if (p->ksaya_day1 < 0.0)
			dd.PreviousDay();
		fprintf(f, "%s %s ", gstr[89].c_str(), gstr[850].c_str());
		fprintf(f, "%d %s, %02d:%02d", dd.day, AvcGetMonthAbr(dd.month), int(h), int(m*60));
		m = modf(p->ksaya_time2*24, &h);
		dd = p->date;
		if (p->ksaya_day2 < 0.0)
			dd.PreviousDay();
		fprintf(f, "%s %d %s, %02d:%02d", gstr[851].c_str(),
			dd.day, AvcGetMonthAbr(dd.month), int(h), int(m*60));
		fprintf(f, "(%s)", AvcGetDSTSignature(p->nDST));
		prevCountFest++;
	}

	if (GetShowSetVal(8) == 1)//(m_dshow.m_info_vriddhi) && (pvd->is_vriddhi))
	{
		if (p->is_vriddhi)
		{
			if (prevCountFest == 0)
				fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
			else
				fprintf(f, "<br>");
			fprintf(f, "%s", gstr[90].c_str());
			prevCountFest++;
		}
	}


	if (p->nCaturmasya & CMASYA_PURN_MASK)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br><br>");
		fprintf(f, "%s [PURNIMA SYSTEM]"
			, gstr[107 + (p->nCaturmasya & CMASYA_PURN_MASK_DAY)
					   + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str()
			);
		if ((p->nCaturmasya & CMASYA_PURN_MASK_DAY) == 0x1)
		{
			fprintf(f, "<br>");
			fprintf(f, gstr[110 + ((p->nCaturmasya & CMASYA_PURN_MASK_MASA) >> 2)].c_str());
		}
		fprintf(f, "<br>");
		prevCountFest++;
	}

	if (p->nCaturmasya & CMASYA_PRAT_MASK)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br><br>");
		fprintf(f, "%s [PRATIPAT SYSTEM]"
			, gstr[107 + ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) >> 8)
					   + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str()
			);
		if ((p->nCaturmasya & CMASYA_PRAT_MASK_DAY) == 0x100)
		{
			fprintf(f, "<br>");
			fprintf(f, gstr[110 + ((p->nCaturmasya & CMASYA_PRAT_MASK_MASA) >> 10)].c_str());
		}
		fprintf(f, "</br>");
		prevCountFest++;
	}

	if (p->nCaturmasya & CMASYA_EKAD_MASK)
	{
		if (prevCountFest == 0)
			fprintf(f, "<table style=\'border-width:1pt;border-color:black;border-style:solid\'><tr><td style=\'font-size:9pt;background:%s;padding-left:25pt;padding-right:35pt;padding-top:15pt;padding-bottom:15pt;vertical-align:center\'>\n", gcal_html_getDayBackground(p));
		else
			fprintf(f, "<br><br>");
		fprintf(f, "%s [EKADASI SYSTEM]"
			, gstr[107 + ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) >> 16)
					   + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str()
			);
		if ((p->nCaturmasya & CMASYA_EKAD_MASK_DAY) == 0x10000)
		{
			fprintf(f, "<br>");
			fprintf(f, gstr[110 + ((p->nCaturmasya & CMASYA_EKAD_MASK_MASA) >> 18)].c_str());
		}
		fprintf(f, "<br>");
		prevCountFest++;
	}

	if (prevCountFest > 0)
		fprintf(f, "</td></tr></table>\n");

	fprintf(f, "<p>");
	// tithi at arunodaya
	if (GetShowSetVal(0) == 1)//m_dshow.m_tithi_arun)
	{
		fprintf(f, "<br>%s: %s", gstr[98].c_str(), GetTithiName(p->astrodata.nTithiArunodaya));
	}

	//"Arunodaya Time",//1
	if (GetShowSetVal(1) == 1)//m_dshow.m_arunodaya)
	{
		fprintf(f, "<br>%s %d:%02d (%s)\r\n", gstr[99].c_str(), p->astrodata.sun.arunodaya.hour
			, p->astrodata.sun.arunodaya.min, AvcGetDSTSignature(p->nDST));
	}

	//"Moonrise Time",//4
	if (GetShowSetVal(4) == 1)
	{
		fprintf(f, "<br>");
		if (p->moonrise.hour < 0)
			fprintf(f, gstr[136].c_str());
		else
		{
			fprintf(f, "%s %d:%02d (%s)", gstr[53].c_str(), p->moonrise.hour
				, p->moonrise.min, AvcGetDSTSignature(p->nDST));
		}
	}
	//"Moonset Time",//5
	if (GetShowSetVal(5) == 1)
	{
		if (GetShowSetVal(4) == 1)
			fprintf(f, "&nbsp;&nbsp;&nbsp;");
		else
			fprintf(f, "<br>");
		if (p->moonrise.hour < 0)
			fprintf(f, gstr[137].c_str());
		else
		{
			fprintf(f, "%s %d:%02d (%s)", gstr[54].c_str(), p->moonset.hour
				, p->moonset.min, AvcGetDSTSignature(p->nDST));
		}
	}
	///"Sun Longitude",//9
	if (GetShowSetVal(9) == 1)//m_dshow.m_sun_long)
	{
		fprintf(f, "<br>%s: %f (*)\r\n", gstr[100].c_str(), p->astrodata.sun.longitude_deg);
	}
	//"Moon Longitude",//10
	if (GetShowSetVal(10) == 1)//m_dshow.m_sun_long)
	{
		if (GetShowSetVal(9)==1)
			fprintf(f, ", ");
		else
			fprintf(f, "<br>");
		fprintf(f, "%s: %f (*)\r\n", gstr[101].c_str(), p->astrodata.moon.longitude_deg);
	}
	//"Ayanamsha value",//11
	if (GetShowSetVal(11) == 1)//m_dshow.m_sun_long)
	{
		if (GetShowSetVal(9)==1 || GetShowSetVal(10)==1)
			fprintf(f, ", ");
		else
			fprintf(f, "<br>");
		fprintf(f, "<br>%s %f (%s) (*)\r\n", gstr[102].c_str(), p->astrodata.msAyanamsa, GetAyanamsaName(GetAyanamsaType()));
	}
	//"Julian Day",//12
	if (GetShowSetVal(12) == 1)//m_dshow.m_sun_long)
	{
		if (GetShowSetVal(9)==1 || GetShowSetVal(10)==1 || GetShowSetVal(11)==1 )
			fprintf(f, ", ");
		else
			fprintf(f, "<br>");
		fprintf(f, "<br>%s %f (*)\r\n", gstr[103].c_str(), p->astrodata.jdate);
	}

	/*BEGIN GCAL 1.4.3*/
	DAYTIME tdA, tdB;

	if (GetShowSetVal(29))
	{
		fprintf(f, "<br>%s %2d:%02d ",
			gstr[51].c_str(), p->astrodata.sun.rise.hour, p->astrodata.sun.rise.min );
		if (GetShowSetVal(32))
		{
			tdA = p->astrodata.sun.rise;
			tdB = p->astrodata.sun.rise;
			tdA -= 24;
			tdB += 24;
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)", AvcGetDSTSignature(p->nDST));
	}
	if (GetShowSetVal(30))
	{
		fprintf(f, "<br>%s    %2d:%02d ", gstr[857].c_str(), p->astrodata.sun.noon.hour, p->astrodata.sun.noon.min);
		if (GetShowSetVal(32))
		{
			tdA = p->astrodata.sun.noon;
			tdB = p->astrodata.sun.noon;
			tdA -= 24;
			tdB += 24;
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)\r\n", AvcGetDSTSignature(p->nDST));
	}
	if (GetShowSetVal(31))
	{
		fprintf(f, "<br>%s  %2d:%02d ", gstr[52].c_str(), p->astrodata.sun.set.hour, p->astrodata.sun.set.min);
		if (GetShowSetVal(32))
		{
			tdA = p->astrodata.sun.set;
			tdB = p->astrodata.sun.set;
			tdA -= 24;
			tdB += 24;
			fprintf(f, " sandhya %2d:%02d - %2d:%02d ", tdA.hour, tdA.min, tdB.hour, tdB.min);
		}
		fprintf(f, " (%s)\r\n", AvcGetDSTSignature(p->nDST));
	}
	if (GetShowSetVal(33))
	{
		fprintf(f,"<br>%s info  Moon in %s %s",
			gstr[51].c_str(), 
			GetNaksatraName(p->astrodata.nNaksatra), gstr[15].c_str());
		if (GetShowSetVal(47))
		{
			fprintf(f, ", %.1f%% passed (%s Pada)", p->astrodata.nNaksatraElapse, gstr[811+int(p->astrodata.nNaksatraElapse/25)].c_str());
		}
		if (GetShowSetVal(46))
		{
			fprintf(f, ", Moon in the %s %s", GetSankrantiName(p->astrodata.nMoonRasi), gstr[105].c_str());
		}
		fprintf(f, ", %s %s<br>Sun in %s %s.",
			GetYogaName(p->astrodata.nYoga), gstr[104].c_str(),
			GetSankrantiName(p->astrodata.nSunRasi), gstr[105].c_str());
	}
	fprintf(f, "</p>");
	fprintf(f, "</body>");
	fprintf(f, "</html>");
	/* END GCAL 1.4.3 */

}

void AvcTransTodayInfoHTML(const char * szLocName, const char * szCountry, double Latitude, double Longitude, int timezoneID, int day, int month, int year, FILE * f)
{
	CLocationRef loc;
	VCTIME vc;
	
	loc.m_strName = szLocName;
//	loc.m_strCountry = szCountry;
	loc.m_fLatitude = Latitude;
	loc.m_fLongitude = Longitude;
	loc.m_nDST = timezoneID;
	loc.m_fTimezone = TTimeZone::GetTimeZoneOffset(timezoneID);
	
	vc.day = day;
	vc.month = month;
	vc.year = year;
	vc.shour = 0.5;
	vc.tzone = loc.m_fTimezone;
	
	WriteTodayInfoHTML(vc, loc, f);
}

void AvcTransAppDayInfoHTML(const char * szLocName, const char * szCountry, double Latitude, double Longitude, int timezoneID, int day, int month, int year, int hour, int minute, FILE * f)
{
	CLocationRef loc;
	VCTIME vc;
	
	loc.m_strName = szLocName;
	//loc.m_strCountry = szCountry;
	loc.m_fLatitude = Latitude;
	loc.m_fLongitude = Longitude;
	loc.m_nDST = timezoneID;
	loc.m_fTimezone = TTimeZone::GetTimeZoneOffset(timezoneID);
	
	vc.day = day;
	vc.month = month;
	vc.year = year;
	vc.shour = (hour*60 + minute)/1440.0;
	vc.tzone = loc.m_fTimezone;
	
	TResultApp app;
	
	CalcAppDay(loc, vc, app);
	
	WriteAppDayHTML(app, f);
}

void AvcTransCalendarHTML(const char * szLocName, const char * szCountry, double Latitude, double Longitude, int timezoneID, int day, int month, int year, int count, FILE * f)
{
	CLocationRef loc;
	VCTIME vc;
	
	loc.m_strName = szLocName;
	//loc.m_strCountry = szCountry;
	loc.m_fLatitude = Latitude;
	loc.m_fLongitude = Longitude;
	loc.m_nDST = timezoneID;
	loc.m_fTimezone = TTimeZone::GetTimeZoneOffset(timezoneID);
	
	vc.day = day;
	vc.month = month;
	vc.year = year;
	vc.shour = 0.5;
	vc.tzone = loc.m_fTimezone;
	
	TResultCalendar cal;
	
	CalcCalendar(cal, loc, vc, count);
	WriteCalendarHTML(cal, f);
}


void AvcTransMasaListHTML(const char * szLocName, const char * szCountry, double Latitude, double Longitude, int timezoneID, int year, int count, FILE * f)
{
	CLocationRef loc;
	VCTIME vc;
	
	loc.m_strName = szLocName;
	//loc.m_strCountry = szCountry;
	loc.m_fLatitude = Latitude;
	loc.m_fLongitude = Longitude;
	loc.m_nDST = timezoneID;
	loc.m_fTimezone = TTimeZone::GetTimeZoneOffset(timezoneID);
	
	TResultMasaList mlist;
	
	CalcMasaList(mlist, loc, year, count);
	WriteMasaListHTML(mlist, f);
}

void AvcTransEventsHTML(const char * szLocName, const char * szCountry, double Latitude, double Longitude, int timezoneID, int day, int month, int year, int count, int options, FILE * f)
{
	CLocationRef loc;
	VCTIME vc;
	VCTIME vcEnd;
	
	loc.m_strName = szLocName;
	//loc.m_strCountry = szCountry;
	loc.m_fLatitude = Latitude;
	loc.m_fLongitude = Longitude;
	loc.m_nDST = timezoneID;
	loc.m_fTimezone = TTimeZone::GetTimeZoneOffset(timezoneID);
	
	vc.day = day;
	vc.month = month;
	vc.year = year;
	vc.shour = 0.5;
	vc.tzone = loc.m_fTimezone;
	
	vcEnd = vc;
	vcEnd += count;
	
	TResultEvents cal;
	
	CalcEvents(cal, loc, vc, vcEnd, options);
	WriteEventsHTML(cal, f);
}
