#include "mio/unwindows.h"
#include <windows.h>
#include <memory>
#if defined(UNDER_CE)
LPFN_WSAASYNCSELECT WSAAsyncSelect = NULL;
#endif
namespace mio {
#if defined(UNDER_CE)
    void RegisterMessageClass(HINSTANCE hInstance) {

        static TCHAR szAppName[] = MESSAGE_CLASS;
        WNDCLASS   wndclassex = {0};
        wndclassex.style         = 0;
        wndclassex.lpfnWndProc   = ::DefWindowProc;
        wndclassex.cbClsExtra    = 0;
        wndclassex.cbWndExtra    = 0;
        wndclassex.hInstance     = hInstance;
        wndclassex.hIcon         = NULL;
        wndclassex.hCursor       = NULL;
        wndclassex.hbrBackground = NULL;
        wndclassex.lpszMenuName  = NULL;
        wndclassex.lpszClassName = szAppName;

        if (!RegisterClass (&wndclassex))
        {
            throw "failed";
        }
    }
#endif
    class LoadAsyncSelect {
    public:
        LoadAsyncSelect() {
#if defined(UNDER_CE)
            RegisterMessageClass(NULL);
            _module = LoadLibrary(_T("ws2.dll"));
            WSAAsyncSelect = (LPFN_WSAASYNCSELECT)GetProcAddress(_module, _T("WSAAsyncSelect"));
#endif
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2,2), &wsaData);
        }
        ~LoadAsyncSelect() {
#if defined(UNDER_CE)
            FreeLibrary(_module);
#endif
            WSACleanup();
        }
#if defined(UNDER_CE)
    private:
        HMODULE _module;
#endif
    };
    
    static std::auto_ptr<LoadAsyncSelect> _g;
    void init() {
        if (_g.get() == NULL)
            _g.reset(new LoadAsyncSelect());
    }
}