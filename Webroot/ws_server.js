//# apt-get install nodejs node-websocket npm

import { WebSocketServer } from 'ws';

const wss=new WebSocketServer({port:3000});

wss.on("connection",function(ws){
console.log("we got new connection: "+ws);
ws.on('message',function message(data){console.log('received: %s',data);ws.send("received: "+data);});
ws.send("Hello");
})
