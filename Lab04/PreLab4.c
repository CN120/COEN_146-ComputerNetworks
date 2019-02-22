
start server
start client

 //Server:
state 0;
listen for filename
recieve filename;

if(checksum is != to calc_checksum or seq_ack != state){
    flip seq_ack;
    send back neg ack packet;
}
else{
    open file;
}
while(1){
    receive message;
    if (message length = 0){
        close file;
        return 0;
    }

    if(checksum is != to calc_checksum or seq_ack != state){
        flip seq_ack;
        send back neg ack packet;
    }
    else{
        send pos ack back;
        flip state;
        write data to file;
    }

}

//client
state 0;
send filename;
while(x time has not passed){
    listen for ack;
    receive ack;
    if(neg ack){
        goto send filename;
    }
}

open input file;
while(1){
    read segment of file to 'data';
    if(EOF){
        close input file;
        return 0;
    }
    send: send packet with data;
    while(x time has not passed){
        listen for ack;
        receive ack;
        if(neg ack){
            goto send;
        }
    }
    goto send;
}
