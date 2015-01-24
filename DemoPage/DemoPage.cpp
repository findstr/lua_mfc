// DemoPage.cpp : implementation file
//

#include "stdafx.h"

#define ICSETTING_EXPORT

#include "DemoPage.h"
#include "afxdialogex.h"


CDemoPage *CDemoPage::My = NULL;
// CDemoPage dialog

IMPLEMENT_DYNAMIC(CDemoPage, PARENT)

//MsgBox(content, caption)
int CDemoPage::MsgBox(lua_State *L)
{
        CString content;
        CString caption;

        content = lua_tostring(L, 1);
        caption = lua_tostring(L, 2);

        My->MessageBox(content, caption);

        return 0;
}

//CreateComboBox(x,y, wdith, heigh, title)
int CDemoPage::CreateStatic(lua_State *L)
{
        CString title;
        struct ctrl_item item;
        CStatic *s = new CStatic;

        int x, y, width, height;

        title = lua_tostring(L, 1);
        x = (int)lua_tonumber(L, 2);
        y = (int)lua_tonumber(L, 3);
        width = (int)lua_tonumber(L, 4);
        height = (int)lua_tonumber(L, 5);

        item.id = My->ctrl_id++;
        item.inst.s = s;
        item.type = CTRL_STATIC;

        s->Create(title, WS_CHILD | WS_VISIBLE,
                CRect(x, y, x + width, y + height), My, item.id);

        My->ctrl_list.push_back(item);

        lua_pushnumber(L, item.id);

        return 1;
}

//CreateComboBox(x,y, wdith, heigh)
int CDemoPage::CreateComboBox(lua_State *L)
{       
        struct ctrl_item item;
        CComboBox *pCb = new CComboBox;

        int x, y, width, height;

        x = (int)lua_tonumber(L, 1);
        y = (int)lua_tonumber(L, 2);
        width = (int)lua_tonumber(L, 3);
        height = (int)lua_tonumber(L, 4);

        item.id = My->ctrl_id++;
        item.inst.cb = pCb;
        item.type = CTRL_COMBOBOX;

        pCb->Create(WS_CHILD | WS_VISIBLE| CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL,
                CRect(x, y, x + width, y + height), My, item.id);

        pCb->SetMinVisibleItems(1000);

        My->ctrl_list.push_back(item);

        lua_pushnumber(L, item.id);

        return 1;
}

void CDemoPage::GetComboText(CComboBox *pCombo, CString &str)
{
        int nSel;

        ASSERT(pCombo);

        nSel = pCombo->GetCurSel();

        if (nSel == -1)
                return ;

        pCombo->GetLBText(nSel, str);
}

void CDemoPage::SetComboText(CComboBox *pCombo, const CString &text)
{
	int i;
	int cnt;
	CString str;

	ASSERT(text);
	ASSERT(pCombo);

	cnt = pCombo->GetCount();

	for (i = 0; i < cnt; i++) {
		pCombo->GetLBText(i, str);
		if (str == text) {
			pCombo->SetCurSel(i);
			break;
		}
	}

        if (i >= cnt) {
                pCombo->AddString(text);
                pCombo->SetCurSel(i);
        }

	return ;
}
//AddString(id, string)
int CDemoPage::AddString(lua_State *L)
{
        int id;
        int index;
        CString str;

#if 0
        str.Format(L"id = %d", id);
        My->MessageBox(str);
#endif

        id = (int)lua_tonumber(L, 1);
        str = lua_tostring(L, 2);

      
        index = My->FindCtrl(id);
 
        switch (My->ctrl_list.at(index).type) {
        case CTRL_STATIC:
                My->ctrl_list.at(index).inst.s->SetWindowText(str);
                break;
        case CTRL_COMBOBOX:
                My->SetComboText(My->ctrl_list.at(index).inst.cb, str);
                break;
        default:
                ASSERT(!"Invalid Control type");
                break;
        }

        return 0;
}

//string = AddString(id)
int CDemoPage::GetString(lua_State *L)
{
        int id;
        int index;
        char *ch;
        CString s;

        id = (int)lua_tonumber(L, 1);

        index = My->FindCtrl(id);

        switch(My->ctrl_list.at(index).type) {
        case CTRL_STATIC:
                My->ctrl_list.at(index).inst.s->GetWindowText(s);
                break;
        case CTRL_COMBOBOX:
                My->GetComboText(My->ctrl_list.at(index).inst.cb, s);
                break;
        default:
                ASSERT(!"Invalid Control type");
                break;
        }

        ch = (char *)alloca(s.GetLength() + 1);
        wcstombs(ch, s.GetBuffer(), s.GetLength() + 1);

        lua_pushstring(L, ch);

        return 1;
}

int CDemoPage::OnMsg(lua_State *L, int id, int msg)
{
        CString err;

        lua_getglobal(L, "OnMsg");
        lua_pushnumber(L, id);
        lua_pushnumber(L, msg);

        if (lua_pcall(L, 2, 0, 0) != 0) {
                err = L"Call OnMsg Err:";
                err += lua_tostring(L, -1);
                My->MessageBox(err);
                return 0;
        }
 
        return 0;
}

int  CDemoPage::FindCtrl(int id)
{
        size_t i;

        for (i = 0; i < ctrl_list.size(); i++) {
                if (ctrl_list.at(i).id == id)
                        break;
        }

        if (i >= ctrl_list.size())
                return -1;

        return i;
}

void CDemoPage::PushFuncsToLua()
{
        //save this pointer
        My = this;
        
        lua_pushcfunction(L, CDemoPage::MsgBox);
        lua_setglobal(L, "MsgBox");

        lua_pushcfunction(L, CDemoPage::CreateStatic);
        lua_setglobal(L, "CreateStatic");

        lua_pushcfunction(L, CDemoPage::CreateComboBox);
        lua_setglobal(L, "CreateComboBox");

        lua_pushcfunction(L, CDemoPage::AddString);
        lua_setglobal(L, "AddString");

        lua_pushcfunction(L, CDemoPage::GetString);
        lua_setglobal(L, "GetString");

}

CDemoPage::CDemoPage()
	: PARENT(CDemoPage::IDD)
{
        ctrl_id = 6000;
}

CDemoPage::~CDemoPage()
{
        size_t i;

        lua_close(L);

        for (i = 0; i < ctrl_list.size(); i++)
                delete ctrl_list.at(i).inst.s;

}

void CDemoPage::DoDataExchange(CDataExchange* pDX)
{
	PARENT::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDemoPage, PARENT)
END_MESSAGE_MAP()


// CDemoPage message handlers


void CDemoPage::OnOK()
{
        // TODO: Add your specialized code here and/or call the base class
        PARENT::OnOK();
}


BOOL CDemoPage::OnInitDialog()
{
        PARENT::OnInitDialog();

        // TODO:  Add extra initialization here
        char *ch;
        CString path;
        CFileDialog dlg(TRUE,
                        NULL, 
                        NULL,
                        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        (LPCTSTR)_TEXT("lua Files (*.lua)|*.lua|All Files (*.*)|*.*||"),
                        NULL);
        if(dlg.DoModal() != IDOK)
                return TRUE;

        path = dlg.GetPathName();

        ch = (char *)alloca(path.GetLength() + 1);
        wcstombs(ch, path, path.GetLength() + 1);
        //load lua vm
        CString str;
        L = luaL_newstate();
        
        luaL_openlibs(L);
 
        PushFuncsToLua();

        if (luaL_loadfile(L, ch) || lua_pcall(L, 0, 0, 0)) {
                str.Format(L"Load lua file error:");
                str += lua_tostring(L, -1);
                MessageBox(str);
        }
        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDemoPage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
        // TODO: Add your specialized code here and/or call the base class
 
        size_t i;

        for (i = 0; i < ctrl_list.size(); i++) {
                if (ctrl_list.at(i).id == nID)
                        break;
        }

        if (i < ctrl_list.size()) {
                if (nCode == CBN_SELCHANGE || nCode == EN_CHANGE)
                        OnMsg(L, nID, nCode);
        }
        

        return PARENT::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
