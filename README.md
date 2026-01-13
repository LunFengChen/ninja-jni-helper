# JNI Helper

Binary Ninja 插件，自动加载 JNI 类型并设置函数签名，让分析 Android SO 更轻松。

作者：[LunFengChen](https://github.com/LunFengChen) 交流群：686725227 (QQ)

## 功能

- 自动加载 JNI 类型库（`JNIEnv*`, `JavaVM*`, `jobject` 等）
- 自动设置 `JNI_OnLoad`、`Java_*` 函数签名
- 导入 Frida RegisterNatives JSON，命名动态注册的 JNI 函数(功能3未测试，有问题和我反馈就行)

## 安装

```powershell
# Windows (PowerShell)
cd "$env:APPDATA\Binary Ninja\plugins"
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

```cmd
# Windows (CMD)
cd "%APPDATA%\Binary Ninja\plugins"
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

```bash
# macOS/Linux(未测试)
cd ~/.binaryninja/plugins
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

打开 Binary Ninja 加载so即可。

## 使用

### 自动功能

打开 Android SO 文件后自动执行：
1. 加载 JNI 类型库
2. 设置 `JNI_OnLoad(JavaVM* vm, void* reserved)` 签名
3. 设置 `Java_*` 函数签名：`jobject Java_xxx(JNIEnv* env, jobject thiz, ...)`

### 导入动态注册函数

1. 运行 Frida 脚本获取 RegisterNatives JSON（脚本来自 [jadx-frida-hookAll](https://github.com/LunFengChen/jadx-frida-hookAll)）：

```javascript
findJNIfunc_byClassName("com.example.ClassName");
```

输出格式：
```json
{
  "class_name": "com.example.Class",
  "name&signature": "methodName(Landroid/content/Context;II)Ljava/lang/String;",
  "which_so": "libexample.so",
  "func_offest": "0x12345"
}
```

2. Binary Ninja 菜单：`Plugins > JNI Helper > Import Frida RegisterNatives JSON`

3. 粘贴 JSON 内容，自动命名函数并设置签名

## 支持架构

- aarch64 (ARM64)
- armv7 (ARM32)

---

[English](#english)

<a name="english"></a>
# JNI Helper (English)

Binary Ninja plugin for automatic JNI type loading and function signature setup.

Author: [LunFengChen](https://github.com/LunFengChen)

QQ Group: 686725227

## Features

- Auto-load JNI type library (`JNIEnv*`, `JavaVM*`, `jobject`, etc.)
- Auto-set `JNI_OnLoad` and `Java_*` function signatures
- Import Frida RegisterNatives JSON for dynamic JNI functions

## Installation

```powershell
# Windows (PowerShell)
cd "$env:APPDATA\Binary Ninja\plugins"
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

```cmd
# Windows (CMD)
cd "%APPDATA%\Binary Ninja\plugins"
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

```bash
# macOS/Linux
cd ~/.binaryninja/plugins
git clone https://github.com/LunFengChen/ninja-jni-helper.git
```

Restart Binary Ninja.

## Usage

### Automatic

Open an Android SO file, the plugin will:
1. Load JNI type library
2. Set `JNI_OnLoad(JavaVM* vm, void* reserved)` signature
3. Set `Java_*` signatures: `jobject Java_xxx(JNIEnv* env, jobject thiz, ...)`

### Import Dynamic JNI Functions

1. Run Frida script to get RegisterNatives JSON (from [jadx-frida-hookAll](https://github.com/LunFengChen/jadx-frida-hookAll))
2. Menu: `Plugins > JNI Helper > Import Frida RegisterNatives JSON`
3. Paste JSON content

## Supported Architectures

- aarch64 (ARM64)
- armv7 (ARM32)
