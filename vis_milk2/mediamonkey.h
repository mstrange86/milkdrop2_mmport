// MediaMonkey compatibility hacks
// removes Wasabi and numerical locale dependencies
//
// Matt Strange 10/2014

#pragma once

#ifdef MEDIAMONKEY

#include <crtdbg.h>
#include <tuple>
#include "api.h"
#include "index_tuple.h"

//
// use index_tuple index metaprogramming to upwrap winamp's void* array into the function's expected return/parameter types
//

template<class ... Args, unsigned... Indices>
inline std::tuple<Args...> build_tuple(void**params, redi::index_tuple< Indices... >)
{
    std::tuple<Args*...> ptr_tuple(reinterpret_cast<Args*...>(params[Indices])...);
    std::tuple<Args...> value_tuple((*(std::get<Indices>(ptr_tuple)))...);

    return value_tuple;
}

template<class Ret, class... Args, unsigned... Indices >
inline Ret execute_with_tuple(Ret(*pf)(Args...), redi::index_tuple< Indices... >, std::tuple<Args...>&& tup)
{
    return pf(std::forward<Args>(std::get<Indices>(tup))...);
}

template<class Ret, class ...Args>
inline bool unpack_params(Ret(*pf)(Args...), void *retval, int nparam, void** params)
{
    if (sizeof...(Args) == nparam)
    {
        Ret*rv = reinterpret_cast<Ret*>(retval);
        redi::make_index_tuple<sizeof...(Args)>::type size;
        std::tuple<Args...> tup = build_tuple<Args...>(params, size);

        (*rv) = execute_with_tuple(pf, size, std::tuple<Args...>(tup));

        return true;
    }
    return false;
}

class waServiceFactory_mm : public waServiceFactory
{
public:
    waServiceFactory_mm(void *data) : m_data(data) {}
private:
    virtual int _dispatch(int msg, void *retval, void **params=0, int nparam=0)
    {
        switch(msg)
        {
        case WASERVICEFACTORY_GETINTERFACE:
            {
                // param is the lock, we have no concern for locking as we are returning a static variable
                // the winamp lock is poorly documented so this could well be incorrect

                void** rv = reinterpret_cast<void**>(retval);
                (*rv) = m_data;
                return 1;
            }
            break;
        default:
            break;
        }

        // should not occur
        _ASSERT(0);
        return 0;
    }

    void *m_data;
};

class api_language_mm : public api_language
{
public:
    api_language_mm() {}
protected:
    virtual int _dispatch(int msg, void *retval, void **params = 0, int nparam = 0)
    {
        switch (msg)
        {
        case API_LANGUAGE_GET_C_NUMERICLOCALE:
        {
            if (unpack_params(GET_C_NUMERICLOCALE, retval, nparam, params))
                return 1;
            break;
        }
        case API_LANGUAGE_GETSTRING:
        {
            if (unpack_params(GETSTRING, retval, nparam, params))
                return 1;
            break;
        }
        case API_LANGUAGE_GETSTRINGW:
        {
            if (unpack_params(GETSTRINGW, retval, nparam, params))
                return 1;
            break;
        }
        case API_LANGUAGE_LOADLMENU:
        {
            if (unpack_params(LOADLMENU, retval, nparam, params))
                return 1;
            break;
        }
        case API_LANGUAGE_CREATELDIALOGPARAMW:
        {
            if (unpack_params(CREATELDIALOGPARAMW, retval, nparam, params))
                return 1;
            break;
        }
        case API_LANGUAGE_LDIALOGBOXPARAMW:
        {
            if (unpack_params(LDIALOGBOXPARAMW, retval, nparam, params))
                return 1;
            break;
        }
        default:
            break;
        }

        // should not occur
        _ASSERT(0);
        return 0;
    }

    static _locale_t GET_C_NUMERICLOCALE()
    {
        return _create_locale(LC_NUMERIC, "C");
    }
    
    static char* GETSTRING(HINSTANCE hinst, HINSTANCE owner, UINT uID, char* str, size_t maxlen)
    {
        if (0 == maxlen)
        {
            // return pointer to const copy
            LoadString(hinst, uID, (LPSTR)&str, maxlen);
        }
        else
        {
            // store copy in str
            LoadString(hinst, uID, str, maxlen);
        }

        return str;
    }

    static wchar_t* GETSTRINGW(HINSTANCE hinst, HINSTANCE owner, UINT uID, wchar_t *str, size_t maxlen)
    {
        if (0 == maxlen)
        {
            // return pointer to const copy
            LoadStringW(hinst, uID, (LPWSTR)&str, maxlen);
        }
        else
        {
            // store copy in str
            LoadStringW(hinst, uID, str, maxlen);
        }

        return str;
    }

    static HMENU LOADLMENU(HINSTANCE localised, HINSTANCE original, UINT id)
    {
        return LoadMenu(localised, MAKEINTRESOURCE(id));
    }

    static HWND CREATELDIALOGPARAMW(HINSTANCE localised, HINSTANCE original, UINT id, HWND parent, DLGPROC proc, LPARAM param)
    {
        return CreateDialogParamW(localised, MAKEINTRESOURCEW(id), parent, proc, param);
    }

    static INT_PTR LDIALOGBOXPARAMW(HINSTANCE localised, HINSTANCE original, UINT id, HWND parent, DLGPROC proc, LPARAM param)
    {
        return DialogBoxParamW(localised, MAKEINTRESOURCEW(id), parent, proc, param);
    }
};

class api_application_mm : public api_application
{
public:
    api_application_mm() {}
protected:
    virtual int _dispatch(int msg, void *retval, void **params=0, int nparam=0)
    {
        switch(msg)
        {
        case API_APPLICATION_DIRECTMOUSEWHEEL_PROCESSDIALOGMESSAGE:
            {
                // we can't handle this, so just ignore it silently
                return 0;
            }
            break;
        }

        // should not occur
        _ASSERT(0);
        return 0;
    }
private:
};

class api_syscb_mm : public api_syscb
{
public:
    api_syscb_mm() {}
protected:
    virtual int _dispatch(int msg, void *retval, void **params=0, int nparam=0)
    {
        switch(msg)
        {
        case API_SYSCB_SYSCB_ISSUECALLBACK:
            {
                // we can't handle this, so just ignore it silently
                return 0;
            }
            break;
        default:
            break;
        }
            
        // should not occur
        _ASSERT(0);
        return 0;
    }
private:
};


extern waServiceFactory_mm WASABI_API_LNG_FACTORY_MM;
extern waServiceFactory_mm WASABI_API_APP_FACTORY_MM;
extern waServiceFactory_mm WASABI_API_SYSCB_FACTORY_MM;

class api_service_mm : public api_service
{
public:
    api_service_mm()
    {

    }
private:
    virtual int _dispatch(int msg, void *retval, void **params=0, int nparam=0)
    {
        switch(msg)
        {
        case API_SERVICE_SERVICE_GETSERVICEBYGUID:
            if (unpack_params(GETSERVICEBYGUID, retval, nparam, params))
                return 1;
            break;
        default:
            break;
        }

        // should not occur
        _ASSERT(0);
        return 0;
    }

    static waServiceFactory* GETSERVICEBYGUID(GUID guid)
    {
        waServiceFactory* rv = 0;
        if (guid == languageApiGUID)
        {
            rv = &WASABI_API_LNG_FACTORY_MM;
        }
        else if (guid == applicationApiServiceGuid)
        {
            rv = &WASABI_API_APP_FACTORY_MM;
        }
        else if (guid == syscbApiServiceGuid)
        {
            rv = &WASABI_API_SYSCB_FACTORY_MM;
        }
        return rv;
    }

};

#endif