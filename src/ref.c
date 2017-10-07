/*
 *  Copyright (C) 2017 Masatoshi Teruya
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  ref.c
 *  lua-ref
 *  Created by Masatoshi Teruya on 17/10/07.
 */

#include <lua.h>
#include <lauxlib.h>


static int del_lua( lua_State *L )
{
    lua_Integer ref = luaL_checkinteger( L, 1 );
    int rc = 0;

    // check is need pushref
    if( lua_gettop( L ) > 1 )
    {
        luaL_checktype( L, 2, LUA_TBOOLEAN );
        if( lua_toboolean( L, 2 ) ){
            lua_rawgeti( L, LUA_REGISTRYINDEX, ref );
            rc = 1;
        }
    }

    luaL_unref( L, LUA_REGISTRYINDEX, ref );

    return rc;
}


static int val_lua( lua_State *L )
{
    lua_Integer ref = luaL_checkinteger( L, 1 );

    lua_rawgeti( L, LUA_REGISTRYINDEX, ref );

    return 1;
}


static int get_lua( lua_State *L )
{
    int ref = LUA_REFNIL;

    if( lua_gettop( L ) ){
        ref = luaL_ref( L, LUA_REGISTRYINDEX );
    }
    lua_pushinteger( L, ref );

    return 1;
}


LUALIB_API int luaopen_ref( lua_State *L )
{
    struct luaL_Reg method[] = {
        { "get", get_lua },
        { "val", val_lua },
        { "del", del_lua },
        { NULL, NULL }
    };
    struct luaL_Reg *ptr = method;

    lua_newtable( L );
    while( ptr->name ){
        lua_pushstring( L, ptr->name );
        lua_pushcfunction( L, ptr->func );
        lua_rawset( L, -3 );
        ptr++;
    }

    // add constants
    lua_pushstring( L, "NOREF" );
    lua_pushinteger( L, LUA_NOREF );
    lua_rawset( L, -3 );

    lua_pushstring( L, "REFNIL" );
    lua_pushinteger( L, LUA_REFNIL );
    lua_rawset( L, -3 );

    return 1;
}
