# Details

Date : 2023-09-10 16:09:38

Directory d:\\RECENT\\校招\\技术\\Program\\TinyRPC

Total : 94 files,  7854 codes, 526 comments, 2834 blanks, all 11214 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [.devcontainer/.env](/.devcontainer/.env) | Properties | 5 | 0 | 0 | 5 |
| [.devcontainer/Dockerfile](/.devcontainer/Dockerfile) | Docker | 17 | 10 | 9 | 36 |
| [.devcontainer/devcontainer.json](/.devcontainer/devcontainer.json) | JSON with Comments | 9 | 13 | 5 | 27 |
| [.devcontainer/docker-compose.yml](/.devcontainer/docker-compose.yml) | YAML | 21 | 6 | 8 | 35 |
| [.devcontainer/install-mariadb.sh](/.devcontainer/install-mariadb.sh) | Shell Script | 102 | 10 | 16 | 128 |
| [.devcontainer/install-protobuf.sh](/.devcontainer/install-protobuf.sh) | Shell Script | 12 | 1 | 2 | 15 |
| [.devcontainer/reinstall-cmake.sh](/.devcontainer/reinstall-cmake.sh) | Shell Script | 40 | 7 | 12 | 59 |
| [.devcontainer/tinyxml.sh](/.devcontainer/tinyxml.sh) | Shell Script | 7 | 2 | 0 | 9 |
| [README.md](/README.md) | Markdown | 877 | 24 | 276 | 1,177 |
| [build.sh](/build.sh) | Shell Script | 17 | 1 | 6 | 24 |
| [conf/test_http_server.xml](/conf/test_http_server.xml) | XML | 27 | 10 | 14 | 51 |
| [conf/test_tinypb_server.xml](/conf/test_tinypb_server.xml) | XML | 37 | 11 | 14 | 62 |
| [generator/template/run.sh](/generator/template/run.sh) | Shell Script | 29 | 4 | 13 | 46 |
| [generator/template/shutdown.sh](/generator/template/shutdown.sh) | Shell Script | 28 | 2 | 13 | 43 |
| [generator/tinyrpc_generator.py](/generator/tinyrpc_generator.py) | Python | 468 | 17 | 123 | 608 |
| [makefile](/makefile) | Makefile | 74 | 18 | 38 | 130 |
| [testcases/test_coroutine.cc](/testcases/test_coroutine.cc) | C++ | 67 | 0 | 26 | 93 |
| [testcases/test_http_server.cc](/testcases/test_http_server.cc) | C++ | 148 | 0 | 46 | 194 |
| [testcases/test_tinypb_server.cc](/testcases/test_tinypb_server.cc) | C++ | 62 | 4 | 24 | 90 |
| [testcases/test_tinypb_server_client.cc](/testcases/test_tinypb_server_client.cc) | C++ | 28 | 0 | 13 | 41 |
| [tinyrpc/comm/config.cc](/tinyrpc/comm/config.cc) | C++ | 220 | 0 | 60 | 280 |
| [tinyrpc/comm/config.h](/tinyrpc/comm/config.h) | C++ | 49 | 2 | 27 | 78 |
| [tinyrpc/comm/error_code.h](/tinyrpc/comm/error_code.h) | C++ | 21 | 3 | 12 | 36 |
| [tinyrpc/comm/log.cc](/tinyrpc/comm/log.cc) | C++ | 337 | 12 | 109 | 458 |
| [tinyrpc/comm/log.h](/tinyrpc/comm/log.h) | C++ | 161 | 1 | 71 | 233 |
| [tinyrpc/comm/msg_req.cc](/tinyrpc/comm/msg_req.cc) | C++ | 50 | 2 | 11 | 63 |
| [tinyrpc/comm/msg_req.h](/tinyrpc/comm/msg_req.h) | C++ | 10 | 0 | 8 | 18 |
| [tinyrpc/comm/mysql_instase.cc](/tinyrpc/comm/mysql_instase.cc) | C++ | 188 | 10 | 42 | 240 |
| [tinyrpc/comm/mysql_instase.h](/tinyrpc/comm/mysql_instase.h) | C++ | 64 | 0 | 40 | 104 |
| [tinyrpc/comm/run_time.h](/tinyrpc/comm/run_time.h) | C++ | 11 | 0 | 6 | 17 |
| [tinyrpc/comm/start.cc](/tinyrpc/comm/start.cc) | C++ | 44 | 3 | 15 | 62 |
| [tinyrpc/comm/start.h](/tinyrpc/comm/start.h) | C++ | 32 | 1 | 15 | 48 |
| [tinyrpc/comm/string_util.cc](/tinyrpc/comm/string_util.cc) | C++ | 49 | 1 | 12 | 62 |
| [tinyrpc/comm/string_util.h](/tinyrpc/comm/string_util.h) | C++ | 15 | 6 | 10 | 31 |
| [tinyrpc/comm/thread_pool.cc](/tinyrpc/comm/thread_pool.cc) | C++ | 44 | 3 | 16 | 63 |
| [tinyrpc/comm/thread_pool.h](/tinyrpc/comm/thread_pool.h) | C++ | 26 | 0 | 15 | 41 |
| [tinyrpc/coroutine/coctx.h](/tinyrpc/coroutine/coctx.h) | C++ | 18 | 1 | 9 | 28 |
| [tinyrpc/coroutine/coroutine.cc](/tinyrpc/coroutine/coroutine.cc) | C++ | 133 | 31 | 50 | 214 |
| [tinyrpc/coroutine/coroutine.h](/tinyrpc/coroutine/coroutine.h) | C++ | 74 | 2 | 43 | 119 |
| [tinyrpc/coroutine/coroutine_hook.cc](/tinyrpc/coroutine/coroutine_hook.cc) | C++ | 215 | 49 | 82 | 346 |
| [tinyrpc/coroutine/coroutine_hook.h](/tinyrpc/coroutine/coroutine_hook.h) | C++ | 27 | 0 | 24 | 51 |
| [tinyrpc/coroutine/coroutine_pool.cc](/tinyrpc/coroutine/coroutine_pool.cc) | C++ | 61 | 6 | 21 | 88 |
| [tinyrpc/coroutine/coroutine_pool.h](/tinyrpc/coroutine/coroutine_pool.h) | C++ | 23 | 4 | 15 | 42 |
| [tinyrpc/coroutine/memory.cc](/tinyrpc/coroutine/memory.cc) | C++ | 69 | 12 | 13 | 94 |
| [tinyrpc/coroutine/memory.h](/tinyrpc/coroutine/memory.h) | C++ | 30 | 1 | 18 | 49 |
| [tinyrpc/net/abstract_codec.h](/tinyrpc/net/abstract_codec.h) | C++ | 22 | 0 | 15 | 37 |
| [tinyrpc/net/abstract_data.h](/tinyrpc/net/abstract_data.h) | C++ | 12 | 0 | 6 | 18 |
| [tinyrpc/net/abstract_dispatcher.h](/tinyrpc/net/abstract_dispatcher.h) | C++ | 17 | 0 | 12 | 29 |
| [tinyrpc/net/byte.h](/tinyrpc/net/byte.h) | C++ | 13 | 0 | 8 | 21 |
| [tinyrpc/net/fd_event.cc](/tinyrpc/net/fd_event.cc) | C++ | 153 | 3 | 45 | 201 |
| [tinyrpc/net/fd_event.h](/tinyrpc/net/fd_event.h) | C++ | 63 | 0 | 42 | 105 |
| [tinyrpc/net/http/http_codec.cc](/tinyrpc/net/http/http_codec.cc) | C++ | 184 | 5 | 32 | 221 |
| [tinyrpc/net/http/http_codec.h](/tinyrpc/net/http/http_codec.h) | C++ | 22 | 0 | 12 | 34 |
| [tinyrpc/net/http/http_define.cc](/tinyrpc/net/http/http_define.cc) | C++ | 46 | 19 | 20 | 85 |
| [tinyrpc/net/http/http_define.h](/tinyrpc/net/http/http_define.h) | C++ | 39 | 21 | 24 | 84 |
| [tinyrpc/net/http/http_dispatcher.cc](/tinyrpc/net/http/http_dispatcher.cc) | C++ | 42 | 0 | 14 | 56 |
| [tinyrpc/net/http/http_dispatcher.h](/tinyrpc/net/http/http_dispatcher.h) | C++ | 18 | 0 | 13 | 31 |
| [tinyrpc/net/http/http_request.h](/tinyrpc/net/http/http_request.h) | C++ | 22 | 0 | 13 | 35 |
| [tinyrpc/net/http/http_response.h](/tinyrpc/net/http/http_response.h) | C++ | 19 | 0 | 9 | 28 |
| [tinyrpc/net/http/http_servlet.cc](/tinyrpc/net/http/http_servlet.cc) | C++ | 51 | 1 | 22 | 74 |
| [tinyrpc/net/http/http_servlet.h](/tinyrpc/net/http/http_servlet.h) | C++ | 28 | 0 | 24 | 52 |
| [tinyrpc/net/mutex.cc](/tinyrpc/net/mutex.cc) | C++ | 57 | 2 | 18 | 77 |
| [tinyrpc/net/mutex.h](/tinyrpc/net/mutex.h) | C++ | 178 | 11 | 42 | 231 |
| [tinyrpc/net/net_address.cc](/tinyrpc/net/net_address.cc) | C++ | 95 | 3 | 26 | 124 |
| [tinyrpc/net/net_address.h](/tinyrpc/net/net_address.h) | C++ | 58 | 0 | 48 | 106 |
| [tinyrpc/net/reactor.cc](/tinyrpc/net/reactor.cc) | C++ | 331 | 37 | 86 | 454 |
| [tinyrpc/net/reactor.h](/tinyrpc/net/reactor.h) | C++ | 70 | 4 | 45 | 119 |
| [tinyrpc/net/tcp/abstract_slot.h](/tinyrpc/net/tcp/abstract_slot.h) | C++ | 25 | 0 | 10 | 35 |
| [tinyrpc/net/tcp/io_thread.cc](/tinyrpc/net/tcp/io_thread.cc) | C++ | 152 | 9 | 45 | 206 |
| [tinyrpc/net/tcp/io_thread.h](/tinyrpc/net/tcp/io_thread.h) | C++ | 56 | 4 | 41 | 101 |
| [tinyrpc/net/tcp/tcp_buffer.cc](/tinyrpc/net/tcp/tcp_buffer.cc) | C++ | 96 | 17 | 37 | 150 |
| [tinyrpc/net/tcp/tcp_buffer.h](/tinyrpc/net/tcp/tcp_buffer.h) | C++ | 33 | 2 | 35 | 70 |
| [tinyrpc/net/tcp/tcp_client.cc](/tinyrpc/net/tcp/tcp_client.cc) | C++ | 153 | 1 | 26 | 180 |
| [tinyrpc/net/tcp/tcp_client.h](/tinyrpc/net/tcp/tcp_client.h) | C++ | 55 | 3 | 28 | 86 |
| [tinyrpc/net/tcp/tcp_connection.cc](/tinyrpc/net/tcp/tcp_connection.cc) | C++ | 257 | 23 | 68 | 348 |
| [tinyrpc/net/tcp/tcp_connection.h](/tinyrpc/net/tcp/tcp_connection.h) | C++ | 82 | 0 | 50 | 132 |
| [tinyrpc/net/tcp/tcp_connection_time_wheel.cc](/tinyrpc/net/tcp/tcp_connection_time_wheel.cc) | C++ | 31 | 2 | 10 | 43 |
| [tinyrpc/net/tcp/tcp_connection_time_wheel.h](/tinyrpc/net/tcp/tcp_connection_time_wheel.h) | C++ | 26 | 0 | 20 | 46 |
| [tinyrpc/net/tcp/tcp_server.cc](/tinyrpc/net/tcp/tcp_server.cc) | C++ | 211 | 23 | 60 | 294 |
| [tinyrpc/net/tcp/tcp_server.h](/tinyrpc/net/tcp/tcp_server.h) | C++ | 73 | 0 | 54 | 127 |
| [tinyrpc/net/timer.cc](/tinyrpc/net/timer.cc) | C++ | 132 | 8 | 33 | 173 |
| [tinyrpc/net/timer.h](/tinyrpc/net/timer.h) | C++ | 56 | 2 | 33 | 91 |
| [tinyrpc/net/tinypb/tinypb_codec.cc](/tinyrpc/net/tinypb/tinypb_codec.cc) | C++ | 221 | 13 | 70 | 304 |
| [tinyrpc/net/tinypb/tinypb_codec.h](/tinyrpc/net/tinypb/tinypb_codec.h) | C++ | 18 | 4 | 16 | 38 |
| [tinyrpc/net/tinypb/tinypb_data.h](/tinyrpc/net/tinypb/tinypb_data.h) | C++ | 30 | 6 | 9 | 45 |
| [tinyrpc/net/tinypb/tinypb_rpc_async_channel.cc](/tinyrpc/net/tinypb/tinypb_rpc_async_channel.cc) | C++ | 114 | 2 | 25 | 141 |
| [tinyrpc/net/tinypb/tinypb_rpc_async_channel.h](/tinyrpc/net/tinypb/tinypb_rpc_async_channel.h) | C++ | 51 | 2 | 26 | 79 |
| [tinyrpc/net/tinypb/tinypb_rpc_channel.cc](/tinyrpc/net/tinypb/tinypb_rpc_channel.cc) | C++ | 89 | 2 | 17 | 108 |
| [tinyrpc/net/tinypb/tinypb_rpc_channel.h](/tinyrpc/net/tinypb/tinypb_rpc_channel.h) | C++ | 22 | 1 | 11 | 34 |
| [tinyrpc/net/tinypb/tinypb_rpc_closure.h](/tinyrpc/net/tinypb/tinypb_rpc_closure.h) | C++ | 22 | 0 | 12 | 34 |
| [tinyrpc/net/tinypb/tinypb_rpc_controller.cc](/tinyrpc/net/tinypb/tinypb_rpc_controller.cc) | C++ | 68 | 0 | 24 | 92 |
| [tinyrpc/net/tinypb/tinypb_rpc_controller.h](/tinyrpc/net/tinypb/tinypb_rpc_controller.h) | C++ | 49 | 3 | 41 | 93 |
| [tinyrpc/net/tinypb/tinypb_rpc_dispatcher.cc](/tinyrpc/net/tinypb/tinypb_rpc_dispatcher.cc) | C++ | 123 | 0 | 37 | 160 |
| [tinyrpc/net/tinypb/tinypb_rpc_dispatcher.h](/tinyrpc/net/tinypb/tinypb_rpc_dispatcher.h) | C++ | 23 | 3 | 18 | 44 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)