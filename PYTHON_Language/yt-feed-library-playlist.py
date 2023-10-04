#!/usr/bin/env python3
# this simple script get all videos from private playlists (feed/library)

import os
import json

import google.oauth2.credentials

from googleapiclient.discovery import build
from googleapiclient.errors import HttpError
from google_auth_oauthlib.flow import InstalledAppFlow

from bs4 import BeautifulSoup

# The CLIENT_SECRETS_FILE variable specifies the name of a file that contains
# the OAuth 2.0 information for this application, including its client_id and
# client_secret.
CLIENT_SECRETS_FILE = "secret.json"

# This OAuth 2.0 access scope allows for full read/write access to the
# authenticated user's account and requires requests to use an SSL connection.
SCOPES = ['https://www.googleapis.com/auth/youtube.force-ssl']
API_SERVICE_NAME = 'youtube'
API_VERSION = 'v3'

def get_authenticated_service():
  flow = InstalledAppFlow.from_client_secrets_file(CLIENT_SECRETS_FILE, SCOPES)
  #credentials = flow.run_console()
  credentials = flow.run_local_server()
  return build(API_SERVICE_NAME, API_VERSION, credentials = credentials)

def playlistitems_list(service, **kwargs):
  results = service.playlistItems().list(
    **kwargs
  ).execute()
  return (results)

if __name__ == '__main__':

  plid = ""
  vid = ""
  
  # When running locally, disable OAuthlib's HTTPs verification. When
  # running in production *do not* leave this option enabled.
  os.environ['OAUTHLIB_INSECURE_TRANSPORT'] = '0'

  service = get_authenticated_service()

  #content after login to page https://www.youtube.com/feed/library
  with open('yt-feed-library-playlist.txt', 'rt') as fp:
    html_string = fp.read()

  soup = BeautifulSoup(html_string, 'html.parser')
  json_data = soup.find_all('script')[34].get_text();
  json_data = json_data[len("\nvar ytInitialData ="):];
  json_data = json_data[:len(json_data)-1];
  #print (json_data) # debug
  json_data = json.loads(json_data)
  
  plitems = json_data \
  ['contents'] \
  ['twoColumnBrowseResultsRenderer'] \
  ['tabs'] \
  [0] \
  ['tabRenderer'] \
  ['content'] \
  ['sectionListRenderer'] \
  ['contents'] \
  [2] \
  ['itemSectionRenderer'] \
  ['contents'] \
  [0] \
  ['shelfRenderer'] \
  ['content'] \
  ['gridRenderer'] \
  ['items'];
  for pi in plitems:
    plid = pi \
    ['gridPlaylistRenderer'] \
    ['playlistId'];
    pltitle = pi \
    ['gridPlaylistRenderer'] \
    ['title'] \
    ['simpleText'];

    print (f"{pltitle} - {plid}")  
  
    result = playlistitems_list(
      service,
      part='contentDetails,snippet',
      fields='nextPageToken,items/contentDetails/videoId,items/snippet/channelTitle,items/snippet/title',
      playlistId=plid,
      maxResults=50
    )

    json_data = json.dumps(result)
    json_data = json.loads(json_data)

    items1 = json_data['items']

    for i in range (0, len(items1)):
      vid = items1[i]['contentDetails']['videoId'];
      vtitle = items1[i]['snippet']['title'];
      print (f"{pltitle} - {vtitle} - {vid}")

    try:

      prevPageToken = nextPageToken = json_data['nextPageToken']

      while nextPageToken:
        result = playlistitems_list(
          service,
          part='contentDetails,snippet',
          fields='nextPageToken,items/contentDetails/videoId,items/snippet/channelTitle,items/snippet/title',
          playlistId=plid,
          pageToken=nextPageToken,
          maxResults=50
        )
        
        json_data = json.dumps(result)
        json_data = json.loads(json_data)
        
        items1 = json_data['items']
        
        for i in range (0, len(items1)):
          vid = items1[i]['contentDetails']['videoId'];
          vtitle = items1[i]['snippet']['title'];
          print (f"{pltitle} - {vtitle} - {vid}")

        if 'nextPageToken' in json_data:
          nextPageToken = json_data['nextPageToken']
        else:
          break

      print(f"++++++++++\t{plid} items > 50")
    
    except KeyError:
      print(f"----------\t{plid} items <= 50")

