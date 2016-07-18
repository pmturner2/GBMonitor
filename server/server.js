var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var path = require('path');

server.listen(3000);

app.use('/', express.static(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'public/scripts')));

// app.get('/', function(request, response) {
//   res.sendfile(__dirname + '/index.html');
// });

// app.get('/monitor.html', function(request, response) {
//   res.sendfile("public/monitor.html");
// });



// var app = require('http').createServer(handler);
// var url = require('url');
// var fs = require('fs');
// var io = require('socket.io')(app);

// var path = require('path');
// var express = require('express');
// var expressApp = express();
// expressApp.set('port', (process.env.PORT || 3000));
// expressApp.use('/', express.static(path.join(__dirname, 'public')));
// expressApp.use(express.static(path.join(__dirname, 'public/scripts')));

// app.listen(3000);

// function handler(request, response) {
//   var path = url.parse(request.url).pathname;
//   switch(path) {
//     case '/':
//       // App connects through this, otherwise should connect through 
//       response.writeHead(200, {'Content-Type': 'text/html'});
//       response.write('helloWorld');
//       response.end();
//       break;
//     case '/monitor.html':
//     case '/public/monitor.html':    
//       // Assume whatever is after the key is unique id of connection.
//       fs.readFile("public/monitor.html", function(error, data) {
//         if (error) {
//           response.writeHead(404);
//           response.write("404 does not exist");
//           response.end();
//         } else {
//           response.writeHead(200, {'Content-Type': 'text/html', 'unique_id': path});
//           response.write(data, 'utf8');
//           response.end();            
//         }
//       });
//       break;
//     default:
//       response.writeHead(404);
//       response.write("404 does not exist!");
//       response.end();
//       break; 
//   }
// }

var uniqueId = 0;

io.on('connection', function(socket) {
  socket.on('registerApp', function(data) {
    console.log("Received registerApp event...");

    socket.emit('registered', {uniqueId: uniqueId++});    
  }.bind(socket));

  socket.on('join', function(data) {
    socket.join(data.room);
    console.log("join message for room " + data.room);
  });

  socket.on('log', function(logData) {
    // console.log("log event logData: " + logData.toString());
    var logObject = JSON.parse(logData);
    io.to(logObject.room).emit('log', logObject);
    console.log("log event for room '" + logObject.room + "': message" + logObject.message.toString());
  });

  socket.emit('connected', { port: 3000 });
});

console.log("Starting server...");
