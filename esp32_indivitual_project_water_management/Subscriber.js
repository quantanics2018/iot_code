const mqtt = require('mqtt');
const fs = require('fs');

const borker_url = "mqtt://broker.emqx.io:1883"


const options = {
    username:"",
    password:""
}

const client  = mqtt.connect(borker_url,options);

client.on('connect',function(){
    console.log("connected to mqtt");
    client.subscribe('quantanics/industry/testing1');
});


client.on('message',function(topic,message){
    
    const data_arr = message.toString().split(',');
    // console.log(data_arr[0]);
    console.log("ouput data:\t"+data_arr);
    
    fs.appendFile('mqtt_subscriber_small_board.json',message,function(err){
        if (err) throw  errs;
        console.log("Record Saved");
    });
});

client.on('error',function(error){
    console.error('Error:',error);
});