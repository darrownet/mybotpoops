var express = require('express')
  , app = express()
  , server = require('http').createServer(app)
  , io = require('socket.io').listen(server)
  , path = require('path');
  
app.use(express.static(__dirname));

server.listen(80);

//SOCKET.IO
io.sockets.on('connection', function (socket) {
  //
  socket.on('led-on', function (data) {
    socket.broadcast.emit('led-on-relay', data);
  });
  //
  socket.on('led-off', function (data) {
    socket.broadcast.emit('led-off-relay', data);
  });
});