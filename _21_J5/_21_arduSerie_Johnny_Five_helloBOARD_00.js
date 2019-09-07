var five = require("johnny-five");
var myBoard;

//myBoard = new five.Board();
myBoard = new five.Board( { port: "COM10" } );



myBoard.on("ready", function() {
  console.log('Arduino is ready!');
});
