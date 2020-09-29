#ifndef HGET
#define HGET
#pragma comment (lib,"winhttp.lib")

#include <Windows.h>
#include <assert.h>
#include <tchar.h>
#include <winhttp.h>
#include <string>
#include <nlohmann/json.hpp>
#include<iostream>
#include <vector>
using json = nlohmann::json;

inline json getHttp() {
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET hSession = NULL,hConnect = NULL, hRequest = NULL;
    URL_COMPONENTS urlComponents = { 0 };
    WCHAR szHostName[256], szUrlPath[2048];
    WCHAR url[] = L"https://api.warframestat.us/pc/sortie";
    std::vector<std::string> ret;
    // セッションハンドルを取得

    hSession = WinHttpOpen(
        L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    assert(hSession);

    urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
    urlComponents.lpszHostName = szHostName;
    urlComponents.dwHostNameLength = sizeof(szHostName) / sizeof(WCHAR);
    urlComponents.lpszUrlPath = szUrlPath;
    urlComponents.dwUrlPathLength = sizeof(szUrlPath) / sizeof(WCHAR);
    // HTTP サーバの指定
    WinHttpCrackUrl(url, wcslen(url), 0, &urlComponents);
    hConnect = WinHttpConnect(hSession, szHostName, urlComponents.nPort, 0);

    assert(hConnect);

    // HTTP リクエストハンドルを作成

    hRequest = WinHttpOpenRequest(hConnect, L"GET", szUrlPath,
        NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
        (INTERNET_SCHEME_HTTPS == urlComponents.nScheme) ? WINHTTP_FLAG_SECURE : 0);

    assert(hRequest);

    // Send a request.
    bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0,
        WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0);

    assert(bResults);

    // リクエスト終了

    bResults = WinHttpReceiveResponse(hRequest, NULL);

    assert(bResults);

    do {

        // 利用可能なデータがあるかチェックする

        dwSize = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            break;
        }

        // バッファの割り当て

        pszOutBuffer = new char[dwSize + 1];

        if (!pszOutBuffer) {
            printf("Out of memory\n");
            dwSize = 0;
        }
        else {
            // Read the data.
            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
                std::cout<<"Error %u in WinHttpReadData.\n"<< GetLastError();
            else {
                ret.push_back(std::string(pszOutBuffer));
            }
            delete[] pszOutBuffer;

        }
    } while (dwSize > 0);

    //
    // クリーンアップ
    //

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    std::string raw;
    for (auto s : ret)
        raw+= s;
    return  json::parse(raw);
}


#endif // !HGET
