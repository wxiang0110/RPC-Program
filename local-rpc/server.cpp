#include <iostream>
#include <map>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
// 使用第三方库实现序列化和反序列化
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
using namespace std;
// 定义RPC函数类型
using RPCCallback = std::function<std::string(const std::string&)>;
class RPCHandler {
public:
    void registerCallback(const std::string& name, RPCCallback callback) {
        std::unique_lock<std::mutex> lock(mtx_);
        callbacks_[name] = callback;
    }
    std::string handleRequest(const std::string& request) {
        // 反序列化请求
        std::map<std::string, std::string> requestMap;
        std::istringstream is(request);
        boost::archive::text_iarchive ia(is);
        ia >> requestMap;
        // 查找并调用对应的回调函数
        std::string name = requestMap["name"];
        std::string args = requestMap["args"];
        std::unique_lock<std::mutex> lock(mtx_);
        auto it = callbacks_.find(name);
        if (it == callbacks_.end()) {
            return "Error: Unknown function";
        }
        RPCCallback callback = it->second;
        return callback(args);
    }
private:
    std::map<std::string, RPCCallback> callbacks_;
    std::mutex mtx_;
};
int main() {
    RPCHandler rpcHandler;
    // 注册回调函数
    rpcHandler.registerCallback("add", [](const std::string& args) {
        std::istringstream is(args);
        int a, b;
        is >> a >> b;
        int result = a + b;
        std::ostringstream os;
        os << result;
        return os.str();
    });
    rpcHandler.registerCallback("sub", [](const std::string& args) {
        std::istringstream is(args);
        int a, b;
        is >> a >> b;
        int result = a - b;
        std::ostringstream os;
        os << result;
        return os.str
    });
    // 创建处理请求的线程
    std::thread requestThread([&]() {
        while (true) {
            std::string request;
            std::cin >> request;
            std::string response = rpcHandler.handleRequest(request);
            std::cout << response << std::endl;
        }
    });
    requestThread.join();
    return 0;
}
