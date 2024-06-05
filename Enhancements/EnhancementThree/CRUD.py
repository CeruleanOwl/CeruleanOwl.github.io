from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self, USER='', PASS=''):
        # Initializing the MongoClient. This helps to 
        # access the MongoDB databases and collections.
        # This is hard-wired to use the aac database, the 
        # animals collection, and the aac user.
        # Definitions of the connection string variables are
        # unique to the individual Apporto environment.
        #
        # You must edit the connection variables below to reflect
        # your own instance of MongoDB!
        #
        # Connection Variables
        #
        HOST = 'localhost'
        PORT = 27017
        DB = 'Animals'
        COL = 'Animals'
        #
        # Initialize Connection
        #
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (USER,PASS,HOST,PORT))
        self.database = self.client['Animals']
        self.collection = self.database['Animals']

# Complete this create method to implement the C in CRUD.
    def create(self, data: dict):
        if data is not None:
            if type(data) != dict:
                return False
            else:
                insertPassed = self.database.Animals.insert_one(data)  # data should be dictionary  
                return True
        else:
            raise Exception("Nothing to save, because data parameter is empty")

# Create method to implement the R in CRUD.
    def read(self, readData=None):
        if readData:
            data = self.database.Animals.find(readData, {"_id": False})
        else:
            data = self.database.Animals.find({}, {"_id": False})
            
        return data

# Create method to implement the U in CRUD.
    def update(self, readData, updateData=None):
        if readData is not None:
            if type(readData) != dict:
                return 0
            else:
                results = self.database.Animals.update_many(readData, {"$set": updateData})
                return results.modified_count #access cursors associated numbers documents changed
        else:
            raise Exception("Nothing to update, because data parameter is empty")
            
# Create method to implement the D in CRUD.
    def delete(self, deleteData=None):
        if deleteData is not None:
            if type(deleteData) != dict:
                return 0
            else:
                results = self.database.Animals.delete_many(deleteData)
                deleted_count = results.deleted_count #access cursors associated numbers documents changed
                return deleted_count
        else:
            raise Exception("Nothing to delete, because data parameter is empty")