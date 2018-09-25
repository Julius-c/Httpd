# 命令行工具`httpd`
1. 功能描述
    + 启动一个http服务器，监听端口由`-p`或`--port`指定，响应HTTP GET请求。URL的`/`被映射到DIR目录。
2. 运行方式
    + `httpd [OPTION]... DIR`
    + 如`./httpd --port 8000 ./site`
3. 命令行选项
    + `-p` **PORT**, `--port` **PORT**: 指定http服务监听**PORT**的端口
    + `-h`，`--help`: 打印帮助信息。
