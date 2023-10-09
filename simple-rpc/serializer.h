#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <assert.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <initializer_list>

// 存储数据流的容器
class Buffer : public std::vector<char> {
    /*
    关于Buffer继承std::vector<char>中，为什么不需要手动开辟一块内存区域来进行数据保存？
    chatGPT：在 Buffer 对象中，存储的数据是通过继承自 std::vector<char> 的机制来管理的。std::vector 类在内部使用动态分配的内存区域来存储元素。
    具体来说，当创建一个 Buffer 对象时，std::vector<char> 的内部机制会自动进行内存分配，以容纳存储的元素。这个内存区域通常被称为动态数组或堆上的数组。
    std::vector<char> 类会根据需要自动管理该内存区域的大小，并在必要时进行内存的重新分配和复制，以适应存储元素的变化。
    因此，尽管没有手动开辟一块内存区域，但在 Buffer 对象中存储的数据仍然是通过 std::vector<char> 内部的动态分配内存来进行管理的。这使得 Buffer 对象可以灵活地存储和管理不同大小的数据。
    */
public:
    typedef std::shared_ptr<Buffer> ptr;
    Buffer() : curpos_(0) {}
    Buffer(const char* s, size_t len) : curpos_(0)
    {
        insert(begin(), s, s + len);
    }
    const char* data() { return &(*this)[0]; }
    const char* curdata() { return &(*this)[curpos_]; }
    size_t cursize() const { return size() - curpos_; }  // 返回当前位置当最后一个元素之间有多少元素
    void offset(int k) { curpos_ += k; }  // 返回当前位置向后移动k个位置
    void append(const char* s, size_t len) { insert(end(), s, s + len); }  // 将字符数组s中从[0-s+len)的内容添加到buffer对象中
    void reset() { 
        curpos_ = 0;
        clear();
    }  // 清空buffer对象中的数据，将当前位置置为0

private:
    size_t curpos_;  // 表示当前位置的索引，它用于支持curdata(),cursize()和offset()函数的实现
};

// 主机字节序是否小端字节序
static bool isLittleEndian()
{
    static uint16_t flag = 1;
    static bool little_end_flag = *((uint8_t*)&flag) == 1;
    return little_end_flag;
}

class Serializer {
public:
    typedef std::shared_ptr<Serializer> ptr;
    Serializer() { buffer_ = std::make_shared<Buffer>(); }
    Serializer(const char* s, size_t len)
    {
        buffer_ = std::make_shared<Buffer>();
        input(s, len);
    }
    Serializer(Buffer::ptr buffer)
        : buffer_(buffer)
    {
    }

    template <typename T>
    void input_type(T t);

    template <typename T>
    void output_type(T& t);

    void reset() { buffer_->reset(); }
    void clear() { reset(); }
    void input(const char* data, int len) { buffer_->append(data, len); }

    template <typename Tuple, std::size_t Id>
    void getv(Serializer& ds, Tuple& t)
    {
        ds >> std::get<Id>(t);  // 将 ds（一个输入流对象）的数据提取到 std::get<Id>(t) 所表示的变量中，其中Id表示索引，t表示对象
    }

    template <typename Tuple, std::size_t... I>
    // ... 表示可变参数模板（variadic templates）或者函数的参数包（parameter pack）
    // std::index_sequence 是一个模板类，用于生成编译时的索引序列
    // 常见用法：生成整数序列
    // template <std::size_t... Indices>
    // void printIndices(index_sequence<Indices...>) {
    //     ((std::cout << Indices << " "), ...);
    // }

    // int main() {
    //     printIndices(std::index_sequence<0, 1, 2, 3>{});
    //     return 0;
    // }
    Tuple get_tuple(std::index_sequence<I...>)  
    {
        Tuple t;
        // 折叠表达式（c++17引入），该表达式属于左折叠（在这个表达式中，逗号运算符 , 用作二元操作符，左操作数是 (getv<Tuple, I>(*this, t) 表达式的结果，右操作数是 0。这个左折叠表达式用于在编译时展开参数包，并对每个展开的参数执行操作）
        std::initializer_list<int> { (getv<Tuple, I>(*this, t), 0)... };  // std::initializer_list<int> 表示一个包含整数类型的初始化列表。在使用 std::initializer_list 时，可以将一组整数值作为参数传递给接受初始化列表的函数或构造函数。
        return t;
    }

    template <typename T>
    Serializer& operator>>(T& i)
    {
        output_type(i);
        return *this;
    }

    template <typename T>
    Serializer& operator<<(T i)
    {
        input_type(i);
        return *this;
    }

    const char* data() { return buffer_->curdata(); }
    size_t size() const { return buffer_->cursize(); }
    std::string toString()
    {
        return std::string(data(), size());
    }

private:
    static void byteOrder(char* s, int len)
    {
        if (isLittleEndian())
            std::reverse(s, s + len);
    }

private:
    int byteOrder_;
    Buffer::ptr buffer_;
};

/*这个函数的设计目的是将变量的数据输入到序列化器中，实现序列化的功能。函数首先获取要输入数据的字节大小，然后通过类型转换将变量的地址转换为 char* 类型的指针，以便进行字节序处理。接着调用 byteOrder 函数进行字节序处理，然后将处理后的数据通过 input 函数输入到序列化器中。通过这个函数，可以方便地将变量的数据序列化为字节流，用于存储、传输或持久化等操作。*/
template <typename T>
inline void Serializer::input_type(T v)
{
    size_t len = sizeof(v);
    char* p = reinterpret_cast<char*>(&v);
    byteOrder(p, len);
    input(p, len);
}

// 偏特化
template <>
inline void Serializer::input_type(std::string v)
{
    // 先存入字符串长度
    uint16_t len = v.size();
    input_type(len);
    byteOrder(const_cast<char*>(v.c_str()), len);
    // 再存入字符串
    input(v.c_str(), len);
}

template <>
inline void Serializer::input_type(const char* v)
{
    input_type<std::string>(std::string(v));
}

/*这个函数的设计目的是将序列化器中的数据输出到指定的变量中，实现反序列化的功能。函数首先确保序列化器中剩余的大小足够容纳要输出的数据，然后使用 memcpy 进行数据拷贝，更新偏移量，并可选地进行字节序的处理。通过这个函数，可以方便地将序列化器中的数据还原为原始数据类型的变量值。*/
template <typename T>
inline void Serializer::output_type(T& v)
{
    size_t len = sizeof(v);
    assert(size() >= len);
    ::memcpy(&v, data(), len);  // ::表示可以访问全局作用域下的标识符
    buffer_->offset(len);
    byteOrder(reinterpret_cast<char*>(&v), len);
}

// 偏特化
template <>
inline void Serializer::output_type(std::string& v)
{
    uint16_t strLen = 0;
    output_type(strLen);
    v = std::string(data(), strLen);
    buffer_->offset(strLen);
    byteOrder(const_cast<char*>(v.c_str()), strLen);
}

#endif