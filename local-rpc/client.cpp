#include <iostream>
#include <memory>
#include <thread>
#include <functional>
#include <cstring>
 
class RPCClient
{
public:
    using RPCCallback = std::function<void(const std::string&)>;
    RPCClient(const std::string& server_address) : server_address_(server_address) {}
    ~RPCClient() {}
 
    // method表示远程调用的方法名，request表示远程调用的请求参数，callback是回调函数，用于处理远程调用结果的响应。
    void Call(const std::string& method, const std::string& request, RPCCallback callback)
    {
        // 序列化请求数据
        std::string data = Serialize(method, request);
        // 发送请求
        SendRequest(data);
        // 开启线程接收响应
        std::thread t([this, callback]() {
            std::string response = RecvResponse();
            // 反序列化响应数据
            std::string result = Deserialize(response);
            callback(result);
        });
        t.detach();
    }
 
private:
    std::string Serialize(const std::string& method, const std::string& request)
    {
        // 省略序列化实现
    }
 
    void SendRequest(const std::string& data)
    {
        // 省略网络发送实现
    }
 
    std::string RecvResponse()
    {
        // 省略网络接收实现
    }
 
    std::string Deserialize(const std::string& response)
    {
        // 省略反序列化实现
    }
 
private:
    std::string server_address_;
};
 
int main()
{
    std::shared_ptr<RPCClient> client(new RPCClient("127.0.0.1:8000"));
    client->Call("Add", "1,2", [](const std::string& result) {
        std::cout << "Result: " << result << std::endl;
    });
    return 0;
}
