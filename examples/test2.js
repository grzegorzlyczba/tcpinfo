var http = require('http');
var tcpinfo = require('tcpinfo');

http.createServer(function (req, res) {
    var ret = tcpinfo.get(req);
    console.log(ret);
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('hello');
}).listen(8080);
