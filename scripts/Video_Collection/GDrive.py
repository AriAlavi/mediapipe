#GoogleDriveDatabase class by Arian Alavi
#Modifications by John Kirchner


from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive
from oauth2client.client import GoogleCredentials
import uuid
import os
import json
import time

class GoogleDriveDatabase:
  def __init__(self, drive, DATABASE_GID:str):
    assert isinstance(DATABASE_GID, str)
    self.folders = {}
    self.drive = drive
    self.database_gid = DATABASE_GID
    folder_list = drive.ListFile({'q': "'{}' in parents and trashed=false".format(DATABASE_GID)}).GetList()
    self.update_folders(drive)
    print("{} folders loaded".format(len(self.folders.keys())))
  def update_folders(self,drive):
    folder_list = drive.ListFile({'q': "'{}' in parents and trashed=false".format(self.database_gid)}).GetList()
    self.folders = {}
    for file in folder_list:
      if file['mimeType'] == "application/vnd.google-apps.folder":
        self.folders[file['title']] = file['id']
    return self.folders

  def upload(self, filename, character, fileType):
    assert isinstance(filename, str)
    assert isinstance(character, str)
    assert isinstance(fileType, str)
    FILETYPE_MIME_MAP = {
        "jpeg" : "image/jpeg",
        "json" : "application/json",
        "zip" : "application/zip",
        "avi" : "video/avi"
    }
    assert fileType in FILETYPE_MIME_MAP.keys(), "fileType must be one of the following: {}".format(fileType)
    assert os.path.isfile(filename), "{} does not exist as a file".format(filename)
    assert self.checkFolder(character), "{} is not a valid character. Pick from list: \n{}".format(character, tuple(self.folders.keys()))
    file = self.drive.CreateFile({
        "title" :  os.path.split(filename)[1],
        "mimeType" : FILETYPE_MIME_MAP[fileType],
        "parents" : [{"id" : self.folders[character]}]
    })
    file.SetContentFile(filename)
    file.Upload()
    print("uploaded {}".format(filename))

  @staticmethod
  def allImagesToJPG(fileName) -> str:
    assert isinstance(fileName, str)
    assert any(extension for extension in GoogleDriveDatabase.VIDEO_EXTENSIONS()), "{} is not a valid image".format(fileName)
    if ".avi" in fileName:
      return fileName
    return fileName.split(".")[0] + ".avi"
    
  @staticmethod
  def VIDEO_EXTENSIONS() -> list:
    return [".avi"]

  @staticmethod
  def FILE_EXTENSIONS() -> list:
    return [".jpg", ".jpeg", ".png", ".zip", ".json"]

  def getFiles(self, character) -> list:
    return [
      x for x in self.drive.ListFile({'q': "'{}' in parents and trashed=false".format(self.folders[character])}).GetList()
      if x['mimeType'] != "application/vnd.google-apps.folder" 
    ]
  def download_file_name(self, file_name):
      if not any(extension in file_name for extension in GoogleDriveDatabase.FILE_EXTENSIONS()):
        file_name += ".avi"
      return GoogleDriveDatabase.allImagesToJPG(file_name)

  def download_file(self, file, folder:str, **kwargs) -> str:
      check_already_exist = kwargs.get("check_local", False)
      file_name = os.path.join(folder, file['title'])
      file_name = self.download_file_name(file_name)
      if check_already_exist:
        local_files = [os.path.join(folder, file) for file in os.listdir(folder)]
        if file_name in local_files:
          return file_name
      file.GetContentFile(file_name)
      print("downloaded", file_name)
      return file_name

  def download(self, character:str,folder:str):
    file_list = self.getFiles(character)
    os.makedirs(folder, exist_ok=True)
    returnlist = []
    
    for file in file_list:
      returnlist.append(self.download_file(file, folder, check_local=True))
    return tuple(returnlist)

  def checkFolder(self, name):
    assert isinstance(name, str)
    return name in self.folders.keys()

  def createFolder(self, name, exist_ok=True):
    if self.checkFolder(name):
      if exist_ok:
        return True
      raise Exception("Folder {} already exists".format(name))
    self.drive.CreateFile({
        "title" : name,
        "mimeType" : "application/vnd.google-apps.folder",
        "parents" : [{"id" : self.database_gid}]
    }).Upload()
  def move_file(self, file_obj, newFolder):
    assert isinstance(newFolder, str)
    assert self.checkFolder(newFolder)
    files = self.drive.auth.service.files()
    file = files.get(fileId=file_obj['id'], fields ='parents').execute()
    prev_parents = ','.join(p['id'] for p in file.get('parents'))
    file = files.update(
        fileId = file_obj['id'],
        addParents = self.folders[newFolder],
        removeParents = prev_parents,
        fields = 'id, parents',
    ).execute()
    return file
  def trash(self, file_obj):
    self.move_file(file_obj, "TRASH")
    print("Sent {} to trash".format(file_obj['title']))
  
def authenticate():
  #auth.authenticate_user() # Google auth stuff, make sure to sign in with your ucsb account
  gauth = GoogleAuth()
  gauth.LocalWebserverAuth()
  drive = GoogleDrive(gauth) # Google auth stuff
  return drive



