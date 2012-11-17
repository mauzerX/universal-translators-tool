/***************************************************************
 * Name:      common.cpp
 * Purpose:   implementation of common functions for Lua
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-02-28
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/

#include "pch.h"
#include "luacontrol.h"
#include "common.h"
#include "gui/selmoduleimpl.h"


int reboot( lua_State* )
{
	Lua::SetRebootFlag();
	return 0;
}



int writeToStdCout(lua_State *L)
{
	int n = lua_gettop(L);
	for (int i = 1; i <= n; ++i)
	{
		std::cout << lua_tostring(L, i);
	}
	return 0;
}



int writeToLog(lua_State *L)
{
	int n = lua_gettop(L);
	for (int i = 1; i <= n; ++i)
	{
		wxLogMessage( lua_tostring(L, i) );
	}
	return 0;
}



int parseFileName(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 1)
	{
		wxLogMessage("parseFileName: function need a string as argument");
		return 0;
	}
	wxString src( lua_tostring(L, 1) );
	wxString volume, path, name, ext;
	wxFileName::SplitPath( src, &volume, &path, &name, &ext );
	volume += wxFileName::GetVolumeSeparator( wxPATH_NATIVE );
	lua_pushstring(L, volume.c_str().AsChar());
	lua_pushstring(L, path.c_str().AsChar());
	lua_pushstring(L, name.c_str().AsChar());
	lua_pushstring(L, ext.c_str().AsChar());
	return 4;
}



int selectSomething(lua_State *L, const wxString& title = wxEmptyString)
{
	int n = lua_gettop(L);

	if (n != 1)
	{
		wxLogError("selectDialog: function need a table with module names as argument");
		return 0;
	}

	OOLUA::Lua_table modules;
	OOLUA::pull2cpp(L, modules);
	wxArrayString strings;

	{
		int count = 1;
		std::string value;
		while (modules.safe_at( count++, value ))
		{
			strings.Add( wxString(value) );
		}
	}

	if (strings.size() == 0)
	{
		wxLogError("selectDialog: empty table received");
		return 0;
	}

	SelectModuleImpl dlg(NULL);
	
	if (!title.IsEmpty())
	{
		dlg.SetTitle( title );
	}

	dlg.FillCombo( strings );

	wxString result;
	if (dlg.ShowModal() == wxID_OK)
	{
		result = dlg.GetValue();
	}

	lua_pushstring(L, result.c_str().AsChar());
	return 1;
}



int selectModuleDialog(lua_State *L)
{
	return selectSomething( L );
}



int selectVersionDialog(lua_State *L)
{
	return selectSomething( L, "Select game version" );
}



int setModuleReady(lua_State* L)
{
	L; // to remove warning of unused variable
	ModuleChangedEvent event;
	wxTheApp->QueueEvent( event.Clone() );	
	return 0;
}




#define GET_BUFNINDEX(x)	\
	if (lua_gettop(L) != 2)	{ \
		wxLogError(#x": function need a string and number of a value in array"); \
		return 0;	} \
	const char* buf = lua_tostring(L, 1); \
	int i = lua_tointeger(L, 2); \
	lua_pop(L, 2); \
	if (!buf) { return 0; }




int getStrInt(lua_State *L)
{
	//if (lua_gettop(L) != 2)
	//{
	//	wxLogError("getStrInt: function need a string and number of a value in array");
	//	return 0;
	//}
	//const char* buf = lua_tostring(L, 1);
	//int i = lua_tointeger(L, 2);
	//lua_pop(L, 2);
	//if (!buf)
	//{
	//	return 0;
	//}
	GET_BUFNINDEX(getStrInt)
	lua_pushnumber(L, buf[i]);
	return 1;
}



int getStrChar(lua_State *L)
{
	GET_BUFNINDEX(getStrChar)
	/*if (lua_gettop(L) != 2)
	{
		wxLogError("getStrChar: function need a string and number of a value in array");
		return 0;
	}
	const char* buf = lua_tostring(L, 1);
	int i = lua_tointeger(L, 2);
	lua_pop(L, 2);
	if (!buf)
	{
		return 0;
	}*/
	lua_pushlstring(L, &buf[i], 1);
	return 1;
}

#undef GET_BUFNINDEX



namespace Lua
{

void CommonRegister()
{
    // sets flag to create new Lua VM
	LUA_REG_C_FUNCTION( reboot );

	// writes string to std::cout
	LUA_REG_C_FUNCTION( writeToStdCout );

	// writes string to wxLogMessage
	LUA_REG_C_FUNCTION( writeToLog );

	// parses full filename and returns it's volume, path, name and extension to Lua VM
	LUA_REG_C_FUNCTION( parseFileName );

	// calls dialog to select active plugin
	LUA_REG_C_FUNCTION( selectModuleDialog );

	// calls dialog to select game version
	LUA_REG_C_FUNCTION( selectVersionDialog );

	// creates event about changed module
	LUA_REG_C_FUNCTION( setModuleReady );

	// returns buffer value at position, "getString(bytes, 3)" for example returns fourth value
	// this function IS NOT SAFE
	LUA_REG_C_FUNCTION( getStrInt );
	LUA_REG_C_FUNCTION( getStrChar );

	// export for wxBusyCursor class, will be usefull for continious operations
	Get().register_class<BusyCursor>();
}

} // namespace Lua



EXPORT_OOLUA_NO_FUNCTIONS( BusyCursor )