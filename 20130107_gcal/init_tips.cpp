#include "stdafx.h"

void InitTips(FILE * hFile)
{
	const char * p[] = {
		"To calculate a full calendar for any location, press F8, or choose Calendar from the Calculate menu.",
		"Whenever you start GCal it will first show you today's events for the default location. Be sure to tell GCal where you are in the world - choose My Location from the Settings menu.",
		"You can quickly see the events coming up in the next few days at your location. From the Today screen, press F7. To move to the day before, press F5. To return to the current day, press F6.",
		"Screen output formatting can be customized to your requirements. From the Settings menu, toggle between Normal View and Enhanced View, or choose a different text size.",
		"GCal has about 2500 locations in its database, but many smaller cities are not listed. You can choose a nearby larger city, or you can easily add a new city. From the File menu, choose Location Manager, and click New City. You'll need to enter latitude, longitude, and time zone information. Don't know the latitude, longitude, and time zone? Try Wikipedia or the local library.",
		"Print your calculated calendar in wall calendar format for easy reference. With your calculated calendar showing in the main program window, choose File/Save Content. From the File Type drop down box in the Save As dialog box, choose HTML File (in Table format).",
		"You can add observance days to your calendars - your guru's Vyasa Puja day, family birthdays - any event important to you! From the Settings menu, choose My Events, then click Add Event. If you don't know the masa and tithi information, then you will need the location of the event and the date and time that it occurred.",
		"At any time you can save the contents of the main display window for future reference. From the File menu, choose Save Content. Or to keep a hard copy, choose Print.",
		"The Event Finder feature could save you from visually scanning pages of calendars to find, for example, the dates of the last 25 Janmastamis, or all the fast days for the next year. Just press Ctrl-F. Once you find an event you are looking for, choose the Calendar Context tab to see the surrounding days.",
		"Gcal has a lot of options to show particular astronomical or calculation information on the calendar output pages. Or, to streamline your calendar you can remove unneeded types of event notifications. From the Settings menu check out Calendar Display.",
		"GCal has facility to output in multiple languages, but it requires some effort to set things up. If you would like to volunteer for set up work, please contact the KrishnaDays team.",

		NULL,
		NULL
	};

	int i;
	for(i = 0; p[i] != NULL; i++)
	{
		fprintf(hFile, "%s\n", p[i]);
	}
}
