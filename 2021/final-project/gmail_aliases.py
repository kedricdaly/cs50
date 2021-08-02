from __future__ import print_function
import os.path
import argparse
from timeit import default_timer as timer
from build_gmail_service import build_gmail_service

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

# -------1---------2---------3---------4---------5---------6---------7---------8
def main():
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--max_messages"
                        , type=int
                        , default=500
                        , help="Maximum number of messages to search in rev. \
                                chron. order. Time is approximately 10 \
                                messages/second. Def: 500")
    parser.add_argument("-s", "--save_path"
                        , help="Saves a .txt file with unique emails to this provided fullpath. Creates dir if necessary"
                        )
    args = parser.parse_args()
    max_messages = args.max_messages
    save_path = args.save_path

    service = build_gmail_service()

    # Call the Gmail API to generate a list of messages
    msg_list = get_message_list(service, max_messages=max_messages)

    if msg_list:
        message_headers = get_message_headers(service, msg_list)
    else:
        print("No messages found.")
        return
    
    output_email_list(message_headers, len(msg_list), save_path)


# -------1---------2---------3---------4---------5---------6---------7---------8
def output_email_list(message_headers, nMessages, save_filepath=False):
    email_list = map(lambda x: x[0]['value'].lower(), message_headers)
    unique_emails = list(set(email_list))
    unique_emails.sort()

    header_text = f"Identified {len(unique_emails)} unique emails in {nMessages} messages: \n"

    if save_filepath:
        fileparts = os.path.split(save_filepath)
        if not os.path.exists(fileparts[0]):
            # mkdir
            os.makedirs(fileparts[0], exist_ok=True)  
            
        # write to .txt
        with open(save_filepath, 'w') as f:
            f.write(header_text)
            for email in unique_emails:
                f.write("%s\n" % email)
        print("\nSaved {}".format(save_filepath))
    else:
        print("\n", end="")
        print(header_text)
        for email in unique_emails:
            print(email)
        
        print("\n", end="")

        input("Press enter to exit")


# -------1---------2---------3---------4---------5---------6---------7---------8
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


# -------1---------2---------3---------4---------5---------6---------7---------8
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

# -------1---------2---------3---------4---------5---------6---------7---------8
if __name__ == '__main__':
    main()