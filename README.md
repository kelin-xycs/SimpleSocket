# SimpleSocket
一个 用 C#  调用  Win Socket 2  的 程序 ，   对  Win Socket 2  简单 的  调用



用 C++ 对 Win Socket 2 简单的 包装 ， 再用 C# 调用 。

测试结果 可以达到 每秒请求数 2万 ， 和  System.Net.Sockets Socket 一个水平 。

其实昨天测试的 时候 好像达到  25000 ， 或者  23000  了，  今天又变成了 2万  。  可能 电脑 问题 吧 。 ~


C++       的 代码 是 从  微软 docs 网站 上 拷贝 的 ， 做了一些 修改 。

Win Socket 2 ：  https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/getting-started-with-winsock       

Win Socket 2 API ：   https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsastartup              

Example Code ：   https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/complete-server-code

当然 ， 总的来说 ，  System.Net.Sockets Socket  的 表现 还是 要 更高 那么 微微 的 一点点 啦 。 

SimpleSocket 使用的是 socket ，  System.Net.Sockets Socket  使用 的 是  WSASocket  。

socket 和  WSASocket  都是  Win Socket 2  的 组成部分 。  

socket 是 对 Socket 规范 的 标准实现 。  WSASocket 是 微软 的 高性能 Socket 实现 。

通过 反编译 System.Net.Sockets Socket 的 代码 ， 可以发现 ， Socket 对象 的 内核对象 SafeCloseSocket ， 是 通过  WSASocket() 返回 的 。
WSASocket()  方法 是 一个 非托管方法， 也是 Win Socket 2 的 标准 API 。 
有意思的 是， 在 反编译 代码中， WSASocket() 方法返回的 对象 是一个 .Net 对象 。 
在 .Net 代码 中 可以和 其他 .Net 对象一样正常使用，没有做任何特殊处理 。

而 作为 Win Socket 2 的 API ，  WSASocket()  方法 返回 的 socket 对象， 也 同样 可以在 非托管代码 中 使用 ， 包括 C++ 或者 其他语言 。 
所以，WSASocket()  方法 返回 的 socket 对象， 既是一个 .Net 对象（CLR 对象） ， 又是 一个 非托管 对象 。

对象 ， 在 内存 里 是 字段 和 函数指针 的 排列 ， 所以， 这个 socket 对象 ， 大概 是 符合  .Net CLR 规范 的 对象 吧 。 

当然， 我 对 反编译 代码 看的 并不仔细 ， 所以 也 有其他 各种 可能 。 
但 用 非托管方法 返回一个 托管对象 ，这是  微软 内部 在  IL 和 非托管代码 之间 的 一个 内部 协议 吧 。

而 作为 非托管 对象 。 我 在想的是， 微软内部使用的 非托管对象 ，是不是 只有  COM ？  或者 还有 其他 规范 ？

WSASocket 可以实现 Overlap 方式 的 Receive 和 Send ，  Overlap 的 Send 还好理解， Overlap 的 Receive 不知是 干什么 ？
通过 查看 System.Net.Sockets Socket 的 代码， 
可以看到  Socket.Receive() 和 Socket.Send() 方法 里面 调用的 仍然是 recv() 和 send() 方法（非托管方法），
并不是 WSARecv() 和 WSASend() 方法（非托管方法） 。 

recv() send() WSARecv() WSASend()   都是  Win Socket 2  的  API  。 



用 C++ 包装 Win Socket 2 ， 提供出 简单 的接口， 这样 就可以 避免 用 C# 去 实现 Win Socket 2 里 定义的 结构体， 或者说 头文件 。

C# 和  非托管代码  之间 的 交互  采用  值类型 ， ref 结构体（struct） 和  数组   传递 参数 效率不错 。  

ref 结构体 和 数组  可以 避免  托管代码 和 非托管 代码 之间 的  数据复制（封送） ，  而且 可以 带回 丰富 的 返回值  。 

但 结构体 最好是 字段是 值类型 的 结构体 。 并且 C++ 中的 结构体 的 字段类型 名字 数量 要和 C# 的 结构体 一致 ， 不然 可能导致 奇怪 的 问题 。 

数组 最好 作为 一个 单独 的 参数 传递 ， 不要 放在 结构体 中 作为 结构体 的 字段 ， 
因为 结构体 里 的 数组 只能是 SafeArray ，  且  需要 在 结构体 的 声明 里 声明 固定长度 。 
参考  数组的默认封送处理  
https://docs.microsoft.com/zh-cn/dotnet/framework/interop/default-marshaling-for-arrays#cpcondefaultmarshalingforarraysanchor4


数组 作为 参数 传递 到 非托管方法 时 ， 不需要 加  ref 关键字 ， 数组 默认就是 按 引用传递 的 （传递首地址） ，  
C++ 对应的 方法 里 声明 一个 指针 参数 来 接收 数组首地址 就行 。            

值类型 是 按值 传递 的 。 
如 int  uint （C++ 里是 unsigned int） ， short ， long  这些 在 C# 和 C++ 之间 定义相同 的 通用类型 应该 可以 放心使用 。  

这次在 结构体 传递参数 上 出了一个 问题 。 导致 花了 很长时间 来 找 。

我一开始使用 结构体 里的  string errorMsg 字段 来 传递 Error Message 。  C++ 里 对应的参数是 char *  类型 。 
结果就导致 当 客户端 强迫关闭 连接 时， 服务器 进程 退出， 且 连 控制台 窗口 也 唰 的 一下 没 了 。

实际上 后来 发现原因 是， 当  socket 返回 error code 时， 会 对  errorMsg 赋值 ， 
把一个 字符串常量 赋值 给 errorMsg 字段（如 “发生了 Receive 错误”） ，
通过 errorMsg 字段 把 Error Message 返回 C#   ， 
但 这个 赋值 就出错 了 ，   这个 出错 会 使 进程退出 。  

更奇葩 的 是， 我 在 其中一个 结构体 上 加了  [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]  这个 标注  。
于是 ，  不止 进程退出 ， 连 控制台 窗口 也 关闭 了 （唰的一下没了）  。  如果 不加 这个 批注 ， 那么 进程退出 了 ，  
但  控制台 窗口 不会 关闭 。  会 显示  “按任意键退出……”   。

因为 是 非托管代码 里的 错误 ，  不会 反馈 什么 错误信息 ，  再加上 进程退出 ， 窗口关闭 ， 所以 不好捕捉到 问题 。 
所以 花了 很长时间 来 找这个 Bug  。    在  Windows   的 事件日志 里 也没有 错误日志 记录 。        

至于 为什么 会把  C++ 里的  字符串  赋值 给   C# 结构体 里的  string  类型 。   
大概是 一开始 对 P/Invoke （System.Runtime.InteropServices） 不熟悉 ，  
所以 把  按 引用（指针） 传递  和  封送（Marshal 类 提供的 方法）  等 几种情况  混起来 想了  。

后来 想想 也知道 ， C++ 里 的 字符串 是一个  char[] 字符数组 ， C# 里的 string 是 一个 .Net 对象 ， 它 包含 了一个  char[] ， 
但是 还 包含 了 其它 的 一些 东西 。   string 的  对象结构  不仅仅 是一个  char[]   。    

但 有意思 的 是 ，  如果 在 C++ 结构体 里 声明  const char *  字段 ， 那么 可以取到 对应的 C# 结构体 里的 string 字段 的 值  。 ：）



我现在 提倡 这样 的 架构 ， 用   C\C++   实现 底层 逻辑 ， 并 提供出简单接口 ， 用 C# 处理 上层 逻辑  。  

注意 是 简单接口 ，  不要搞 什么 对象 之类 的 。        函数 指针 数组 结构体     这些 足够 了  。  ^^




























































