//
// Created by zavier on 2021/12/18.
//

#include "http_connection.h"
#include "parse.h"
#include "acid/common/util.h"
namespace acid::http {
static auto g_logger = GetLogInstance();

std::string HttpResult::toString() const {
    std::string str = "[HttpResult result=" + std::to_string(result)
            + " msg=" + msg + " response=" + (response ? response->toString() : "nullptr") + "]";
    return str;
}

HttpConnection::HttpConnection(Socket::ptr socket, bool owner)
        : SocketStream(socket, owner) {
}

HttpResponse::ptr HttpConnection::recvResponse() {
    HttpResponseParser::ptr parser(new HttpResponseParser);
    uint64_t buff_size = HttpResponseParser::GetHttpResponseBufferSize();
    std::string buffer;
    buffer.resize(buff_size);
    char* data = &buffer[0];
    size_t offset = 0;
    while (!parser->isFinished()) {
        ssize_t len = read(data + offset, buff_size - offset);
        //ACID_LOG_INFO(g_logger) << data;
        if (len <= 0) {
            close();
            return nullptr;
        }
        len += offset;
        size_t nparse = parser->execute(data, len);
        if (parser->hasError() || nparse == 0) {
            SPDLOG_LOGGER_DEBUG(g_logger, parser->hasError());
            close();
            return nullptr;
        }
        offset = len - nparse;
    }
    if (parser->isChunked()) {
        do {
            ssize_t len = read(data + offset, buff_size - offset);
            //ACID_LOG_INFO(g_logger) << data;
            if (len <= 0) {
                SPDLOG_LOGGER_DEBUG(g_logger, "parse chunk error, code={}", parser->hasError());
                close();
                return nullptr;
            }
            len += offset;
            size_t nparse = parser->execute(data, len, true);
            if (parser->hasError() || nparse == 0) {
                SPDLOG_LOGGER_DEBUG(g_logger, "parse chunk error, code={}", parser->hasError());
                close();
                return nullptr;
            }
            offset = len - nparse;
        } while (!parser->isFinished());
    } else {
        uint64_t length = parser->getContentLength();
        if (length >= 0) {
            std::string body;
            body.resize(length);
            size_t len = 0;
            if (length >= offset) {
                memcpy(&body[0], data, offset);
                len = offset;
            } else {
                memcpy(&body[0], data, length);
                len = length;
            }
            length -= len;
            if(length > 0) {
                if(readFixSize(&body[len], length) <= 0) {
                    close();
                    return nullptr;
                }
            }
            parser->getData()->setBody(body);
        }
    }
    return parser->getData();
}

ssize_t HttpConnection::sendRequest(HttpRequest::ptr request) {
    std::string str = request->toString();
    return writeFixSize(str.c_str(), str.size());
}

HttpResult::ptr HttpConnection::DoGet(const std::string &url,
                                      uint64_t timeout_ms,
                                      const std::map<std::string,std::string> &header,
                                      const std::string &body) {
    Uri::ptr uri = Uri::Create(url);
    if (!uri) {
        return std::make_shared<HttpResult>(HttpResult::INVALID_URL, nullptr, "invalid url: " + url);
    }
    return DoGet(uri, timeout_ms, header, body);
}

HttpResult::ptr HttpConnection::DoGet(Uri::ptr uri,
                                      uint64_t timeout_ms,
                                      const std::map<std::string, std::string> &header,
                                      const std::string &body) {
    return DoRequest(HttpMethod::GET, uri, timeout_ms, header, body);
}

HttpResult::ptr HttpConnection::DoPost(const std::string &url,
                                       uint64_t timeout_ms,
                                       const std::map<std::string, std::string> &header,
                                       const std::string &body) {
    Uri::ptr uri = Uri::Create(url);
    if (!uri) {
        return std::make_shared<HttpResult>(HttpResult::INVALID_URL, nullptr, "invalid url: " + url);
    }
    return DoGet(uri, timeout_ms, header, body);
}

HttpResult::ptr HttpConnection::DoPost(Uri::ptr uri, uint64_t timeout_ms, const std::map<std::string, std::string> &header,
                       const std::string &body) {
    return DoRequest(HttpMethod::POST, uri, timeout_ms, header, body);
}

HttpResult::ptr HttpConnection::DoRequest(HttpMethod method,
                                          const std::string &url, uint64_t timeout_ms,
                                          const std::map<std::string, std::string> &header,
                                          const std::string &body) {
    Uri::ptr uri = Uri::Create(url);
    if (!uri) {
        return std::make_shared<HttpResult>(HttpResult::INVALID_URL, nullptr, "invalid url: " + url);
    }
    return DoGet(uri, timeout_ms, header, body);
}

HttpResult::ptr HttpConnection::DoRequest(HttpMethod method,
                                          Uri::ptr uri, uint64_t timeout_ms,
                                          const std::map<std::string, std::string> &header,
                                          const std::string &body) {
    HttpRequest::ptr request = std::make_shared<HttpRequest>();
    request->setMethod(method);
    request->setPath(uri->getPath());
    request->setQuery(uri->getQuery());
    request->setFragment(uri->getFragment());
    bool hasHost = false;
    for (auto& i: header) {
        if(strcasecmp(i.first.c_str(), "connection") == 0) {
            if(strcasecmp(i.second.c_str(), "keep-alive") == 0) {
                request->setClose(false);
            }
            continue;
        }
        if (!hasHost && strcasecmp(i.first.c_str(), "host") == 0) {
            hasHost = !i.second.empty();
        }
    }
    if (!hasHost) {
        request->setHeader("Host", uri->getHost());
    }
    request->setBody(body);
    return DoRequest(request,uri, timeout_ms);
}

HttpResult::ptr HttpConnection::DoRequest(HttpRequest::ptr request, Uri::ptr uri, uint64_t timeout_ms) {
    if (uri->getScheme().empty()) {
        uri->setScheme("http");
    }

    Address::ptr address = uri->createAddress();
    if (!address) {
        return std::make_shared<HttpResult>(HttpResult::INVALID_HOST, nullptr, "invalid host: " + uri->getHost());
    }
    Socket::ptr sock = Socket::CreateTCP(address);
    if(!sock) {
        return std::make_shared<HttpResult>(HttpResult::CREATE_SOCKET_ERROR
                , nullptr, "create socket fail: " + address->toString()
                           + " errno=" + std::to_string(errno)
                           + " errstr=" + std::string(strerror(errno)));
    }
    if(!sock->connect(address)) {
        return std::make_shared<HttpResult>(HttpResult::CONNECT_FAIL
                , nullptr, "connect fail: " + address->toString());
    }

    HttpConnection::ptr connection = std::make_shared<HttpConnection>(sock);

    sock->setRecvTimeout(timeout_ms);

    int rt = connection->sendRequest(request);

    if(rt == 0) {
        return std::make_shared<HttpResult>(HttpResult::SEND_CLOSE_BY_PEER
                , nullptr, "send request closed by peer: " + address->toString());
    }

    if(rt < 0) {
        return std::make_shared<HttpResult>(HttpResult::SEND_SOCKET_ERROR
                , nullptr, "send request socket error errno=" + std::to_string(errno)
                           + " errstr=" + std::string(strerror(errno)));
    }

    HttpResponse::ptr response = connection->recvResponse();
    if(!response) {
        return std::make_shared<HttpResult>(HttpResult::TIMEOUT
                , nullptr, "recv response timeout: " + address->toString()
                           + " timeout_ms:" + std::to_string(timeout_ms));
    }
    return std::make_shared<HttpResult>(HttpResult::OK, response, "ok");
}

HttpConnection::~HttpConnection() {
    SPDLOG_LOGGER_DEBUG(g_logger, "~HttpConnection");
}

HttpConnectionPool::ptr
HttpConnectionPool::Create(const std::string &uri, const std::string &vhost, uint32_t max_size,
                           uint32_t max_alive_time, uint32_t max_request) {
    Uri::ptr turi = Uri::Create(uri);
    if(turi) {
        SPDLOG_LOGGER_ERROR(g_logger, "invalid uri={}", uri);
    }
    return std::make_shared<HttpConnectionPool>(turi->getHost()
            , vhost, turi->getPort(), turi->getScheme() == "https"
            , max_size, max_alive_time, max_request);
}

HttpConnectionPool::HttpConnectionPool(const std::string &host, const std::string &vhost, uint32_t port,
                                       bool is_https, uint32_t max_size, uint32_t max_alive_time,
                                       uint32_t max_request)
        :m_host(host)
        ,m_vhost(vhost)
        ,m_port(port ? port : (is_https ? 443 : 80))
        ,m_maxSize(max_size)
        ,m_maxAliveTime(max_alive_time)
        ,m_maxRequest(max_request)
        ,m_isHttps(is_https){

}

HttpResult::ptr HttpConnectionPool::doGet(const std::string &url, uint64_t timeout_ms,
                                          const std::map<std::string, std::string> &headers,
                                          const std::string &body) {
    return doRequest(HttpMethod::GET, url, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doGet(Uri::ptr uri, uint64_t timeout_ms, const std::map<std::string, std::string> &headers,
                          const std::string &body) {
    std::stringstream ss;
    ss << uri->getPath()
       << (uri->getQuery().empty() ? "" : "?")
       << uri->getQuery()
       << (uri->getFragment().empty() ? "" : "#")
       << uri->getFragment();
    return doGet(ss.str(), timeout_ms, headers, body);
}

HttpResult::ptr HttpConnectionPool::doPost(const std::string &url, uint64_t timeout_ms,
                                           const std::map<std::string, std::string> &headers,
                                           const std::string &body) {
    return doRequest(HttpMethod::POST, url, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doPost(Uri::ptr uri, uint64_t timeout_ms, const std::map<std::string, std::string> &headers,
                           const std::string &body) {
    std::stringstream ss;
    ss << uri->getPath()
       << (uri->getQuery().empty() ? "" : "?")
       << uri->getQuery()
       << (uri->getFragment().empty() ? "" : "#")
       << uri->getFragment();
    return doPost(ss.str(), timeout_ms, headers, body);
}

HttpResult::ptr HttpConnectionPool::doRequest(HttpMethod method, const std::string &url, uint64_t timeout_ms,
                                              const std::map<std::string, std::string> &headers,
                                              const std::string &body) {
    HttpRequest::ptr req = std::make_shared<HttpRequest>();
    req->setPath(url);
    req->setMethod(method);
    bool has_host = false;
    //req->setClose(false);
    for(auto& i : headers) {
        if(strcasecmp(i.first.c_str(), "connection") == 0) {
            if(strcasecmp(i.second.c_str(), "keep-alive") == 0) {
                req->setClose(false);
            }
            continue;
        }

        if(!has_host && strcasecmp(i.first.c_str(), "host") == 0) {
            has_host = !i.second.empty();
        }

        req->setHeader(i.first, i.second);
    }
    if(!has_host) {
        if(m_vhost.empty()) {
            req->setHeader("Host", m_host);
        } else {
            req->setHeader("Host", m_vhost);
        }
    }
    req->setBody(body);
    return doRequest(req, timeout_ms);
}

HttpResult::ptr HttpConnectionPool::doRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout_ms,
                                              const std::map<std::string, std::string> &headers,
                                              const std::string &body) {
    std::stringstream ss;
    ss << uri->getPath()
       << (uri->getQuery().empty() ? "" : "?")
       << uri->getQuery()
       << (uri->getFragment().empty() ? "" : "#")
       << uri->getFragment();
    return doRequest(method, ss.str(), timeout_ms, headers, body);
}

HttpResult::ptr HttpConnectionPool::doRequest(HttpRequest::ptr request, uint64_t timeout_ms) {
    HttpConnection::ptr conn = getConnection();
    if (!conn) {
        return std::make_shared<HttpResult>(HttpResult::POOL_INVALID_CONNECTION
                , nullptr, "pool host:" + m_host + " port:" + std::to_string(m_port));
    }
    Socket::ptr sock = conn->getSocket();
    if(!sock) {
        return std::make_shared<HttpResult>(HttpResult::POOL_INVALID_CONNECTION
                , nullptr, "pool host:" + m_host + " port:" + std::to_string(m_port));
    }
    sock->setRecvTimeout(timeout_ms);
    ssize_t rt = conn->sendRequest(request);
    if(rt == 0) {
        return std::make_shared<HttpResult>(HttpResult::SEND_CLOSE_BY_PEER
                , nullptr, "send request closed by peer: " + sock->getRemoteAddress()->toString());
    }
    if(rt < 0) {
        return std::make_shared<HttpResult>(HttpResult::SEND_SOCKET_ERROR
                , nullptr, "send request socket error errno=" + std::to_string(errno)
                           + " errstr=" + std::string(strerror(errno)));
    }
    HttpResponse::ptr rsp = conn->recvResponse();

    if(!rsp) {
        return std::make_shared<HttpResult>(HttpResult::TIMEOUT
                , nullptr, "recv response timeout: " + sock->getRemoteAddress()->toString()
                           + " timeout_ms:" + std::to_string(timeout_ms));
    }
    return std::make_shared<HttpResult>(HttpResult::OK, rsp, "ok");

}

HttpConnection::ptr HttpConnectionPool::getConnection() {
    uint64_t now_ms = acid::GetCurrentMS();
    std::vector<HttpConnection*> invalid_conns;
    HttpConnection* ptr = nullptr;
    std::unique_lock<co::co_mutex> lock(m_mutex);
    while(!m_conns.empty()) {
        auto conn = *m_conns.begin();
        m_conns.pop_front();
        if(!conn->isConnected()) {
            invalid_conns.push_back(conn);
            continue;
        }
        if((conn->m_createTime + m_maxAliveTime) > now_ms) {
            invalid_conns.push_back(conn);
            continue;
        }
        ptr = conn;
        break;
    }
    lock.unlock();
    for(auto i : invalid_conns) {
        delete i;
    }
    m_total -= invalid_conns.size();

    if(!ptr) {
        IPAddress::ptr addr = Address::LookupAnyIPAddress(m_host);
        if(!addr) {
            SPDLOG_LOGGER_ERROR(g_logger, "get addr fail: {}", m_host);
            return nullptr;
        }
        addr->setPort(m_port);
        //Socket::ptr sock = m_isHttps ? SSLSocket::CreateTCP(addr) : Socket::CreateTCP(addr);
        Socket::ptr sock = Socket::CreateTCP(addr);
        if(!sock) {
            SPDLOG_LOGGER_ERROR(g_logger, "create sock fail: {}", addr->toString());
            return nullptr;
        }
        if(!sock->connect(addr)) {
            SPDLOG_LOGGER_ERROR(g_logger, "sock connect fail: {}", addr->toString());
            return nullptr;
        }

        ptr = new HttpConnection(sock);
        ++m_total;
    }
    return HttpConnection::ptr(ptr, [this](HttpConnection* conn){
        RelesePtr(conn, this);
    });
}

void HttpConnectionPool::RelesePtr(HttpConnection *ptr, HttpConnectionPool *pool) {
    ++ptr->m_request;
    if (!ptr->isConnected()
            || ((ptr->m_createTime + pool->m_maxAliveTime) < acid::GetCurrentMS())
            || (ptr->m_request >= pool->m_maxRequest)) {
        delete ptr;
        --pool->m_total;
        return;
    }
    std::unique_lock<co::co_mutex> lock(pool->m_mutex);
    pool->m_conns.push_back(ptr);
}

}