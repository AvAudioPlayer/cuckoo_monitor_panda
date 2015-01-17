/*
Cuckoo Sandbox - Automated Malware Analysis.
Copyright (C) 2010-2015 Cuckoo Foundation.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MONITOR_MISC_H
#define MONITOR_MISC_H

#include <windows.h>
#include "flags.h"
#include "ntapi.h"

void misc_init();

wchar_t *get_unicode_buffer();

uintptr_t pid_from_process_handle(HANDLE process_handle);
uintptr_t pid_from_thread_handle(HANDLE thread_handle);
uintptr_t tid_from_thread_handle(HANDLE thread_handle);
uintptr_t parent_process_id();

uint32_t path_get_full_pathA(const char *in, wchar_t *out);
uint32_t path_get_full_pathW(const wchar_t *in, wchar_t *out);
uint32_t path_get_full_path_handle(HANDLE file_handle, wchar_t *out);
uint32_t path_get_full_path_unistr(const UNICODE_STRING *in, wchar_t *out);
uint32_t path_get_full_path_objattr(
    const OBJECT_ATTRIBUTES *in, wchar_t *out);

void wcsncpyA(wchar_t *str, const char *value, uint32_t length);

void hide_module_from_peb(HMODULE module_handle);
void destroy_pe_header(HANDLE module_handle);

int copy_unicode_string(const UNICODE_STRING *in,
    UNICODE_STRING *out, wchar_t *buffer, uint32_t length);

int copy_object_attributes(const OBJECT_ATTRIBUTES *in,
    OBJECT_ATTRIBUTES *out, UNICODE_STRING *unistr,
    wchar_t *buffer, uint32_t length);

uint32_t reg_get_key(HANDLE key_handle, wchar_t *regkey);
uint32_t reg_get_key_ascii(HANDLE key_handle,
    const char *subkey, uint32_t length, wchar_t *regkey);
uint32_t reg_get_key_asciiz(HANDLE key_handle,
    const char *subkey, wchar_t *regkey);
uint32_t reg_get_key_uni(HANDLE key_handle,
    const wchar_t *subkey, uint32_t length, wchar_t *regkey);
uint32_t reg_get_key_uniz(HANDLE key_handle,
    const wchar_t *subkey, wchar_t *regkey);
uint32_t reg_get_key_unistr(HANDLE key_handle,
    const UNICODE_STRING *unistr, wchar_t *regkey);
uint32_t reg_get_key_objattr(const OBJECT_ATTRIBUTES *obj, wchar_t *regkey);

void reg_get_info_from_keyvalue(const void *buf, uint32_t length,
    KEY_VALUE_INFORMATION_CLASS information_class, wchar_t **reg_name,
    uint32_t *reg_type, uint32_t *data_length, uint8_t **data);

void get_ip_port(const struct sockaddr *addr, const char **ip, int *port);

int is_shutting_down();

void library_from_unicode_string(const UNICODE_STRING *us,
    char *library, int32_t length);

int stacktrace(uint32_t ebp, uint32_t *addrs, uint32_t length);

void setup_exception_handler();
void *memdup(const void *addr, uint32_t length);
int page_is_readable(const uint8_t *addr);
void clsid_to_string(REFCLSID rclsid, wchar_t *buf);

wchar_t *flag_to_string(flag_t which, uint32_t flag);

uint64_t hash_buffer(const void *buf, uint32_t length);
uint64_t hash_string(const char *buf, int32_t length);
uint64_t hash_stringW(const wchar_t *buf, int32_t length);
uint64_t hash_uint64(uint64_t value);

const uint8_t *module_from_address(const uint8_t *addr);
uint32_t module_image_size(const uint8_t *addr);

#define COPY_UNICODE_STRING(local_name, param_name) \
    UNICODE_STRING local_name; \
    wchar_t *local_name##_buffer = get_unicode_buffer(); \
    copy_unicode_string(param_name, &local_name, \
        local_name##_buffer, sizeof(local_name##_buffer));

#define COPY_OBJECT_ATTRIBUTES(local_name, param_name) \
    OBJECT_ATTRIBUTES local_name; UNICODE_STRING local_name##_unistr; \
    wchar_t *local_name##_buffer = get_unicode_buffer(); \
    copy_object_attributes(param_name, &local_name, &local_name##_unistr, \
        local_name##_buffer, sizeof(local_name##_buffer));

#define OBJECT_NAME_INFORMATION_REQUIRED_SIZE \
    sizeof(OBJECT_NAME_INFORMATION) + sizeof(wchar_t) * MAX_PATH_W

#define PAGE_READABLE \
    (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | \
     PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | \
     PAGE_EXECUTE_WRITECOPY)

#if !__x86_64__

static inline uintptr_t get_ebp()
{
    uintptr_t ret;
    __asm__ volatile("movl %%ebp, %0" : "=r" (ret));
    return ret;
}

#endif

#endif
