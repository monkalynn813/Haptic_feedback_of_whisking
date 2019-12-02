

# importing the requests library 
import requests 
import json
import time

# defining the api-endpoint 
API_ENDPOINT = "http://192.168.1.1"

# your API key here 
API_KEY = "test_code"



# data to be sent to api 
# data = {'api_dev_key':API_KEY} 
data= [0,5]
msg=json.dumps(data).encode()
# sending post request and saving response as response object 
# while True:
r  = requests.get(url = API_ENDPOINT, data = msg) 
    # time.sleep(0.5)

