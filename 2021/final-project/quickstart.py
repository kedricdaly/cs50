from __future__ import print_function
import os.path
import re
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from timeit import default_timer as timer
import requests as req
import time


# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

def main():
    """Shows basic usage of the Gmail API.
    """
    creds = None
    # The file token.json stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('token.json', SCOPES)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                './final-project/.secrets/credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
        # Save the credentials for the next run
        with open('token.json', 'w') as token:
            token.write(creds.to_json())

    service = build('gmail', 'v1', credentials=creds)

    # Call the Gmail API
    msg_list = list_messages(service)

    messages_found = False;
    if msg_list:
        messages_found = True
    """
    if not msg_list:
        print('No messages found.')
    else:
        print('Message list:')
        messages_found = True;
        for msg in msg_list:
            print(msg['id'])
    """

    email_dict = {}
    message_headers = []
    apiTimer = 0
    appendTimer = 0

    if messages_found:
        iMsg = 0;
        for msg in msg_list:
            apiStart = timer()
            message = service.users().messages().get(userId='me', id=msg['id'], format='metadata', metadataHeaders=["Delivered-To"]).execute()
            apiEnd = timer()
            apiTimer += apiEnd - apiStart
            #header = message.get('headers',[])
            appendStart = timer()
            # only append messages with a 'Delivered-To' header
            if 'headers' in message['payload']:
                message_headers.append(message['payload']['headers'])
            appendEnd = timer()
            appendTimer += appendEnd - appendStart
            #iMsg += 1
            #if iMsg > 1:
            #    break
    
    iMsg = 0;

    email_list = map(lambda x: x[0]['value'].lower(), message_headers)
    unique_emails = list(set(email_list))
    unique_emails.sort()
    print(f"Identified {len(unique_emails)} unique emails in {len(msg_list)} messages: \n")
    for email in unique_emails:
        print(email)
    
    print("\n", end="")

    """
    for msg in message_headers:
        if iMsg % 79 == 0:
            print(".", end="")
            #print("\n")
        for header in msg:
            # underlying_dict = header[0]
            if header['name'] == 'Delivered-To':
                email_dict[header['value'].lower()] = 1
                break
            #elif header['name'] == 'To':
            # This was only being used for sent messages, and we want to skip sent messages
            #    email_val = find_between(header['value'],'<','>')
            #    email_dict[email_val.lower()] = 1
            #    break
            else:
                # unknown delivery address
                a = 1
    
    # print out list of unique addresses (keys in the email_dict dictionary)
    print("\n")
    for key in email_dict.keys():
        print(key)
    """



def list_messages(service):
    # looping adapted from: https://stackoverflow.com/questions/57733991/get-all-emails-with-google-python-api-client
    MAX_MESSAGES = 10000
    query = 'not in:sent' # omit sent messages
    results = service.users().messages().list(userId='me', maxResults=500, includeSpamTrash=True).execute()
    msg_list = results.get('messages',[])

    nextPageToken = None
    if "nextPageToken" in results:
        nextPageToken = results['nextPageToken']

    while nextPageToken:
        results = service.users().messages().list(userId='me', maxResults=500, includeSpamTrash=True, pageToken=nextPageToken).execute()
        new_msg_list = results.get('messages',[])
        msg_list.extend(new_msg_list)
        if len(msg_list) >= MAX_MESSAGES:
            break
        if 'nextPageToken' in results:
            nextPageToken = results['nextPageToken']
        else:
            break
    #nextPageToken = results.get('nextPageToken')
    #print("nextPageToken: %s" % nextPageToken)
    #results2 = service.users().messages().list(userId='me', pageToken=nextPageToken, maxResults=500, includeSpamTrash=True).execute()
    #msg_list2 = results2.get('messages',[])
    #thirdPageToken = results2.get('nextPageToken')
    #print("thirdPageToken: %s" % thirdPageToken)
    #print('msg_list:\n')
    #msg_list.extend(msg_list2)
    #print("Len msg_list: %d" % len(msg_list))
    return msg_list
    #print(msg_list)
    

# https://stackoverflow.com/questions/3368969/find-string-between-two-substrings
def find_between(s, start, end):
    try:
        return (s.split(start))[1].split(end)[0]
    except:
        return s

if __name__ == '__main__':
    main()