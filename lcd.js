var i2c = require('i2c');
var address = 0x28;
var wire = new i2c(address, { device: '/dev/i2c-1' });

var command = 0xFE;
var off = 0x42;
var on  = 0x41;
var clear = 0x51;
var home = 0x46;
var cursor = 0x4B;

function throwError (error) {
  if (error) throw error;
}
// Commands are legit, text needs an ASCII code: https://duckduckgo.com/?q=ascii+table
//wire.writeBytes(command, [on], throwError);
//wire.writeBytes(command, [clear], throwError);
//wire.writeBytes(command, [cursor_off], throwError);
//wire.writeBytes(command, [home], throwError);
//wire.writeBytes(command, [off], throwError);
//wire.writeBytes(command, [on], throwError);
//wire.writeByte(69, throwError);
wire.writeBytes(65, [66,67], throwError);
