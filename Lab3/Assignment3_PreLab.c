/* Psuedo Code
start server
start client
server:
while(no message){
  wait
}

client: calc checksum
client: add to header and send message
while(no acknowledgement){
  wait
}
server:
  recieve message
  check checksome
  send acknowledgement to client

client:
recieve acknowledgement
if ack(good){
  change sequence num
  send next message
}
else{
  resend original message
}

*/
