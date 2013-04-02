var net = require('net');
var tcpinfo = require('tcpinfo');
var sock = new net.Socket({
    type: 'tcp4'
});

sock.connect(80, '127.0.0.1');
var ret = tcpinfo.get(sock);
console.log('info1', ret);
var ret = tcpinfo.get(sock.fd);
console.log('info2', ret);
setTimeout(function () {
    sock.destroy();
}, 1000);
