#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#endif

#ifndef _WIN32
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "Webserver.h"


/* TODO:
+Write demo app
add subscription
Add websocket support
+Add Menu support



*/


#define WEBSERWER_PORT 8888
#define WEBSERWER_INTERFACE INADDR_ANY

#define CORS_STUFF "Access-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET,POST\r\nAccess-Control-Allow-Headers: Content-Type\r\n"
#define HTTP_HEADER_OKAY "HTTP/1.0 200 Okaaay\r\nContent-Type: %s\r\nContent-Length: %d\r\nExpires: 0\r\nCache-Control: no-cache\r\nConnection: close\r\n" CORS_STUFF "\r\n"
#define HTTP_HEADER_REDIRECT "HTTP/1.0 301 Redirect\nLocation: /file/index.html\nConnection: close\n\n"

#define trace(x...) {fprintf(stderr,x);fprintf(stderr,"\n");}
#define crash(x...) {fprintf(stderr,"Aborted at line %s:%d\n",__FILE__,__LINE__);abort();}

enum{
HTTP_METHOD_UNKNOWN=0,
HTTP_METHOD_GET,
HTTP_METHOD_POST
};

#define PH_GET_ 23635
#define PH_POST_ 22708
#define PH_HOST_ 24286
#define PH_CONNECTION_ 22244
#define PH_PRAGMA_ 19712
#define PH_CACHE_CONTROL_ 25415
#define PH_USER_AGENT_ 21412
#define PH_UPGRADE_ 11617
#define PH_ORIGIN_ 15968
#define PH_SEC_WEBSOCKET_VERSION_ 1167
#define PH_ACCEPT_ENCODING_ 26281
#define PH_ACCEPT_LANGUAGE_ 23145
#define PH_SEC_WEBSOCKET_KEY_ 23851
#define PH_SEC_WEBSOCKET_EXTENSIONS_ 7570
#define PH_CONTENT_LENGTH_ 2728
#define PH_UPGRADE_INSECURE_REQUESTS_ 10611
#define PH_CONTENT_TYPE_ 25158
#define PH_ACCEPT_ 23328
#define PH_REFERER_ 9025
#define PH_APPLICATION_X_WWW_FORM_URLENCODED 1252
#define PH_MEMR 4533
#define PH_MEMW 4538
#define PH_MEMC 4518
#define PH_MEMS 4534
#define PH_EXIT 27959
#define PH_INFO 13650
#define PH_FIND 8775
#define PH_PING 8778
#define PH_SOCK 18670
#define PH_FILE 8584
#define PH_STOR 24181
#define PH_FUNC 21062
#define PH_SUBS 20694
#define PH_USUB 23205
#define PH_MENU 4696

// for MIME
#define PH__TXT 24343
#define PH__HTML 21072
#define PH__HTM 11920
#define PH__CSS 6902
#define PH__JS 12597
#define PH__JSON 24082
#define PH__PNG 20042
#define PH__JPG 9738
#define PH__JPEG 16715
#define PH__GIF 10537
#define PH__ICO 8434
#define PH__AVIF 27082
#define PH__BMP 1459
#define PH__WAV 26681
#define PH__MP3 12918
#define PH__MP4 12919
#define PH__WEBM 5361
#define PH__OGG 18794
#define PH__EXE 4872
#define PH__DLL 7567
#define PH__BIN 1329

#define ph_create(x) static int x##_val,x##_len;
#define ph_add(x,d) x##_val<<=5;x##_val^=d-(d>='a' && d<='z'?32:0);x##_val+=x##_len;x##_val&=0x7FFF;x##_len++;
//trace("hashing %c, val: %d",d,x##_val);
#define ph_reset(x) x##_val=0;x##_len=0;
#define ph_value(x) (x##_val)

pthread_t web_server;

void *(*menu_handler)(void);
uint8_t *recv_buf=NULL;
uint8_t *send_buf=NULL;
uint8_t *headers_buf=NULL;
uint8_t *storage=NULL;
uint32_t *funcs=NULL;
SUBS_ITEM *subs=NULL;


#define RECV_BUF_SIZE 4096
#define SEND_BUF_SIZE 4096
#define HEADERS_BUF_SIZE 4096
#define STORAGE_BUF_SIZE 4096
#define FUNCS_BUF_SIZE 4096
#define SUBS_COUNT_MAX 1024


void serve_buf(int fd, uint8_t *buf, char *type, int len){
sprintf(headers_buf,HTTP_HEADER_OKAY,type,len);
send(fd,headers_buf,strlen(headers_buf),0);
send(fd,buf,len,0);
}

void serve_menu(int fd){
MENUITEM *menu=menu_handler();
trace("menu: %p",menu);
uint8_t *out_buf=send_buf;
out_buf+=sprintf(out_buf,"[\n");
int p=0;
while(menu->label[0] && menu->type && menu->addr){
if(p++){
out_buf+=sprintf(out_buf,",\n");
}
out_buf+=sprintf(out_buf,"{label:\"%s\",type:\"%c\",addr:%llu,hint:[%.5f,%.5f,%.5f],streamable:%d,apply:%llu}",
menu->label,menu->type,(uint64_t)menu->addr,menu->min,menu->max,menu->val,menu->streamable,(uint64_t)menu->apply);
menu++;
}
out_buf+=sprintf(out_buf,"\n]\n");
serve_buf(fd,send_buf,"application/json; charset=UTF-8",out_buf-send_buf);
close(fd);
}


void serve_file(int fd, char *filename){
ph_create(ext);
char *t;
int filesize,chunk,q;
FILE *f=fopen(filename,"rb");
trace("opened %s = %p",filename,f);

if(!f){
sprintf(send_buf,"Can't open file \"%s\"!",filename);
serve_buf(fd,send_buf,"text/plain",strlen(send_buf));
close(fd);
return;
}
fseek(f,0,SEEK_END);
filesize=ftell(f);
fseek(f,0,SEEK_SET);

for(q=0;filename[q];q++){
if(filename[q]=='.'){
ph_reset(ext);
}
ph_add(ext,filename[q]);
}

switch(ph_value(ext)){
case PH__HTML:
case PH__HTM: t="text/html; charset=UTF-8";break;
case PH__CSS: t="text/css; charset=UTF-8";break;
case PH__JS: t="text/javascript; charset=UTF-8";break;
case PH__JSON: t="application/json; charset=UTF-8";break;
case PH__PNG: t="image/png";break;
case PH__JPG:
case PH__JPEG: t="image/jpeg";break;
case PH__GIF: t="image/gif";break;
case PH__ICO: t="image/vnd.microsoft.icon";break;
case PH__AVIF: t="image/avif";break;
case PH__BMP: t="image/bmp";break;
case PH__WAV: t="audio/wav";break;
case PH__MP3: t="audio/mpeg";break;
case PH__MP4: t="video/mp4";break;
case PH__WEBM: t="video/webm";break;
case PH__OGG: t="application/ogg";break;
case PH__EXE:
case PH__DLL:
case PH__BIN: t="application/octect-stream";break;
default:
t="text/plain; charset=UTF-8";
}

// send buffers
sprintf(headers_buf,HTTP_HEADER_OKAY,t,filesize);
write(fd,headers_buf,strlen(headers_buf));

while(filesize>0){
chunk=filesize;
if(chunk>SEND_BUF_SIZE){chunk=SEND_BUF_SIZE;}
chunk=fread(send_buf,1,chunk,f);
write(fd,send_buf,chunk);
filesize-=chunk;
}
close(fd);

}

void serve_redirect();
void serve_subs();


/*
main commands, you may join then in single call
+/memr - read memory (one or more values)
+/memw - write memory
+/memc - call __cdecl void function (or int?)
/mems - subscribe to this address and type
/subs - pull all subscribed addresses data
/usub - reset all subscriptions
+/exit - just quit application
/info - get system info and base addrs (like storage or some calls)
/find - find value of integer or string in memory (???)
+/ping - simple responce "pong"

special commands, not join them:
?/sock - switch to websocket protocol
+/file/index.html - serve static file from current dir (user may escape from directory app!)
?/ - redirect to "/file/index.html"

system info, responce to "/info" command:
//"stor" - storage with 4096 bytes for user data (client can write here for func calls)
//"func" - storage for your function pointers
"subs" - pointer for subscriptions array
"menu" - json doc for functions and pointers

type data:
Q - unsigned integer 64 bits
I - unsigned integer 32 bits
S - unsigned integer 16 bits
C - unsigned integer 8 bits
q - signed integer 64 bits
i - signed integer 32 bits
s - signed integer 16 bits
c - signed integer 8 bits
b - boolean, like checkbox
t - trigger, just call func, no data
T - Title, no addr, no callback
N - big endian 32 bit value
n - big endian 16 bit value
f - float
d - double
A - ASCII string (with null-terminator, may overflow buffer)
G - uint32_t value with RGBX (RGBA)

menu struct:
name
descr
type
addr
func on enter
func on preview (before user finished input)
float .min
float .max
float .val
read_addr


Examples:
GET /memwI12345678=555&memwf983745387=1.33333&memrA=876827364 HTTP/1.0
...
HTTP/1.0 200 Okay
Content-Type: text/plain
Content-Length: ...

"Hello, world"

*/


void serve_client(int fd){
int n;
ph_create(str);
static int EOL,method,type,addr,header_name;
static int content_offset,content_len;
static int query_offset,query_len;
int resp;
static uint8_t *header_value;
static char value[256];
static int value_pos;
uint8_t *out_buf=send_buf;
int q;
int req_pos;
trace("reading start");
int req_size=recv(fd,recv_buf,RECV_BUF_SIZE,0);
trace("reading end: %d", req_size);

req_pos=0;
content_len=-1;
content_offset=-1;

// what method?
method=HTTP_METHOD_UNKNOWN;
ph_reset(str);
while(req_pos<req_size){
ph_add(str,recv_buf[req_pos]);

if(recv_buf[req_pos++]=='/'){
if(ph_value(str)==PH_GET_){method=HTTP_METHOD_GET;}
if(ph_value(str)==PH_POST_){method=HTTP_METHOD_POST;}
break;
}
}

if(method==HTTP_METHOD_UNKNOWN){ // вы тут не хулюганте!
trace("unknown method");
closesocket(fd);
WSACleanup();
//close(fd);
return;
}

query_offset=req_pos;
while(req_pos<req_size){
trace("q=%c",recv_buf[req_pos]);
if(recv_buf[req_pos]==' '){query_len=req_pos-query_offset;
trace("we found len: %d",query_len);
break;}
req_pos++;
}
recv_buf[req_pos]=0;

if(method==HTTP_METHOD_POST){ // прочитать заголовки до конца, найти указатель на тело, найти размер тела
//trace("post from %d to %d",req_pos,req_size);
EOL=0;
ph_reset(str);
while(req_pos<req_size){
if(recv_buf[req_pos]==' '){
//trace("skipped space");
req_pos++;
continue;
}
if(recv_buf[req_pos]=='\r' || recv_buf[req_pos]=='\n'){
//trace("found EOL, str %d, name: %d ",str_val,header_name);
if(header_name==PH_CONTENT_LENGTH_){
//trace("we found content_len");
content_len=atoi(header_value);
}
if(header_name==PH_CONTENT_TYPE_ && ph_value(str)!=PH_APPLICATION_X_WWW_FORM_URLENCODED){

//trace("wrong encoding: %s, wait val %d",header_value, str_val);
close(fd);return;
} // мы хотим только WWW_FORM_URLENCODED!
header_name=0;
ph_reset(str);
EOL++;
if(EOL==4){
content_offset=req_pos+1;
break;
}
req_pos++;
continue;
}
ph_add(str,recv_buf[req_pos]);
//trace("%c (%d)",recv_buf[req_pos],str_val);
EOL=0;

if(recv_buf[req_pos]==':'){
//trace("found header %d val: %.10s",str_val,recv_buf+req_pos);
header_name=ph_value(str);
header_value=recv_buf+req_pos+2;
ph_reset(str);
}
req_pos++;
}

}

trace("we have %d content %d %d",method,content_offset,content_len);
trace("we have %d query %d %d",method,query_offset,query_len);

if(query_len>content_len){
content_offset=query_offset;
content_len=query_len;
}

if(content_len<=0 || content_offset<0 || content_offset+content_len>req_size){
trace("wrong input ");
close(fd);return;
}

// exec commands

req_pos=content_offset;
req_size=content_offset+content_len;

resp=0;
ph_reset(str);
while(req_pos<req_size){
ph_add(str,recv_buf[req_pos]);
//trace("%d: %c (%d)",req_pos,recv_buf[req_pos],str_val);

if(ph_value(str)==PH_PING){
out_buf+=sprintf(out_buf,"pong\n");
}

if(ph_value(str)==PH_EXIT){
exit(0);
}

if(ph_value(str)==PH_MEMW){
req_pos++;
type=recv_buf[req_pos];
req_pos++;
addr=0;
while(req_pos<req_size && recv_buf[req_pos]>='0' && recv_buf[req_pos]<='9'){
addr=addr*10+recv_buf[req_pos]-'0';
req_pos++;
}
if(recv_buf[req_pos++]!='='){
close(fd);return;
}
value_pos=0;
while(req_pos<req_size && recv_buf[req_pos]!=' ' && recv_buf[req_pos]!='&'){
value[value_pos++]=recv_buf[req_pos];
req_pos++;
}
value[value_pos]=0;
trace("we have memwrite command to 0x%x, type: %c, new value: \"%s\"\n",addr,type,value);

if(type=='c' || type=='b'){
*(int8_t*)addr=atoi(value);
}
if(type=='C'){
*(uint8_t*)addr=atoi(value);
}
if(type=='f'){
*(float*)addr=atof(value);
}
if(type=='I'){
*(unsigned int*)addr=atoi(value);
}
if(type=='i'){
*(signed int*)addr=atoi(value);
}
if(type=='A'){
strcpy((void*)addr,value);
}

}


if(ph_value(str)==PH_MEMR){
req_pos++;
type=recv_buf[req_pos];
req_pos++;
addr=0;
while(req_pos<req_size && recv_buf[req_pos]>='0' && recv_buf[req_pos]<='9'){
addr=addr*10+recv_buf[req_pos]-'0';
req_pos++;
}
while(recv_buf[req_pos]!='&' && req_pos<req_size){
req_pos++;
}

if(type=='f'){
out_buf+=sprintf(out_buf,"%c%u=%.7f\n",type,addr,*(float*)addr);
}
if(type=='b' || type=='c'){
out_buf+=sprintf(out_buf,"%c%u=%d\n",type,addr,(int)*(uint8_t*)addr);
}
if(type=='C'){
out_buf+=sprintf(out_buf,"%c%u=%u\n",type,addr,(int)*(uint8_t*)addr);
}
if(type=='I'){
out_buf+=sprintf(out_buf,"%c%u=%u\n",type,addr,*(unsigned int*)addr);
}
if(type=='i'){
out_buf+=sprintf(out_buf,"%c%u=%d\n",type,addr,*(signed int*)addr);
}
if(type=='A'){
out_buf+=sprintf(out_buf,"%c%u=%s\n",type,addr,(char*)addr);
}

}


if(ph_value(str)==PH_MEMC){
req_pos++;
req_pos++; // must be =
addr=0;
while(req_pos<req_size && recv_buf[req_pos]>='0' && recv_buf[req_pos]<='9'){
addr=addr*10+recv_buf[req_pos]-'0';
req_pos++;
}
((void(*)(void))(addr))();
}



if(ph_value(str)==PH_FILE){
trace("found file command");
req_pos++;
req_pos++; // must be /
header_value=recv_buf+req_pos;
while(req_pos<req_size && recv_buf[req_pos]!='&' && recv_buf[req_pos]>' '){
req_pos++;
}
recv_buf[req_pos]=0;
serve_file(fd,header_value);
return;
}



if(ph_value(str)==PH_MENU){
serve_menu(fd);
return;
}



if(recv_buf[req_pos]=='&'){
ph_reset(str);
}

req_pos++;

}


serve_buf(fd,send_buf,"text/plain",out_buf-send_buf);


/*
// special case
if(memcmp(recv_buf,"GET / ",6)==0){
write(fd,HTTP_HEADER_REDIRECT,strlen(HTTP_HEADER_REDIRECT));
return;
}

*/
}


void *listen_tcp(void *param){

#ifdef _WIN32
WSADATA wsaData;
WSAStartup(MAKEWORD(2,2), &wsaData);
#endif

int server, client, len, opt;
struct sockaddr_in servaddr, client_addr;
server=socket(AF_INET,SOCK_STREAM,0);
if(server<0){crash();}
opt=1;
#ifndef _WIN32
setsockopt(server, SOL_SOCKET, SO_REUSEPORT,(const char*) &opt, sizeof(opt));
#endif
setsockopt(server, SOL_SOCKET, SO_REUSEADDR,(const char*) &opt, sizeof(opt));

memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=WEBSERWER_INTERFACE;
servaddr.sin_port=htons(WEBSERWER_PORT);

if(bind(server, (struct sockaddr*)&servaddr, sizeof(servaddr))){crash();}
if(listen(server,5)){crash();}

trace("ready to accept connections");

while(1){
len=sizeof(client_addr);
client=accept(server, (struct sockaddr*)&client_addr, &len);
if(client<0){continue;}
trace("accepted!");
serve_client(client);
trace("client served");
}
}

void webserver_start(void*(*menu)(void)){

menu_handler=menu;
recv_buf=malloc(RECV_BUF_SIZE);
send_buf=malloc(SEND_BUF_SIZE);
headers_buf=malloc(HEADERS_BUF_SIZE);
storage=malloc(STORAGE_BUF_SIZE);
funcs=malloc(FUNCS_BUF_SIZE);
subs=malloc(SUBS_COUNT_MAX*sizeof(*subs));
memset(subs,0,SUBS_COUNT_MAX*sizeof(*subs));

pthread_create(&web_server, NULL, listen_tcp, NULL);
}


#ifdef _SIMPLE_DEMO_APP

// simple demo app

int demo_1=0;
float demo_2=0;
char demo_3[]="This is a demo string";

void demo_sub(){
demo_1=0;
demo_2=0;
strcpy(demo_3,"demo_sub() was called");
}

void *demo_menu(){
static MENUITEM menu[]={
{"demo_1",'i',&demo_1,.min=-10,.max=10,.val=0},
{"demo_2",'f',&demo_2,.min=-10,.max=10,.val=0,.streamable=1},
{"demo_3",'A',&demo_3},
{"demo_sub",'t',&demo_sub},
{"",0,0}
};
return(&menu);
}

int main(void){
int now,spent;
int demo_started; // variable on stack!
webserver_start(&demo_menu);
demo_started=time(0);

// use "echo 0 > /proc/sys/kernel/randomize_va_space" to disable randomization
fprintf(stderr,"offsets: demo_1:%p/%u (int)\n",&demo_1,(int)&demo_1);
fprintf(stderr,"offsets: demo_2:%p/%u (float)\n",&demo_2,(int)&demo_2);
fprintf(stderr,"offsets: demo_3:%p/%u (ascii string)\n",&demo_3,(int)&demo_3);
fprintf(stderr,"offsets: demo_started:%p/%u (int)\n",&demo_started,(int)&demo_started);
fprintf(stderr,"offsets: demo_sub():%p/%u (int)\n",&demo_sub,(int)&demo_sub);

while(1){
now=time(0);
spent=now-demo_started;
fprintf(stderr,"\rVars: %d, %2.5f \"%s\", time spent: %d seconds...",demo_1,demo_2,demo_3,spent);
demo_1++;
demo_2+=0.0123;
sleep(1);
}

return 0;
}


#endif
