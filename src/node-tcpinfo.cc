/*
 * Copyright (C) 2013 by Grzegorz Lyczba <grzegorz.lyczba@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <string.h>
#include <errno.h>
#include <stdlib.h>


#include <v8.h>
#include <node.h>
#include <v8-debug.h>

using namespace v8;
using namespace node;


static inline Handle<Object> formatTcpInfo(struct tcp_info *info) {
    HandleScope handle_scope;
    Handle<Object> obj = Object::New();
    obj->Set(v8::String::New("state"), v8::Int32::New(info->tcpi_state));
    obj->Set(v8::String::New("ca_state"), v8::Int32::New(info->tcpi_ca_state));
    obj->Set(v8::String::New("retransmits"), v8::Int32::New(info->tcpi_retransmits));
    obj->Set(v8::String::New("probes"), v8::Int32::New(info->tcpi_probes));
    obj->Set(v8::String::New("options"), v8::Int32::New(info->tcpi_options));
    obj->Set(v8::String::New("snd_wscale"), v8::Int32::New(info->tcpi_snd_wscale));
    obj->Set(v8::String::New("rcv_wscale"), v8::Int32::New(info->tcpi_rcv_wscale));
    obj->Set(v8::String::New("rto"), v8::Int32::New(info->tcpi_rto));
    obj->Set(v8::String::New("ato"), v8::Int32::New(info->tcpi_ato));
    obj->Set(v8::String::New("snd_mss"), v8::Int32::New(info->tcpi_snd_mss));
    obj->Set(v8::String::New("rcv_mss"), v8::Int32::New(info->tcpi_rcv_mss));
    obj->Set(v8::String::New("unacked"), v8::Int32::New(info->tcpi_unacked));
    obj->Set(v8::String::New("sacked"), v8::Int32::New(info->tcpi_sacked));
    obj->Set(v8::String::New("lost"), v8::Int32::New(info->tcpi_lost));
    obj->Set(v8::String::New("retrans"), v8::Int32::New(info->tcpi_retrans));
    obj->Set(v8::String::New("fackets"), v8::Int32::New(info->tcpi_fackets));
    obj->Set(v8::String::New("last_data_sent"), v8::Int32::New(info->tcpi_last_data_sent));
    obj->Set(v8::String::New("last_ack_sent"), v8::Int32::New(info->tcpi_last_ack_sent));
    obj->Set(v8::String::New("last_data_recv"), v8::Int32::New(info->tcpi_last_data_recv));
    obj->Set(v8::String::New("last_ack_recv"), v8::Int32::New(info->tcpi_last_ack_recv));
    obj->Set(v8::String::New("pmtu"), v8::Int32::New(info->tcpi_pmtu));
    obj->Set(v8::String::New("rcv_ssthresh"), v8::Int32::New(info->tcpi_rcv_ssthresh));
    obj->Set(v8::String::New("rtt"), v8::Int32::New(info->tcpi_rtt));
    obj->Set(v8::String::New("rttvar"), v8::Int32::New(info->tcpi_rttvar));
    obj->Set(v8::String::New("snd_ssthresh"), v8::Int32::New(info->tcpi_snd_ssthresh));
    obj->Set(v8::String::New("snd_cwnd"), v8::Int32::New(info->tcpi_snd_cwnd));
    obj->Set(v8::String::New("advmss"), v8::Int32::New(info->tcpi_advmss));
    obj->Set(v8::String::New("reordering"), v8::Int32::New(info->tcpi_reordering));
    obj->Set(v8::String::New("rcv_rtt"), v8::Int32::New(info->tcpi_rcv_rtt));
    obj->Set(v8::String::New("rcv_space"), v8::Int32::New(info->tcpi_rcv_space));
    obj->Set(v8::String::New("total_retrans"), v8::Int32::New(info->tcpi_total_retrans));
    return handle_scope.Close(obj);
}



static v8::Handle<v8::Value> getMethod(const v8::Arguments& args) {
    v8::HandleScope scope;
    int sock, ret;
    void *stats;
    socklen_t size;

    if (args.Length() != 1) {
        return ThrowException(Exception::Error (String::New("Invalid params")));
    }

    Local<Array> result = Array::New(2);

    size = sizeof(struct tcp_info);
    sock = (int) v8::Local<v8::Integer>::Cast(args[0])->Int32Value();
    stats = malloc(size);
    memset(stats, 0, size);
    ret = getsockopt(sock, IPPROTO_TCP, TCP_INFO, stats, &size);

    if (ret == -1) {
        result->Set(1, String::New(strerror(errno)));
    } else {
        result->Set(1, formatTcpInfo((struct tcp_info*)stats));
    }
    result->Set(0, Int32::New(ret));
    free(stats);
    return scope.Close(result);
}

extern "C" void init (v8::Handle<v8::Object> target) {
    v8::HandleScope scope;
    NODE_SET_METHOD(target, "get", getMethod);
}

