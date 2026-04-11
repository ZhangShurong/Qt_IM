# Qt_IM - Qt Instant Messaging System

# Qt_IM - Qt 即时通讯系统

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://qt.io)
[![Qt](https://img.shields.io/badge/Qt-4.8%2B-green.svg)](https://qt.io)
[![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()
[![Tests](https://img.shields.io/badge/Tests-26%2F26-passing-brightgreen.svg)]()

---

## Table of Contents / 目录

- [Project Overview / 项目概述](#project-overview--项目概述)
- [Features / 功能特性](#features--功能特性)
- [Architecture / 系统架构](#architecture--系统架构)
- [Project Structure / 项目结构](#project-structure--项目结构)
- [Quick Start / 快速开始](#quick-start--快速开始)
  - [Prerequisites / 环境要求](#prerequisites--环境要求)
  - [Building / 编译构建](#building--编译构建)
  - [Running / 运行程序](#running--运行程序)
- [Protocol Design / 协议设计](#protocol-design--协议设计)
- [Unit Tests / 单元测试](#unit-tests--单元测试)
- [Technical Highlights / 技术亮点](#technical-highlights--技术亮点)
- [Contributing / 贡献指南](#contributing--贡献指南)
- [License / 许可证](#license--许可证)

---

## Project Overview / 项目概述

**English:** Qt_IM is a feature-rich instant messaging (IM) system built with the Qt framework. It provides QQ-like and WeChat-like user interfaces with complete client-server architecture. The system supports real-time messaging, file transfer, group chat, friend management, and secure communication through encryption.

**中文：** Qt_IM 是一个基于 Qt 框架开发的功能丰富的即时通讯（IM）系统。它提供类似 QQ 和微信的用户界面，采用完整的客户端-服务器架构。该系统支持实时消息、文件传输、群聊、好友管理以及加密安全通信。

---

## Features / 功能特性

### Core Features / 核心功能

| Feature | Description | 描述 |
|---------|-------------|------|
| **User Authentication** | Registration, login with password recovery | 用户注册、登录及密码找回 |
| **Real-time Messaging** | One-to-one and group text messaging | 一对一和群组文字聊天 |
| **File Transfer** | Send and receive files between users | 用户间文件收发 |
| **Friend Management** | Add/remove friends, online status display | 好友增删、在线状态显示 |
| **Group Chat** | Create/join groups, group member management | 创建/加入群聊、成员管理 |
| **Message Encryption** | XOR and AES encryption support | XOR 和 AES 加密支持 |
| **SQLite Storage** | Local message and user data persistence | 本地消息和用户数据持久化 |
| **Cross-platform** | Windows, Linux, macOS support | 支持 Windows、Linux、macOS |

### UI Features / 界面功能

- **QQ-style Interface**: Classic QQ-like chat window design
- **WeChat-style Interface**: Modern WeChat-like UI alternative
- **Custom Calendar Widget**: Built-in calendar component
- **Discussion Groups**: Group chat with member list
- **File Transfer Dialog**: Visual file sending progress
- **Friend List Panel**: Categorized friend display

---

## Architecture / 系统架构

### System Architecture Diagram / 系统架构图

```
┌─────────────────────────────────────────────────────────────────┐
│                        Client Side                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐   │
│  │   CV_Client  │  │    IMClient  │  │      CV_IM           │   │
│  │  (QQ Style)  │  │  (MVC Arch)  │  │   (Simple Client)    │   │
│  └──────┬───────┘  └──────┬───────┘  └──────────┬───────────┘   │
│         │                 │                      │               │
│         └─────────────────┼──────────────────────┘               │
│                           │                                      │
│                    TCP Socket (JSPP Protocol)                     │
│                           │                                      │
├───────────────────────────┼──────────────────────────────────────┤
│                       Server Side                                │
│                           │                                      │
│         ┌─────────────────┼─────────────────┐                   │
│         │                 │                 │                    │
│  ┌──────┴───────┐  ┌──────┴───────┐  ┌──────┴───────┐          │
│  │   CV_Server  │  │   IMServer   │  │   Database   │          │
│  │              │  │  (MVC Arch)  │  │   (SQLite)   │          │
│  └──────────────┘  └──────────────┘  └──────────────┘          │
└─────────────────────────────────────────────────────────────────┘
```

### MVC Architecture (IMClient/IMServer) / MVC 架构

```
┌─────────────────────────────────────────────────────────────┐
│                        Model Layer                          │
│  IMUser | IMEncryption | IMConstant | IMTcpSocket          │
├─────────────────────────────────────────────────────────────┤
│                      Control Layer                          │
│  IMClientSocketCtrl | IMServerMessageCtrl | IMDatabaseCtrl │
├─────────────────────────────────────────────────────────────┤
│                        View Layer                           │
│  IMChatWidget | IMMailListWidget | IMFlockListWidget | ...  │
└─────────────────────────────────────────────────────────────┘
```

---

## Project Structure / 项目结构

```
Qt_IM/
├── CV_Client/                  # QQ-style client (older version)
│   ├── IM/                     # Core IM logic
│   │   ├── imclient.h/cpp      # IM client implementation
│   │   ├── connection.h/cpp    # Connection management
│   │   └── jspp.h/cpp          # JSPP protocol handler
│   ├── utils/                  # Utility functions
│   │   └── utils.cpp           # AES encryption utilities
│   └── CV_Client.pro           # Qt project file
│
├── CV_Server/                  # Server for CV_Client
│   ├── main.cpp                # Server entry point
│   └── CVServers.pro           # Qt project file
│
├── IMClient/                   # Modern MVC-based client
│   ├── model/                  # Data models
│   │   ├── IMUser.h            # User model (friends, groups)
│   │   ├── IMEncryption.h     # Encryption module
│   │   ├── IMConstant.h       # Constants definition
│   │   └── IMTcpSocket.h      # TCP socket wrapper
│   ├── view/                   # UI components
│   │   ├── IMChatWidget.h     # Chat window widget
│   │   ├── IMMailListWidget.h # Friend list widget
│   │   ├── IMFlockListWidget.h# Group list widget
│   │   └── ...                 # Other UI components
│   ├── external/               # External libraries
│   │   └── CustomCalendar/     # Custom calendar component
│   └── IMClient.pro            # Qt project file
│
├── IMServer/                   # Modern MVC-based server
│   ├── model/                  # Server models
│   │   ├── IMThread.h          # Thread management
│   │   ├── IMTcpServer.h       # TCP server implementation
│   │   └── ...
│   ├── view/                   # Server UI
│   │   └── IMServerWindow.h   # Server control panel
│   ├── control/                # Business logic controllers
│   │   ├── IMDatabaseCtrl.h   # Database controller
│   │   ├── IMServerMessageCtrl.h # Message handling
│   │   └── IMClientSocketCtrl.h  # Client connection ctrl
│   └── IMServer.pro            # Qt project file
│
├── CV_IM/                      # Simple IM client variant
│   └── CV_IM.pro               # Qt project file
│
├── tests/                      # Unit tests
│   ├── test_framework.h        # Lightweight test framework
│   ├── test_main.cpp           # Test cases (26 tests)
│   └── CMakeLists.txt          # Test build config
│
├── tmp/                        # Reference projects & temp files
│   ├── UI/                     # Reference UI implementations
│   │   ├── QQLikeUI-master/    # QQ-like UI reference
│   │   └── weChat-master/      # WeChat-like UI reference
│   └── CV_Server/              # Reference server code
│
├── protocol.json               # JSPP protocol examples
├── LICENSE                     # GPL v3 License
├── README.md                   # This file
└── .gitignore                  # Git ignore rules
```

---

## Quick Start / 快速开始

### Prerequisites / 环境要求

#### Required / 必需依赖

| Dependency | Version | Purpose | 用途 |
|------------|---------|---------|------|
| **Qt Framework** | 4.8+ / 5.x | GUI & Networking | 图形界面与网络通信 |
| **C++ Compiler** | C++11 compatible | Code compilation | 代码编译 |
| **CMake** | 3.10+ | Test building (optional) | 测试构建（可选） |
| **SQLite3** | 3.x | Data storage | 数据存储 |

#### Installation Examples / 安装示例

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev libsqlite3-dev cmake g++
# For Qt4:
# sudo apt-get install qt4-default libsqlite3-dev cmake g++
```

**CentOS/RHEL:**
```bash
sudo yum install qt5-qtbase-devel sqlite-devel cmake gcc-c++
```

**macOS (Homebrew):**
```brew install qt5 sqlite cmake```

**Windows:**
- Download Qt from https://qt.io/download
- Install Qt Creator with Qt Components
- Install CMake from https://cmake.org/download/

---

### Building / 编译构建

#### Option 1: Using qmake (Recommended) / 使用 qmake（推荐）

**Build CV_Client + CV_Server:**
```bash
# Build Server
cd CV_Server
qmake CVServers.pro
make

# Build Client
cd ../CV_Client
qmake CV_Client.pro
make
```

**Build IMClient + IMServer (Modern Version):**
```bash
# Build Server
cd IMServer
qmake IMServer.pro
make

# Build Client
cd ../IMClient
qmake IMClient.pro
make
```

#### Option 2: Using Qt Creator / 使用 Qt Creator

1. Open `CV_Client/CV_Client.pro` or `IMClient/IMClient.pro`
2. Configure build kit (Desktop Qt 5.x GCC 64bit)
3. Click "Build" -> "Build Project"
4. Run from Qt Creator or execute the built binary

#### Option 3: Building Unit Tests / 构建单元测试

```bash
cd tests
mkdir build && cd build
cmake ..
make
./run_tests
```

---

### Running / 运行程序

#### Step 1: Start Server / 启动服务器

```bash
cd IMServer
./IMServer
# Or for older version:
# cd CV_Server
# ./CV_Server
```

The server will start listening on the configured port (default: **8888**).

#### Step 2: Start Client / 启动客户端

```bash
cd IMClient
./IMClient
# Or for older version:
# cd CV_Client
# ./CV_Client
```

#### Step 3: Register & Login / 注册与登录

1. Click "Register" to create a new account
2. Enter username, password, and security question
3. Login with your credentials
4. Add friends and start chatting!

---

## Protocol Design / 协议设计

### JSPP Protocol / JSPP 协议

Qt_IM uses **JSPP (JSON-like Simple Protocol)** for client-server communication.

**Protocol Format Example / 协议格式示例:**

```json
{
    "reg": {
        "user_name": "username",
        "pwd": "password",
        "question": "security_question",
        "answer": "security_answer"
    },
    "login": {
        "user_name": "username",
        "pwd": "password"
    },
    "conversation": {
        "message": {
            "from": "sender@domain.com",
            "to": "receiver@domain.com",
            "body": "Hello World!"
        }
    }
}
```

**Message Types / 消息类型:**

| Type | Description | 用途 |
|------|-------------|------|
| `reg` | User registration | 用户注册 |
| `login` | User authentication | 用户登录 |
| `conversation` | Chat messages | 聊天消息 |
| `friend` | Friend operations | 好友操作 |
| `group` | Group chat operations | 群聊操作 |
| `file` | File transfer | 文件传输 |

---

## Unit Tests / 单元测试

### Test Coverage / 测试覆盖

The project includes comprehensive unit tests covering core functionality:

| Module | Test Cases | Status |
|--------|------------|--------|
| JSPP Protocol | 6 tests | All Passing |
| User Model | 6 tests | All Passing |
| Encryption | 5 tests | All Passing |
| Utility Functions | 9 tests | All Passing |
| **Total** | **26 tests** | **All Passing** |

### Running Tests / 运行测试

```bash
cd tests/build
./run_tests
```

**Expected Output / 预期输出:**

```
========================================
  Qt_IM Unit Test Suite
========================================

[PASS] JSPP_SerializeEmpty
[PASS] JSPP_SerializeLogin
[PASS] JSPP_DeserializeLogin
[PASS] JSPP_SerializeMessage
[PASS] JSPP_DeserializeMessage
[PASS] JSPP_RoundTrip
[PASS] User_CreateUser
[PASS] User_AddFriend
[PASS] User_RemoveFriend
[PASS] User_FriendList
[PASS] User_GroupManagement
[PASS] User_OnlineStatus
[PASS] XOR_BasicOperation
[PASS] XOR_SymmetricProperty
[PASS] XOR_KnownValues
[PASS] XOR_EmptyString
[PASS] XOR_SpecialCharacters
[PASS] Utils_StringSplit
[PASS] Utils_StringJoin
[PASS] Utils_StringTrim
[PASS] Utils_IPPortParsing
[PASS] Utils_VectorOperations
[PASS] Utils_MapOperations
[PASS] Utils_TimestampFormat
[PASS] Utils_ValidationFunctions
[PASS] Utils_ConversionFunctions

========================================
  Results: 26/26 passed (100%)
========================================
```

---

## Technical Highlights / 技术亮点

### 1. Cross-Platform Compatibility / 跨平台兼容性

- Uses Qt's `QTcpSocket` instead of platform-specific sockets
- Compatible with Windows, Linux, and macOS
- No external dependencies on OS-specific libraries

### 2. Secure Communication / 安全通信

```cpp
// XOR Encryption Example
void encrypt(char* data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

// AES Encryption Support (via Crypto++ library)
int AES_utils::encrypt(unsigned char *in, size_t src_len, unsigned char *out);
```

### 3. MVC Architecture Pattern / MVC 架构模式

Clean separation of concerns:
- **Model**: Data structures and business entities
- **View**: UI components and user interaction
- **Control**: Business logic and coordination

### 4. Thread-Safe Design / 线程安全设计

- `QThread` based concurrent processing
- Mutex protection for shared resources
- Signal-slot mechanism for thread communication

### 5. Memory Management / 内存管理

- Smart pointers where applicable
- RAII pattern for resource management
- Fixed memory leaks in encryption modules

---

## Contributing / 贡献指南

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

欢迎贡献代码！请遵循以下步骤：

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 提交 Pull Request

### Code Style / 代码规范

- Follow Qt coding conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Ensure code compiles without warnings

---

## License / 许可证

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for details.

本项目基于 **GNU General Public License v3.0** 许可证开源 - 详见 [LICENSE](LICENSE) 文件。

---

## Acknowledgments / 致谢

- **Qt Framework** - Cross-platform application framework
- **Crypto++** - C++ library for cryptographic schemes
- **SQLite** - SQL database engine
- **Reference UI Projects** - QQLikeUI and weChat UI designs

---

## Contact / 联系方式

- **Author**: ZhangShurong
- **Repository**: [https://github.com/ZhangShurong/Qt_IM](https://github.com/ZhangShurong/Qt_IM)
- **Issues**: Please use GitHub Issues for bug reports and feature requests

---

<p align="center">
  <strong>Qt_IM - A Modern Instant Messaging System</strong><br>
  <em>Built with Qt | Powered by C++</em>
</p>
