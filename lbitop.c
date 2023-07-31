/**
 * @file lbitop.c
 * @author Brais Solla González
 * @brief Bitop library for Lua, compatible with the LuaJIT one
 * @version 0.1
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdint.h>

typedef union {
   lua_Number n;
   uint64_t   u;
} BitOpN_t;

// Convert argument to bit type.
// Taken from the bitOp library
static uint32_t barg(lua_State *L, int idx){
  BitOpN_t bn;
  uint32_t b;

  bn.n = luaL_checknumber(L, idx);
  bn.n += 6755399441055744.0;  /* 2^52+2^51 */
   
  b = (uint32_t) bn.u;
  return b;
}

/* Return bit type. */
#define BRET(b)  lua_pushnumber(L, (lua_Number)(SBits)(b)); return 1;

static int bit_tobit(lua_State *L){ 
  lua_pushnumber(L, (lua_Number) ((int32_t) barg(L, 1)));
  return 1;
}

static int bit_bnot(lua_State *L) { 
  lua_pushnumber(L, (lua_Number) ((int32_t) ~barg(L, 1)));
  return 1;
}

static int bit_bor(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((int32_t) a | (int32_t) b));
  return 1;
}

static int bit_band(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((int32_t) a & (int32_t) b));
  return 1;
}

static int bit_bxor(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((int32_t) a ^ (int32_t) b));
  return 1;
}

static int bit_lshift(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((uint32_t) a << (uint32_t) b));
  return 1;
}

static int bit_rshift(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((uint32_t) a >> (uint32_t) b));
  return 1;
}

static int bit_arshift(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) ((int32_t) a >> (uint32_t) b));
  return 1;
}


static int bit_rol(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) (((a << b) | (a >> (32-b)))));
  return 1;
}

static int bit_ror(lua_State* L){
  uint32_t a = barg(L, 1);
  uint32_t b = barg(L, 2);

  lua_pushnumber(L, (lua_Number) (((a << (32-b)) | (a >> b))));
  return 1;
}


static int bit_bswap(lua_State *L){
  uint32_t b = barg(L, 1);
  lua_pushnumber(L, (lua_Number) __builtin_bswap32(b));
  return 1;
}

// Taken from bitop
static int bit_tohex(lua_State *L){
  uint32_t b = barg(L, 1);
  int32_t  n = lua_isnone(L, 2) ? 8 : (int32_t) barg(L, 2);
  const char *hexdigits = "0123456789abcdef";
  char buf[8];
  int i;
  if (n < 0) { n = -n; hexdigits = "0123456789ABCDEF"; }
  if (n > 8) n = 8;
  for (i = (int)n; --i >= 0; ) { buf[i] = hexdigits[b & 15]; b >>= 4; }
  lua_pushlstring(L, buf, (size_t)n);
  return 1;
}

static const luaL_Reg bitops[] = {
  { "tobit",	bit_tobit },
  { "bnot",	bit_bnot },
  { "band",	bit_band },
  { "bor",	bit_bor },
  { "bxor",	bit_bxor },
  { "lshift",	bit_lshift },
  { "rshift",	bit_rshift },
  { "arshift",	bit_arshift },
  { "rol",	bit_rol },
  { "ror",	bit_ror },
  { "bswap",	bit_bswap },
  { "tohex",	bit_tohex },
  { NULL, NULL }
};


LUALIB_API int luaopen_bitop(lua_State *L){
  luaL_newlib(L, bitops);
  return 1;
}
