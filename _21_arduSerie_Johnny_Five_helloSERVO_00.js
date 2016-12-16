var five = require("johnny-five");
var board = new five.Board({ port: "COM3" });

board.on("ready", function() {

  var servo = new five.Servo(9);

  // Sweep from 0-180 and repeat.
  servo.sweep();
});
