# importing the requests library 
import requests 

# defining the api-endpoint 
API_ENDPOINT = "http://localhost:8080/"

# your API key here 
API_KEY = "XXXXXXXXXXXXXXXXX"

# your source code here 
source_code = ''' 
print("Hello, world!") 
a = 1 
b = 2 
print(a + b) 
'''

# data to be sent to api 
json = {'api_dev_key':API_KEY, 
		'api_option':'paste', 
		'api_paste_code':source_code, 
		'api_paste_format':'python'} 

# sending post request and saving response as response object 
#r = requests.post(url = API_ENDPOINT, data = data) 

# extracting response text 
#pastebin_url = r.text 
#print("The pastebin URL is:%s"%pastebin_url) 

text = """random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 
random text  random text  random text  random text  random text  random text 

"""
f = open("image1.png","rb")
image = f.read()
f.close()


def send(inp,port=8081):    
    r = requests.post("http://localhost:"+str(port)+"/", data=inp )
    print(r.status_code, r.reason)
    print(r.text[:300] + '...')

def read():
    f = open("1.txt","rb")
    r = f.read(4)
    f.close()
    r.decode("ascii")
    print(r)
