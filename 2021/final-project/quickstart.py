from __future__ import print_function
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials

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

    if messages_found:
        for msg in msg_list:
            message = service.users().messages().get(userId='me', id=msg['id'], format='metadata').execute()
            #header = message.get('headers',[])
            message_header = message.get('headers')
            print("header:\n")
            print(message_header)





def list_messages(service):
    results = service.users().messages().list(userId='me').execute()
    msg_list = results.get('messages',[])
    #print('msg_list:\n')
    return msg_list
    #print(msg_list)

if __name__ == '__main__':
    main()