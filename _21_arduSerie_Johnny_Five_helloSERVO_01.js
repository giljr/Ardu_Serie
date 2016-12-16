var five = require("johnny-five");
var board = new five.Board({ port: "COM3" });

board.on("ready", function() {

 var servo_tilt = new five.Servo(10);
servo_tilt .sweep({
  range: [45, 135]
 });

// servo_tilt .sweep({
//  range: [0, 30],
//  interval: 100,
//  });

// servo_tilt .sweep({
//  range: [0, 60],
//  interval: 100,
//  step: 10
// });

// var servo_pan = new five.Servo(9);
// servo_pan.sweep({
//   range: [0, 180],
//   interval: 10000,
//  });


});
