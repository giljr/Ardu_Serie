var five = require('johnny-five');

five.Board( { port: "COM10" } ).on("ready", function(){
  console.log('Board ready');
  five.Led(13).strobe(200);
})
