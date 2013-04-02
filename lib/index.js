var net = require('net');
var http = require('http');
var tcpinfo = require('../build/default/tcpinfo.node');
var get = function (s) {
    if (typeof s !== 'number')  {
        if (s instanceof http.IncomingMessage) {
            s = s.socket.fd;
        } else if (s instanceof net.Socket) {
            s = s.fd;
        } else {
            throw 'Invalid params';
        }
    }
    return tcpinfo.get(s);
};
exports.get = get;
