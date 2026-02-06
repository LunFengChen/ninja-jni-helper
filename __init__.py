# -*- coding: utf-8 -*-
"""
JNI Helper - Binary Ninja plugin for Android SO analysis.

Auto-load JNI types and set function signatures for JNI_OnLoad, Java_* functions.
Support importing Frida RegisterNatives JSON for dynamic JNI functions.
"""

import json
import os
import re
import shutil
import threading
from pathlib import Path
from typing import Dict, List, Optional, Tuple

from binaryninja import PluginCommand, log_info, log_warn
from binaryninja.binaryview import BinaryView, BinaryViewType
from binaryninja.interaction import get_text_line_input
from binaryninja.typelibrary import TypeLibrary

# 支持的架构映射
ARCH_MAP: Dict[str, str] = {
    "aarch64": "jni_aarch64.bntl",
    "armv7": "jni_armv7.bntl",
    "thumb2": "jni_armv7.bntl",
}

# JNI 类型映射表
JNI_TYPE_MAP: Dict[str, Tuple[str, str]] = {
    'Z': ('jboolean', 'bool'),
    'B': ('jbyte', 'byte'),
    'C': ('jchar', 'char'),
    'S': ('jshort', 'short'),
    'I': ('jint', 'int'),
    'J': ('jlong', 'long'),
    'F': ('jfloat', 'float'),
    'D': ('jdouble', 'double'),
    'V': ('void', 'void'),
}


def inject_jni_base_types(bv: BinaryView) -> bool:
    """
    手动注入 JNI 基础类型定义(fallback 方案)
    从 typelibs/jni_fallback.h 加载完整的 JNI 类型定义
    
    Returns:
        True if successful, False otherwise
    """
    # 加载类型定义文件
    header_path = Path(__file__).parent / "typelibs" / "jni_fallback.h"
    if not header_path.exists():
        log_warn(f"[JNI Helper] Fallback header not found: {header_path}")
        return False
    
    try:
        with open(header_path, 'r', encoding='utf-8') as f:
            type_definitions = f.read()
        
        # 解析并注入类型定义
        result = bv.parse_types_from_string(type_definitions)
        if not result.types:
            log_warn("[JNI Helper] Failed to parse fallback type definitions")
            return False
        
        count = 0
        for name, type_obj in result.types.items():
            if not bv.get_type_by_name(name):
                bv.define_user_type(name, type_obj)
                count += 1
        
        log_info(f"[JNI Helper] Fallback: Injected {count} JNI types (with full JNINativeInterface)")
        return True
        
    except Exception as e:
        log_warn(f"[JNI Helper] Fallback injection failed: {e}")
        return False


def get_typelib_dir() -> Path:
    """获取 Binary Ninja 类型库目录路径。"""
    return Path(os.environ.get("APPDATA", Path.home())) / "Binary Ninja" / "typelibs"


def install_typelibs() -> None:
    """安装 JNI 类型库到 Binary Ninja 类型库目录。"""
    plugin_dir = Path(__file__).parent / "typelibs"

    for arch, src_name in ARCH_MAP.items():
        src = plugin_dir / src_name
        if not src.exists():
            continue

        dst_dir = get_typelib_dir() / arch
        dst_dir.mkdir(parents=True, exist_ok=True)
        dst = dst_dir / "jni.bntl"

        if not dst.exists():
            shutil.copy(src, dst)
            log_info(f"[JNI Helper] Installed {arch}/jni.bntl")


def early_setup(bv: BinaryView) -> None:
    """
    早期阶段处理：加载类型库 + 设置固定签名函数 + 启动轮询线程。

    Args:
        bv: Binary Ninja BinaryView 对象
    """
    if not bv.arch or bv.arch.name not in ARCH_MAP:
        return

    log_info("[JNI Helper] Phase 1: Loading JNI types...")
    log_info(f"[JNI Helper] Architecture: {bv.arch.name}")

    # 加载类型库
    typelib_path = get_typelib_dir() / bv.arch.name / "jni.bntl"
    log_info(f"[JNI Helper] Type library path: {typelib_path}")
    log_info(f"[JNI Helper] Type library exists: {typelib_path.exists()}")
    
    types_ready = False
    
    if typelib_path.exists():
        try:
            tl = TypeLibrary.load_from_file(str(typelib_path))
            if tl and tl not in bv.type_libraries:
                bv.add_type_library(tl)
                log_info(f"[JNI Helper] Loaded JNI type library for {bv.arch.name}")
                
                # 验证关键类型是否可用 - 检查 JNINativeInterface 结构
                if bv.get_type_by_name('JNINativeInterface'):
                    types_ready = True
                    log_info("[JNI Helper] Type library verified (with JNINativeInterface)")
                elif bv.get_type_by_name('jobject'):
                    # 类型库加载了，但没有 JNINativeInterface，需要 fallback
                    log_warn("[JNI Helper] Type library incomplete (missing JNINativeInterface)")
                    types_ready = False
                else:
                    log_warn("[JNI Helper] Type library loaded but types not available")
                    types_ready = False
        except Exception as e:
            log_warn(f"[JNI Helper] Type library load failed: {e}")
    else:
        log_info("[JNI Helper] Type library not found, will use fallback")
    
    # Fallback: 手动注入基础类型
    if not types_ready:
        log_info("[JNI Helper] Using fallback type injection...")
        types_ready = inject_jni_base_types(bv)
    
    if not types_ready:
        log_warn("[JNI Helper] Failed to load JNI types, plugin may not work correctly")
        return

    # 设置固定签名的函数
    fixed_sigs: Dict[str, str] = {
        "JNI_OnLoad": "jint JNI_OnLoad(JavaVM* vm, void* reserved)",
        "JNI_OnUnload": "void JNI_OnUnload(JavaVM* vm, void* reserved)",
    }

    count = 0
    for func in bv.functions:
        if func.name in fixed_sigs:
            sig = fixed_sigs[func.name]
            try:
                parsed = bv.parse_type_string(sig)
                if parsed:
                    func.set_user_type(parsed[0])
                    log_info(f"[JNI Helper] (early) {func.name}")
                    count += 1
                else:
                    log_warn(f"[JNI Helper] Failed to parse: {sig}")
            except Exception as e:
                log_warn(f"[JNI Helper] Failed: {func.name}: {e}")

    if count > 0:
        log_info(f"[JNI Helper] Phase 1 done: {count} fixed functions")

    # 启动轮询线程
    threading.Thread(target=poll_java_functions, args=(bv,), daemon=True).start()


def poll_java_functions(bv: BinaryView) -> None:
    """
    轮询检查并设置已准备好的 Java_* 函数签名。

    持续轮询最多 60 秒，每 0.5 秒检查一次。
    当函数参数数量 >= 2 时认为已准备好，立即设置签名。

    Args:
        bv: Binary Ninja BinaryView 对象
    """
    import time

    processed: set = set()
    max_wait = 60

    for _ in range(max_wait * 2):
        time.sleep(0.5)

        for func in bv.functions:
            name = func.name
            if not name.startswith("Java_") or name in processed:
                continue

            # 参数 >= 2 才算准备好
            if len(func.parameter_vars) < 2:
                continue

            try:
                params = ["JNIEnv* env", "jobject thiz"]
                for i in range(2, len(func.parameter_vars)):
                    params.append(f"int64_t arg{i - 1}")
                sig = f"jobject {name}({', '.join(params)})"

                parsed = bv.parse_type_string(sig)
                if parsed:
                    func.set_user_type(parsed[0])
                    log_info(f"[JNI Helper] (poll) {name} ({len(func.parameter_vars)} params)")
                    processed.add(name)
                else:
                    log_warn(f"[JNI Helper] Failed to parse: {sig}")
            except Exception as e:
                log_warn(f"[JNI Helper] Failed: {name}: {e}")

        # 检查是否所有 Java_* 都处理完了
        all_java = [f.name for f in bv.functions if f.name.startswith("Java_")]
        if all_java and all(n in processed for n in all_java):
            log_info(f"[JNI Helper] Poll done: all {len(processed)} Java_* functions set")
            bv.update_analysis()
            return

    if processed:
        log_info(f"[JNI Helper] Poll timeout: {len(processed)} functions set")


def set_java_functions(bv: BinaryView) -> None:
    """
    设置所有 Java_* 函数签名。

    Args:
        bv: Binary Ninja BinaryView 对象
    """
    count = 0
    skipped = 0

    for func in bv.functions:
        name = func.name
        if not name.startswith("Java_"):
            continue

        if len(func.parameter_vars) < 2:
            skipped += 1
            continue

        try:
            params = ["JNIEnv* env", "jobject thiz"]
            for i in range(2, len(func.parameter_vars)):
                params.append(f"int64_t arg{i - 1}")
            sig = f"jobject {name}({', '.join(params)})"

            parsed = bv.parse_type_string(sig)
            if parsed:
                func.set_user_type(parsed[0])
                log_info(f"[JNI Helper] {name} ({len(func.parameter_vars)} params)")
                count += 1
            else:
                log_warn(f"[JNI Helper] Failed to parse: {sig}")
        except Exception as e:
            log_warn(f"[JNI Helper] Failed: {name}: {e}")

    if count > 0:
        msg = f"[JNI Helper] Set {count} Java_* functions"
        if skipped:
            msg += f", {skipped} skipped (not ready)"
        log_warn(msg)
        bv.update_analysis()


def late_setup(bv: BinaryView) -> None:
    """
    后期阶段处理：分析完成后最终确认一次。

    Args:
        bv: Binary Ninja BinaryView 对象
    """
    if not bv.arch or bv.arch.name not in ARCH_MAP:
        return

    log_info("[JNI Helper] Final pass: Checking Java_* signatures...")
    set_java_functions(bv)


def parse_jni_type(s: str, idx: int = 0) -> Tuple[Optional[str], Optional[str], int]:
    """
    解析单个 JNI 类型。

    Args:
        s: JNI 签名字符串
        idx: 当前解析位置

    Returns:
        (类型名, 简短名, 消耗的字符数)
    """
    if idx >= len(s):
        return None, None, 0

    c = s[idx]
    if c in JNI_TYPE_MAP:
        return JNI_TYPE_MAP[c][0], JNI_TYPE_MAP[c][1], 1
    elif c == 'L':
        # 对象类型 Lcom/example/Class;
        end = s.index(';', idx)
        full_name = s[idx + 1:end]
        short_name = full_name.split('/')[-1]
        return 'jobject', short_name.lower(), end - idx + 1
    elif c == '[':
        # 数组类型
        inner_type, inner_name, consumed = parse_jni_type(s, idx + 1)
        array_types = ['jint', 'jbyte', 'jchar', 'jshort', 'jlong', 'jfloat', 'jdouble', 'jboolean']
        if inner_type in array_types:
            return f'{inner_type}Array', f'{inner_name}_arr', consumed + 1
        return 'jobjectArray', f'{inner_name}_arr', consumed + 1

    return 'jobject', 'obj', 1


def parse_jni_signature(sig: str) -> Tuple[str, List[Tuple[str, str]]]:
    """
    解析 JNI 方法签名。

    Args:
        sig: JNI 签名字符串，格式为 (参数...)返回类型

    Returns:
        (返回类型, [(参数类型, 参数名), ...])

    Examples:
        >>> parse_jni_signature("(Landroid/content/Context;II)V")
        ('void', [('jobject', 'arg1_context'), ('jint', 'arg2_int'), ('jint', 'arg3_int')])
    """
    match = re.match(r'\(([^)]*)\)(.+)', sig)
    if not match:
        return 'jobject', []

    params_str, ret_str = match.groups()

    # 解析参数
    params: List[Tuple[str, str]] = []
    idx = 0
    arg_idx = 1

    while idx < len(params_str):
        ptype, pname, consumed = parse_jni_type(params_str, idx)
        if ptype:
            params.append((ptype, f'arg{arg_idx}_{pname}'))
            arg_idx += 1
        idx += consumed

    # 解析返回类型
    ret_type, _, _ = parse_jni_type(ret_str)

    return ret_type or 'jobject', params


def import_frida_json(bv: BinaryView, json_text: str) -> int:
    """
    导入 Frida RegisterNatives JSON，命名动态注册的 JNI 函数。

    支持两种格式：
    1. JSON 数组: [{"class_name": ..., "name&signature": ..., "func_offest": ...}, ...]
    2. 多行 JSON: 每行一个 JSON 对象

    Args:
        bv: Binary Ninja BinaryView 对象
        json_text: Frida 输出的 JSON 文本

    Returns:
        成功导入的函数数量
    """
    try:
        entries: List[dict] = []
        json_text = json_text.strip()

        if json_text.startswith('['):
            entries = json.loads(json_text)
        else:
            for line in json_text.split('\n'):
                line = line.strip()
                if line.startswith('{'):
                    entries.append(json.loads(line))

        count = 0
        for entry in entries:
            class_name = entry.get('class_name', '')
            name_sig = entry.get('name&signature', '')
            offset = entry.get('func_offest', '')

            # 解析方法名和签名
            match = re.match(r'(\w+)(\(.+)', name_sig)
            if not match:
                log_info(f"[JNI Helper] Skip invalid: {name_sig}")
                continue

            method_name, signature = match.groups()

            # 解析偏移地址
            if isinstance(offset, str):
                offset = int(offset, 16) if offset.startswith('0x') else int(offset)

            # 查找函数
            func = bv.get_function_at(bv.start + offset)
            if not func:
                log_info(f"[JNI Helper] Function not found at {hex(offset)}")
                continue

            # 生成函数名: dyn_原函数名_Java_com_example_Class_methodName
            safe_class = class_name.replace('.', '_')
            old_name = func.name
            new_name = f"dyn_{old_name}_Java_{safe_class}_{method_name}"

            # 解析签名生成参数
            ret_type, sig_params = parse_jni_signature(signature)
            params = ["JNIEnv* env", "jobject thiz"]
            for ptype, pname in sig_params:
                params.append(f"{ptype} {pname}")

            func_sig = f"{ret_type} {new_name}({', '.join(params)})"

            try:
                func.name = new_name
                parsed = bv.parse_type_string(func_sig)
                if parsed:
                    func.set_user_type(parsed[0])
                    log_info(f"[JNI Helper] {new_name} @ {hex(offset)}")
                    count += 1
            except Exception as e:
                log_info(f"[JNI Helper] Failed: {new_name}: {e}")

        if count > 0:
            log_warn(f"[JNI Helper] Imported {count} dynamic JNI functions")
            bv.update_analysis()

        return count
    except Exception as e:
        log_warn(f"[JNI Helper] Import failed: {e}")
        return 0


def cmd_import_frida_json(bv: BinaryView) -> None:
    """菜单命令：导入 Frida RegisterNatives JSON。"""
    json_text = get_text_line_input("Paste Frida RegisterNatives JSON:", "Import JNI")
    if json_text:
        import_frida_json(bv, json_text)


# ============== 插件初始化 ==============

# 安装类型库
install_typelibs()

# 注册事件处理
BinaryViewType.add_binaryview_finalized_event(early_setup)
BinaryViewType.add_binaryview_initial_analysis_completion_event(late_setup)

# 注册菜单命令
PluginCommand.register(
    "JNI Helper\\Import Frida RegisterNatives JSON",
    "Import dynamic JNI functions from Frida hook output",
    cmd_import_frida_json
)

log_info("[JNI Helper] Ready")
