//
// Created by zavier on 2021/12/14.
//
#include "acid/common/config.h"
#include "tcp_server.h"

namespace acid {
static auto g_logger = GetLogInstance();

static ConfigVar<uint64_t>::ptr g_tcp_server_recv_timeout =
        Config::Lookup<uint64_t>("tcp_server.recv_timeout",
                                 (uint64_t)(60 * 1000 * 2), "tcp server recv timeout");

TcpServer::TcpServer()
    : m_timer()
    , m_recvTimeout(g_tcp_server_recv_timeout->getValue())
    , m_name("acid/1.0.0")
    , m_stop(true) {
}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::bind(Address::ptr addr) {
    std::vector<Address::ptr> addrs, fail;
    addrs.push_back(addr);
    return bind(addrs, fail);
}

bool TcpServer::bind(const std::vector<Address::ptr> &addrs, std::vector<Address::ptr> &fail) {
    for(Address::ptr addr : addrs){
        Socket::ptr sock = Socket::CreateTCP(addr);
        if(!sock->bind(addr)) {
            SPDLOG_LOGGER_ERROR(g_logger, "bind fail errno={} errstr={} addr={}", errno, strerror(errno), addr->toString());
            fail.push_back(addr);
            continue;
        }
        if(!sock->listen()) {
            SPDLOG_LOGGER_ERROR(g_logger, "listen fail errno={} errstr={} addr={}", errno, strerror(errno), addr->toString());
            fail.push_back(addr);
            continue;
        }
        m_listens.push_back(sock);
    }
    if(!fail.empty()) {
        m_listens.clear();
        return false;
    }
    for(auto& sock : m_listens) {
        SPDLOG_LOGGER_INFO(g_logger, "server {} bind {} success", m_name, sock->toString());
    }

    return true;
}

void TcpServer::start() {
    if(!isStop()) {
        return;
    }
    m_stop = false;
    m_stopCh = co::co_chan<bool>();
    for(auto& sock : m_listens) {
        go [sock, this] {
            this->startAccept(sock);
        };
    }
    // 阻塞等待
    m_stopCh >> nullptr;
}

void TcpServer::stop() {
    if(isStop()) {
        return;
    }
    m_stop = true;
    for(auto& sock : m_listens) {
        sock->close();
    }
    m_stopCh.Close();
}

void TcpServer::startAccept(Socket::ptr sock) {
    while(!isStop()) {
        Socket::ptr client = sock->accept();
        if(client) {
            client->setRecvTimeout(m_recvTimeout);
            go [client, this] {
                this->handleClient(client);
            };
        } else {
            if (!sock->isConnected()) {
                return;
            }
            SPDLOG_LOGGER_ERROR(g_logger, "accept fail, errno={} errstr={}", errno, strerror(errno));
        }
    }
}

void TcpServer::handleClient(Socket::ptr client) {
    SPDLOG_LOGGER_INFO(g_logger,"handleClient: {}", client->toString());
}

}