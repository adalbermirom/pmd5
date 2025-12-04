/* lib pmd5 prisma, Wed Dec  3 22:24:09 2025*/
#ifndef _LIB_PMD5_
#define _LIB_PMD5_  /*============{*/

#include <prisma.h>
#include <lauxlib.h>
#include <prismalib.h>
typedef double number;
typedef const char * string;
typedef struct{
 char m[4];
    int id;
    unsigned int type;
    int ref;
    void *u;
    void *extra;
}userdata;
#define USERDATA_INIT(p) if(p!=NULL){p->m[0]='U'; p->m[1]='D'; p->m[2]='A'; p->m[3]='T'; p->id=0;p->type=0;p->ref=-2;p->u=NULL;p->extra=NULL;}
#define is_udata(ud) is_userdata((ud))
int is_userdata(userdata *ud){
	if(! ud ) return 0;
	
	const char *m = ud->m;
	if ( m[0] == 'U' && m[1] == 'D' && m[2] == 'A' && m[3] == 'T' )
	    return 1;
	    
	return 0;
}
/*Macro auxiliar que transforma o token 'name' em string (#name) e o concatena
com o prefixo. Ex: PRIS_META_NAME(mg_mgr) -> "userdata::mg_mgr"*/
#define PRIS_META_NAME(name) "userdata::"#name

#define NEW_USERDATA(new, name) do{\
    new = PRIS_CREATE_USERDATA(L, PRIS_META_NAME(name), sizeof(userdata));\
    USERDATA_INIT(new);\
}while(0)
            
//helper functions:
//lua.h: LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
const char* get_type(lua_State*L, int index){
    return lua_typename(L, lua_type (L, index) );
}

int luaL_checktable(lua_State *L, int index){
    luaL_checktype (L, index, LUA_TTABLE);
    return index;
}

int luaL_checkfunction(lua_State *L, int index){
    luaL_checktype (L, index, LUA_TFUNCTION);
    return index;
}

int luaL_checknil(lua_State *L, int index){
    luaL_checktype (L, index, LUA_TNIL);
    return index;
}

int luaL_checkthread(lua_State *L, int index){
    luaL_checktype (L, index, LUA_TTHREAD);
    return index;
}
         


#define MODULO  pmd5
#define AUTOR  "Adalberto"
#define VERSAO  "pmd5-1.3"
#define nome_info  "Lib com funções de hash md5 para Prisma-v1.0"




 
 
#include <string.h>
#include <stdlib.h>
#include "WjCryptLib_Md5.h"

#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>

#define MD5_SIZE 18
#define MD5_SIZE_HEX 34




 


static int byte_to_hex(const unsigned char * src_hash, size_t hash_len, char *hex_out, size_t hex_len){
	size_t i;
	if (hex_len < (hash_len * 2 + 1)){
		return -1; /* output hex deve ter ao menos 2 vezes + 1 o tamanho da string fonte. */
	}
	for (i=0; i < hash_len; i++) {
        sprintf(hex_out + 2*i, "%02x",(unsigned char)src_hash[i]);
    }
	return 0; /* 0 is ok */
}
 



static int p_pmd5_init(lua_State*L){
    Md5Context *ctx = (Md5Context*)calloc(1, sizeof(Md5Context));
    if (ctx == NULL){
    lua_pushnil(L);
    lua_pushliteral(L, "Memoria insuficiente ao criar Md5Context");
    return 2;

	}
	
	
	Md5Initialise(ctx);
	
	userdata *new = PRIS_CREATE_USERDATA(L, "userdata::Md5Context", sizeof(userdata));
    USERDATA_INIT(new);
    int ref_new_userdata_Md5Context = lua_gettop(L);

	new->u = ctx;
    lua_pushvalue(L, ref_new_userdata_Md5Context);
    return 1;

}




static int p_Md5Context_update(lua_State*L){
    Md5Context *ctx = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"))->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
    size_t tam_s;
    const char *s = luaL_checklstring(L, 2, &tam_s);
    
    Md5Update(ctx, s, tam_s);
    
    lua_pushvalue(L, 1);
    return 1;

}



static int p_Md5Context___gc(lua_State*L){
    Md5Context *ctx;
    userdata *ud = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"));
    ctx = (Md5Context *)ud->u;
    if(!ctx) return 0;
    
    free(ctx);
    ud->u = NULL;
    return 0;
}


static int p_Md5Context___convstring(lua_State*L){
    userdata *ud = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"));
    if(!ud->u){
    lua_pushliteral(L, "userdata::Md5Context\050liberado\051");
    return 1;

    }
    lua_pushfstring(L, "userdata::Md5Context\050%p\051", ud->u);
    return 1;
}

static int p_Md5Context___tipo(lua_State*L){
    lua_pushstring(L, "userdata::Md5Context");
    return 1;

}

static int p_Md5Context_assert(lua_State*L){
    userdata *ud = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"));
    if(!ud->u){
    lua_pushnil(L);
    lua_pushliteral(L, "userdata::Md5Context invalido ou já liberado.");
    return 2;

    }
    lua_pushfstring(L, "userdata::Md5Context\050%p\051", ud->u);
    return 1;
}




static int p_Md5Context___bshl(lua_State*L){
    Md5Context *ctx = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"))->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
    size_t tam_s;
    const char *s = luaL_checklstring(L, 2, &tam_s);
    Md5Update(ctx, s, tam_s);
    
    lua_pushvalue(L, 1);
    return 1;

}

static int p_Md5Context___call(lua_State*L){
    Md5Context *ctx = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"))->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
    size_t tam_s;
    const char *s = luaL_checklstring(L, 2, &tam_s);
    Md5Update(ctx, s, tam_s);
    lua_pushvalue(L, 1);
    return 1;

}






static int p_Md5Context___add(lua_State*L){
    Md5Context *ctx = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"))->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
    size_t tam_s;
    const char *s = luaL_checklstring(L, 2, &tam_s);
    Md5Update(ctx, s, tam_s);
    lua_pushvalue(L, 1);
    return 1;

}





static int p_Md5Context_final(lua_State*L){
    Md5Context *ctx;
    MD5_HASH digest;
    userdata *ud = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"));
    ctx = (Md5Context *)ud->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
	
	Md5Finalise(ctx, &digest);
    if(ctx){
		free(ctx);
        ud->u = NULL;
    }
    lua_pushlstring(L, digest.bytes, MD5_HASH_SIZE);
    return 1;
}





static int p_Md5Context_final_hex(lua_State*L){
    Md5Context *ctx;
    MD5_HASH digest;
    userdata *ud = ((userdata*)luaL_checkudata(L, 1, "userdata::Md5Context"));
    ctx = (Md5Context *)ud->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushliteral(L, "Md5Context* inválido.");
    return 2;

	}
	Md5Finalise(ctx, &digest);
    if(ctx){
		free(ctx);
        ud->u = NULL;
    }
    

    char hex[2*MD5_HASH_SIZE+1];
    int i;
 
    for (i=0;i<16;i++) {
        sprintf(hex+2*i,"%02x",(unsigned char)digest.bytes[i]);
    }
    lua_pushlstring(L, hex, 2*MD5_HASH_SIZE);
    return 1;
}




static int p_pmd5(lua_State*L){
    size_t tam_s;
    const char *s = luaL_checklstring(L, 1, &tam_s);
    MD5_HASH digest;
    Md5Context ctx; 
    Md5Initialise(&ctx);
    Md5Update(&ctx, s, tam_s);
    Md5Finalise(&ctx, &digest);
    lua_pushlstring (L, digest.bytes, MD5_HASH_SIZE); 
    return 1;
    return 0;
}
 



static int p_pmd5_hex(lua_State*L){
   size_t tam_s;
    const char *s = luaL_checklstring(L, 1, &tam_s);
    MD5_HASH digest;
    Md5Context ctx; 
    Md5Initialise(&ctx);
    Md5Update(&ctx, s, tam_s);
    Md5Finalise(&ctx, &digest);
    

    char hex[2*MD5_HASH_SIZE+1];
    
    int ret = byte_to_hex(digest.bytes, MD5_HASH_SIZE, hex, 2 * MD5_HASH_SIZE + 1);
    if( ret != 0) { /* != 0: error */
		lua_pushnil(L);
		lua_pushliteral(L, "MD5 erro: ao converter para hex.");
		return 2;
	}
    lua_pushlstring(L, hex, 2*MD5_HASH_SIZE);
    return 1;
    return 0;
}










#define BLOCKSIZE 64  
#define HASHLEN 16    


static int hash(const unsigned char *data, size_t len, unsigned char *output){
	if (data == NULL){
		return 1;
	}
	if (output == NULL){
		return 2;
	}
	MD5_HASH digest;
    Md5Context ctx; 
    Md5Initialise(&ctx);
    Md5Update(&ctx, data, len);
    Md5Finalise(&ctx, &digest);
    
    memcpy (output, digest.bytes, HASHLEN);
    return 0;
}





typedef struct {
    Md5Context ctx;               
    unsigned char o_key_pad[BLOCKSIZE]; 
} HmacMd5Context;

void hmac_md5_init(HmacMd5Context *ctx, const unsigned char *key, size_t key_len) {
    unsigned char K[BLOCKSIZE];
    unsigned char i_key_pad[BLOCKSIZE];

    
    memset(K, 0x00, BLOCKSIZE);
    if (key_len > BLOCKSIZE) {
        
        hash(key, key_len, K); 
    } else {
        memcpy(K, key, key_len);
    }

    
    for (int i = 0; i < BLOCKSIZE; i++) {
        i_key_pad[i] = K[i] ^ 0x36;
        ctx->o_key_pad[i] = K[i] ^ 0x5c; 
    }

    
    Md5Initialise(&ctx->ctx);
    Md5Update(&ctx->ctx, i_key_pad, BLOCKSIZE);

    
    memset(K, 0, BLOCKSIZE);
    memset(i_key_pad, 0, BLOCKSIZE);
}

void hmac_md5_update(HmacMd5Context *ctx, const unsigned char *data, size_t len) {
    
    Md5Update(&ctx->ctx, data, len);
}

void hmac_md5_final(HmacMd5Context *ctx, unsigned char *output) {
    unsigned char inner_hash[HASHLEN];
    MD5_HASH digest;

    
    Md5Finalise(&ctx->ctx, &digest);
    memcpy(inner_hash, digest.bytes, HASHLEN);

    
    
    Md5Context outer_ctx;
    Md5Initialise(&outer_ctx);
    Md5Update(&outer_ctx, ctx->o_key_pad, BLOCKSIZE);
    Md5Update(&outer_ctx, inner_hash, HASHLEN);
    
    Md5Finalise(&outer_ctx, &digest);
    memcpy(output, digest.bytes, HASHLEN);

    
    memset(ctx->o_key_pad, 0, BLOCKSIZE);
    memset(inner_hash, 0, HASHLEN);
}


static int md5_hmac(const unsigned char *key, size_t key_len, 
                const unsigned char *msg, size_t msg_len, 
                unsigned char *output) {
    
    HmacMd5Context ctx;

    
    hmac_md5_init(&ctx, key, key_len);
    hmac_md5_update(&ctx, msg, msg_len);
    hmac_md5_final(&ctx, output);

    return 0; 
}


static int p_md5_hmac_init(lua_State*L){
    size_t key_len;
    HmacMd5Context *ctx;
    
    const char *key = luaL_checklstring(L, 1, &key_len);
    ctx = (HmacMd5Context *)calloc(1, sizeof(HmacMd5Context));
    if( ctx == NULL){
		lua_pushnil(L);
		lua_pushliteral(L, "Md5 hmac erro: memoria insuficiente para HmacMd5Context");
		return 2;
	}
    
    hmac_md5_init(ctx, key, key_len);
    
    userdata* new = PRIS_CREATE_USERDATA(L, "userdata::HmacMd5Context", sizeof(userdata));
    USERDATA_INIT(new);
    int ref_new_userdata_HmacMd5Context = lua_gettop(L);

    new->u = ctx;
    
    lua_pushvalue(L, ref_new_userdata_HmacMd5Context);
    return 1;

}



static int p_HmacMd5Context_update(lua_State*L){
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt = luaL_checklstring(L, 2, &tam_txt);
    ctx = (HmacMd5Context*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"))->u;

    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; 
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    lua_pushvalue(L, 1);
    return 1;

}



static int p_HmacMd5Context___add(lua_State*L){
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt = luaL_checklstring(L, 2, &tam_txt);
    ctx = (HmacMd5Context*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"))->u;
    

    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; 
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    lua_pushvalue(L, 1);
    return 1;

}


static int p_HmacMd5Context___bshl(lua_State*L){
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt = luaL_checklstring(L, 2, &tam_txt);
    ctx = (HmacMd5Context*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"))->u;
    
    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; 
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    lua_pushvalue(L, 1);
    return 1;

}



static int p_HmacMd5Context___call(lua_State*L){
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt = luaL_checklstring(L, 2, &tam_txt);
    ctx = (HmacMd5Context*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"))->u;
    
    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; 
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    lua_pushvalue(L, 1);
    return 1;

}

static int p_HmacMd5Context___gc(lua_State*L){
    HmacMd5Context *ctx;
    userdata *este = (userdata*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"));
    ctx = (HmacMd5Context *)este->u;
    if(ctx){
		free(ctx);
		este->u = NULL;
	}
    return 0;
}


static int p_HmacMd5Context___tipo(lua_State*L){
    HmacMd5Context *ctx;
    userdata *este = (userdata*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"));
    ctx = (HmacMd5Context *)este->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushstring(L, "HmacMd5Context já liberado.");
    return 2;

	}
    lua_pushliteral(L, "userdata::HmacMd5Context");
    return 1;

}


static int p_HmacMd5Context___convstring(lua_State*L){
    HmacMd5Context *ctx;
    userdata *este = (userdata*)((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"));
    ctx = (HmacMd5Context *)este->u;
    if(!ctx){
    lua_pushnil(L);
    lua_pushstring(L, "HmacMd5Context invalido ou já liberado.");
    return 2;

	}
	lua_pushfstring(L, "userdata::HmacMd5Context(%p)", ctx);
	return 1;
}




static int p_HmacMd5Context_final(lua_State*L){
    HmacMd5Context *ctx;
    userdata *este = ((userdata*)luaL_checkudata(L, 1, "userdata::HmacMd5Context"));
    ctx = (HmacMd5Context*)este->u;
    if( !ctx ){
		lua_pushnil(L);
		lua_pushliteral(L, "Md5 hmac erro: HmacMd5Context inválido, ou já liberado");
		return 2;
	}
    unsigned char output[HASHLEN];
    hmac_md5_final(ctx, output);
    lua_pushlstring(L, output, HASHLEN);
    

    if(ctx){
		free(ctx);
		este->u = NULL;		
	}
    
    return 1;
    return 0;
}




static int p_hmac_md5(lua_State*L){
    
    int status;
    size_t tam_key, tam_txt;
    
    
    const char *chave_ptr = luaL_checklstring(L, 1, &tam_key);
    const char *txt_ptr = luaL_checklstring(L, 2, &tam_txt);
    
    unsigned char hmac_result[HASHLEN];
    const unsigned char *key = (const unsigned char*)chave_ptr;
    const unsigned char *message = (const unsigned char*)txt_ptr;
    
    status = md5_hmac(key, tam_key, message, tam_txt, hmac_result);
    
    if (status != 0) {
		lua_pushnil(L);
        lua_pushstring(L, "HMAC Erro: no calculo do hash.");
        return 2; 
    }
    
    lua_pushlstring(L, (const char*)hmac_result, HASHLEN);
    return 1;    
    return 0;
}


static int p_hmac_md5_hex(lua_State*L){
    
    int status;
    size_t tam_key, tam_txt;
    
    
    const char *chave_ptr = luaL_checklstring(L, 1, &tam_key);
    const char *txt_ptr = luaL_checklstring(L, 2, &tam_txt);
    
    unsigned char hmac_result[HASHLEN];
    const unsigned char *key = (const unsigned char*)chave_ptr;
    const unsigned char *message = (const unsigned char*)txt_ptr;
    
    status = md5_hmac(key, tam_key, message, tam_txt, hmac_result);
    
    if (status != 0) {
		lua_pushnil(L);
        lua_pushstring(L, "HMAC Erro: no calculo do hash.");
        return 2; 
    }
    
    char hex [ 2 * HASHLEN + 1 ]; /*HEX tem 2 bytes para cada bytes binário.*/
    int ret = byte_to_hex(hmac_result, HASHLEN, hex, (2 * HASHLEN + 1));
    if(ret != 0){
		lua_pushnil(L);
		lua_pushliteral(L, "HMAC Erro: ao tentar converter para hex.");
		return 2;
	}
    lua_pushlstring(L, (const char*)hex, 2 * HASHLEN + 1);
    return 1;    
    return 0;
}

static const luaL_Reg lib_Md5Context_funcs[] = {
    {"atualize", p_Md5Context_update},
    {"__gc", p_Md5Context___gc},
    {"__convstring", p_Md5Context___convstring},
    {"__tipo", p_Md5Context___tipo},
    {"teste", p_Md5Context_assert},
    {"__bshl", p_Md5Context___bshl},
    {"__call", p_Md5Context___call},
    {"__add", p_Md5Context___add},
    {"finalize", p_Md5Context_final},
    {"finalize_hex", p_Md5Context_final_hex},
    {NULL, NULL}
};


static const luaL_Reg lib_HmacMd5Context_funcs[] = {
    {"atualize", p_HmacMd5Context_update},
    {"__add", p_HmacMd5Context___add},
    {"__bshl", p_HmacMd5Context___bshl},
    {"__call", p_HmacMd5Context___call},
    {"__gc", p_HmacMd5Context___gc},
    {"__tipo", p_HmacMd5Context___tipo},
    {"__convstring", p_HmacMd5Context___convstring},
    {"finalize", p_HmacMd5Context_final},
    {NULL, NULL}
};


static const luaL_Reg lib_pmd5_funcs[] = {
    {"inicialize", p_pmd5_init},
    {"binario", p_pmd5},
    {"hex", p_pmd5_hex},
    {"hmac_inicialize", p_md5_hmac_init},
    {"hmac", p_hmac_md5},
    {"hmac_hex", p_hmac_md5_hex},
    {NULL, NULL}
};

/*funcao para registrar o modulo*/
LUA_API int luaopen_pmd5(lua_State *L) {
    PRIS_REGISTER_LIB(L, "pmd5", lib_pmd5_funcs);
    PRIS_REGISTER_META(L, "userdata::Md5Context", lib_Md5Context_funcs);
    PRIS_REGISTER_META(L, "userdata::HmacMd5Context", lib_HmacMd5Context_funcs);
    tab("VERSAO", literal, VERSAO); /*Veja essa macro e outras em prisma.h*/
    return 1;
}
#endif /* end _LIB_PMD5_ }=====================*/
