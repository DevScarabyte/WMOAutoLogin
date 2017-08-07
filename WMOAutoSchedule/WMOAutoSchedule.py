from __future__ import print_function
import httplib2
import os
import re

from apiclient import discovery
from oauth2client import client
from oauth2client import tools
from oauth2client.file import Storage

import datetime

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()
except ImportError:
    flags = None

# If modifying these scopes, delete your previously saved credentials
# at ~/.credentials/calendar-python-quickstart.json
SCOPES = 'https://www.googleapis.com/auth/calendar',
'https://www.googleapis.com/auth/calendar.readonly'
CLIENT_SECRET_FILE = 'client_secret.json'
APPLICATION_NAME = 'Google Calendar API Python Quickstart'


def get_credentials():
    home_dir = os.path.expanduser('~')
    credential_dir = os.path.join(home_dir, '.credentials')
    if not os.path.exists(credential_dir):
        os.makedirs(credential_dir)
    credential_path = os.path.join(credential_dir,
                                   'calendar-python-quickstart.json')

    store = Storage(credential_path)
    credentials = store.get()
    if not credentials or credentials.invalid:
        flow = client.flow_from_clientsecrets(CLIENT_SECRET_FILE, SCOPES)
        flow.user_agent = APPLICATION_NAME
        if flags:
            credentials = tools.run_flow(flow, store, flags)
        else: # Needed only for compatibility with Python 2.6
            credentials = tools.run(flow, store)
        print('Storing credentials to ' + credential_path)
    return credentials

def main():
    credentials = get_credentials()
    http = credentials.authorize(httplib2.Http())
    service = discovery.build('calendar', 'v3', http=http)
    now = datetime.datetime.utcnow().isoformat() + 'Z' # 'Z' indicates UTC time
    #created_event = service.events().quickAdd(       
    #calendarId='primary',
    #text='Test at address street city state July 24th 10am-10:25am').execute()
    #print (created_event['id']) 
    loc = open("OtherInfo.txt","r")
    address = loc.readline()
    calID = loc.readline()
    loc.close()
    file = open("Shifts.txt","r")
    shifts = list()
    for line in file:
        subLines = line.split( )
        year = subLines[2]
        month = subLines[1]
        day = subLines[0]
        intMonth = 0;
        if month == "January":
            intMonth = 1;
        elif month == "February":
            intMonth = 2;
        elif month == "March":
            intMonth = 3;
        elif month == "April":
            intMonth = 4;
        elif month == "May":
            intMonth = 5;
        elif month == "June":
            intMonth = 6;
        elif month == "July":
            intMonth = 7;
        elif month == "August":
            intMonth = 8;
        elif month == "September":
            intMonth = 9;
        elif month == "October":
            intMonth = 10;
        elif month == "November":
            intMonth = 11;
        elif month == "December":
            intMonth = 12;
        sDate = year + '-' + str(intMonth) + '-' + day + 'T00:00:00Z'
        eDate = year + '-' + str(intMonth) + '-' + day + 'T23:59:59Z'
        eventResults = service.events().list(calendarId=calID, timeMin=sDate, timeMax=eDate).execute()
        events = eventResults.get('items', [])
        # CASHIER July 25 2017 9:30am-1:30pm at 4975 Transit Road Depew NY
        if not events:
            if len(subLines) == 9:
                shifts.append(subLines[8] + " " + month + " " + day + " " + year + " " + subLines[3] + "-" + subLines[5] + " at " + address)
            elif len(subLines) == 11:
                shifts.append(subLines[10] + " " + month + " " + day + " " + year + " " + subLines[3] + "-" + subLines[5] + " at " + address)
                shifts.append("Meal " + month + " " + day + " " + year + " " + subLines[7] + "-" + subLines[9] + " at " + address)
            else:
                print("Error")
    
    file.close()

    print("Shifts to be added: " + str(len(shifts)))
    for shift in shifts:
        print(shift)
        created_event = service.events().quickAdd(       
        calendarId=calID,
        text=shift).execute()
        print("Google Calendar Event ID: " + created_event['id']) 

if __name__ == '__main__':
    main()
