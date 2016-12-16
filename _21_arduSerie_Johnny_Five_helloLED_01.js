// JavaScript Document
var five = require('johnny-five'),
           board = new five.Board({ port: "COM10" }),
           led,
           toggleState = false;

board.on("ready", function(){
   console.log('Board ready!!!');
   led=new five.Led(13);

   setInterval(toggleLED,400);

    function toggleLED(){
    toggleState =! toggleState;
      if (toggleState) led.on();
      else led.off();
   }

});
console.log("\n Waiting for device to connect...");
