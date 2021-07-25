from __future__ import print_function
import os.path
import argparse
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from timeit import default_timer as timer


# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--max_messages"
                        , type=int
                        , default=500
                        , help="Maximum number of messages to search in rev. \
                                chron. order. Time is approximately 10 \
                                messages/second. Def: 500")
    args = parser.parse_args()
    max_messages = args.max_messages

    service = build_gmail_service()

    # Call the Gmail API to generate a list of messages
    msg_list = get_message_list(service, max_messages=max_messages)

    if msg_list:
        message_headers = get_message_headers(service, msg_list)
    else:
        print("No messages found.")
        return
    
    output_email_list(message_headers, len(msg_list))

def output_email_list(message_headers, nMessages):
    email_list = map(lambda x: x[0]['value'].lower(), message_headers)
    unique_emails = list(set(email_list))
    unique_emails.sort()
    print("\n", end="")
    print(f"Identified {len(unique_emails)} unique emails in {nMessages} messages: \n")
    for email in unique_emails:
        print(email)
    
    print("\n", end="")

    input("Press enter to exit")

def get_message_headers(service, msg_list):
    message_headers = []
    BATCH_SIZE = 10
    apiTimer = 0
    appendTimer = 0
    iMsg = 1;

    for msg in msg_list:
        apiStart = timer()
        message = service.users().messages().get(userId='me', id=msg['id'], format='metadata', metadataHeaders=["Delivered-To"]).execute()
        apiEnd = timer()
        apiTimer += apiEnd - apiStart
        if iMsg % BATCH_SIZE == 0:
            print(".", end="", flush=True)
            if iMsg % (10 * BATCH_SIZE) == 0:
                if iMsg > 0:
                    print("{}\n".format(iMsg),end="", flush=True)
        appendStart = timer()
        # only append messages with a 'Delivered-To' header
        if 'headers' in message['payload']:
            message_headers.append(message['payload']['headers'])
        appendEnd = timer()
        appendTimer += appendEnd - appendStart
        iMsg += 1

    return message_headers

def build_gmail_service():
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
    return service



def get_message_list(service, max_messages=500):
    # looping adapted from: https://stackoverflow.com/questions/57733991/get-all-emails-with-google-python-api-client
    if max_messages >= 500:
        results = service.users().messages().list(userId='me', maxResults=500, includeSpamTrash=True).execute()
    else:
        results = service.users().messages().list(userId='me', maxResults=max_messages, includeSpamTrash=True).execute()
    msg_list = results.get('messages',[])

    if len(msg_list) >= max_messages:
        return msg_list

    nextPageToken = None
    if "nextPageToken" in results:
        nextPageToken = results['nextPageToken']

    remaining_messages = max_messages - len(msg_list)

    while nextPageToken:
        if remaining_messages >= 500:
            results = service.users().messages().list(userId='me', maxResults=500, includeSpamTrash=True, pageToken=nextPageToken).execute()
        else:
            results = service.users().messages().list(userId='me', maxResults=remaining_messages, includeSpamTrash=True, pageToken=nextPageToken).execute()

        new_msg_list = results.get('messages',[])
        msg_list.extend(new_msg_list)
        if len(msg_list) >= max_messages:
            break
        if 'nextPageToken' in results:
            nextPageToken = results['nextPageToken']
        else:
            break

    return msg_list

if __name__ == '__main__':
    main()