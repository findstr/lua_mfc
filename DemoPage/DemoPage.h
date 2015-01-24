#pragma once

#include "resource.h"

extern "C" {

#include "../lua5.3/lua/lua.h"
#include "../lua5.3/lua/lauxlib.h"
#include "../lua5.3/lua/lualib.h"
}

#include <vector>

#define PARENT  CDialogEx

// CDemoPage dialog
#ifdef ICSETTING_EXPORT
#define ICSETTING_API    __declspec(dllexport)
#else
#define ICSETTING_API    __declspec(dllimport)
#endif

class ICSETTING_API CDemoPage : public PARENT
{
	DECLARE_DYNAMIC(CDemoPage)

public:
	CDemoPage();
	virtual ~CDemoPage();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };
private:
        void PushFuncsToLua();
        int  FindCtrl(int i);
        void SetComboText(CComboBox *cb, const CString &txt);
        void GetComboText(CComboBox *cb, CString &txt);

        static int MsgBox(lua_State *L);

        static int CreateStatic(lua_State *L);
        static int CreateComboBox(lua_State *L);

        static int AddString(lua_State *L);
        static int GetString(lua_State *L);

        static int OnMsg(lua_State *L, int id, int msg);
private:
        enum ctrl_type {
                CTRL_STATIC,
                CTRL_COMBOBOX,
        };

        struct ctrl_item {
                int                     id;
                enum ctrl_type          type;
                union {
                        CStatic         *s;
                        CComboBox       *cb;
                } inst;
        };

        static CDemoPage                *My;
        lua_State                       *L;
        DWORD                           ctrl_id;
        std::vector<struct ctrl_item>   ctrl_list;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
        virtual void OnOK();
        virtual BOOL OnInitDialog();
        virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
