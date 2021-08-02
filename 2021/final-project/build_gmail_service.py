from __future__ import print_function
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

# -------1---------2---------3---------4---------5---------6---------7---------8
def build_gmail_service():
    """Builds service for use with Gmail API"""
    creds = None
    # adapted from Gmail Python API quickstart
    # https://developers.google.com/gmail/api/quickstart/python
    #
    # The file token.json stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    token_fullpath = './.secrets/token.json'
    cred_fullpath = './.secrets/credentials.json'

    if os.path.exists(token_fullpath):
        creds = Credentials.from_authorized_user_file(token_fullpath, SCOPES)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(cred_fullpath
                                                            , SCOPES)
            creds = flow.run_local_server(port=0)
        # Save the credentials for the next run
        with open(token_fullpath, 'w') as token:
            token.write(creds.to_json())

    service = build('gmail', 'v1', credentials=creds)
    return service
    