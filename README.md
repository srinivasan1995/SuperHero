# SuperHero Retrieveal Information System 
a Webserver written in c++ 
default port specified is 8000 , you can change port in code in the file main.cpp (static const char* s_http_port = "8000").

if API provides issue try registering  to API again (go to superheroapi.com and register via facebook id to get access token and subsitute url with access token ). 

static const char* url = "https://superheroapi.com/api.php/10206518162748484/search/";

program is  run with help of Visual studio (needs client application to see visual output ).

uses http://superheroapi.com's API to access super hero related info and sends to client . 

it uses openssl and mongoose library  to establish connection with REST API .

to run the server application 
1)openssl library has to be added C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.22.27905\include.
2)mongoose library is present as part of src and include  directories . 
3) if visual studio is present it is easier to run application . 

CmakeLists for project is already added , when executed via visual studio , it should produce build which would be executed by Ninja Builder . 

if there is response timeout in API , do exit the application and restart it . (i did face this issue few times , looks like the API takes longer time ).


feel free to ask any queries and happy experimenting !!!!!!!!!!!!!!!!!







