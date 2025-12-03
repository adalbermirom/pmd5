/**
*  $Id: pmd5.c,v 1.0 2019/08/27 Exp $
*  binding md5 para Prisma-1.0
*  @author  Adalberto Amorim Felipe
* 
* Este arquivo é um plib.c
* 
* Para transpilar em uma lib é necessário o utilitário plibc
* 
* plibc pmd5.plib.c
* 
* isso irá gerar uma pasta lib_src que conterá o código C transpilado.
* 
* Para facilitar eu mesmo já vou deixar transpilado sempre na versão mais recente.
* 
* Vou deixar também o código do plibc na pasta 'scripts' caso alguém queira usá-lo para
* customizar e transpilar também. 
* 
* Lembre-se: ele precisa de Prisma já instalado, v1.0.107 ou superior.
* 
* prisma ./scripts/pblibc pmd5.plib.c
* 
* Porém o pblic está em fazes de testes, e não gera mensagens de erros fáceis de debugar.
* 
*/

$.modulo: pmd5
$.AUTOR: "Adalberto"
$.VERSAO: "pmd5-1.3"
$.nome_info: "Lib com funções de hash md5 para Prisma-v1.0"


$.clibs: ../vendor/WjCryptLib_Md5.c
$.includes: -I../include -I../vendor
 
 
#include <string.h>
#include <stdlib.h>
#include "WjCryptLib_Md5.h"

#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>

#define MD5_SIZE 18
#define MD5_SIZE_HEX 34

/*
--[[[
\fonte pmd5.c
\versao Versão: 1.3 estável
\autor Adalberto
\resumo Biblioteca de hash md5 para Prisma feita em C.

<p>
  Esta lib binding foi feita em cima do código de Cameron Rich.
</p>
<p>
  Sua licença permite ser usada de forma totalmente gratuita, tendo apenas de manter
  as notas originais que já se encontram no arquivo ./include/md5.h
</p>

\h3 Exemplo de uso:

\codigo prisma
local pmd5 = inclua'pmd5'
local txt = "minha string longa para fazer um hashing de exemplo md5..........";
imprima(pmd5.hex(txt));
imprima('b7f726e31dabcaa19712f420deb01e54');
\codigo--

Saída:
\bloco
b7f726e31dabcaa19712f420deb01e54
b7f726e31dabcaa19712f420deb01e54
\bloco--
]]

*/ 
 

/*
--[[[
\fn pmd5.inicialize()
\resumo Cria e inicializa um novo objeto de contexto MD5 (Md5Context) para hashing incremental.
\retorno 1 userdata O objeto Md5Context recém-criado, pronto para ser atualizado.
\retorno 2 string Em caso de erro, retorna 'nil' seguido da mensagem de erro.
\h4 Exemplo:
\codigo prisma
local ctx = pmd5.inicialize();
\codigo--
]]
funcao pmd5.inicialize()
*/

funcao pmd5_init() #[export inicialize]
    Md5Context *ctx = (Md5Context*)calloc(1, sizeof(Md5Context));
    if (ctx == NULL){
		$.retorne $.nulo, $.literal("Memoria insuficiente ao criar Md5Context");
	}
	//Md5Initialise(ctx);
	
	Md5Initialise(ctx);
	
	userdata *new = $.new_userdata(Md5Context);
	new->u = ctx;
	$.retorne $.userdata(new);
fim

/*
--[[[
\fn ctx:atualize(txt)
\resumo Adiciona dados ao contexto MD5 (Md5Context) atual.
\param txt string A porção da string a ser adicionada ao hash.
\retorno 1 userdata O próprio objeto Md5Context, permitindo encadeamento.
\retorno 2 string Em caso de erro, retorna 'nil' seguido da mensagem de erro.
\h4 Notas:
<p>
Funciona como alias para os operadores '+', '<<' e a chamada de função (ctx("string")).
</p>
\codigo prisma
ctx:atualize("A"):atualize("B"):atualize("C");
\codigo--
]]
funcao ctx:atualize(txt)
*/


funcao Md5Context:update(este, txt) #[export atualize]
    Md5Context *ctx = $.Md5Context(este)->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
    size_t tam_s;
    const char *s, tam_s = $.lstring(txt);
    //Md5Update(ctx, s, tam_s);
    Md5Update(ctx, s, tam_s);
    
    retorne $.param(este); //retorna o próprio userdata.
fim



funcao Md5Context:__gc(este)
    Md5Context *ctx;
    userdata *ud = $.Md5Context(este);
    ctx = (Md5Context *)ud->u;
    if(!ctx) return 0;
    
    free(ctx);
    ud->u = NULL;
fim


funcao Md5Context:__convstring(este)
    userdata *ud = $.Md5Context(este);
    if(!ud->u){
		retorne $.literal("userdata::Md5Context\050liberado\051");
    }
    lua_pushfstring(L, "userdata::Md5Context\050%p\051", ud->u);
    return 1;
fim

funcao Md5Context:__tipo(este)
    retorne $.string("userdata::Md5Context");
fim

funcao Md5Context:assert(este) #[export teste]
    userdata *ud = $.Md5Context(este);
    if(!ud->u){
		retorne $.nulo, $.literal("userdata::Md5Context invalido ou já liberado.");
    }
    lua_pushfstring(L, "userdata::Md5Context\050%p\051", ud->u);
    return 1;
fim


/*
--[[[
\fn ctx << txt
\resumo Metamétodo para o operador de deslocamento de bits à esquerda (<<). Usado para atualizar o hash com uma nova string.
\param txt string A string a ser adicionada ao hash.
\retorno 1 userdata O próprio objeto Md5Context para encadeamento.
\h4 Exemplo:
\codigo prisma
local ctx = pmd5.inicialize() << "dados1" << "dados2";
\codigo--
]]
    funcao ctx:__bshl(txt)
*/

funcao Md5Context:__bshl(este, txt) 
    Md5Context *ctx = $.Md5Context(este)->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
    size_t tam_s;
    const char *s, tam_s = $.lstring(txt);
    Md5Update(ctx, s, tam_s);
    
    retorne $.param(este); //retorna o próprio userdata.
fim

funcao Md5Context:__call(este, txt) 
    Md5Context *ctx = $.Md5Context(este)->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
    size_t tam_s;
    const char *s, tam_s = $.lstring(txt);
    Md5Update(ctx, s, tam_s);
    retorne $.param(este); //retorna o próprio userdata.
fim



/*
--[[[
\fn ctx + txt
\resumo Metamétodo para o operador de soma (+). Permite atualizar o hash com uma nova string de forma concisa.
\param txt string A string a ser adicionada ao hash.
\retorno 1 userdata O próprio objeto Md5Context para encadeamento.
\h4 Exemplo:
\codigo prisma
local ctx = pmd5.inicialize();
ctx = ctx + "dados1" + "dados2";
\codigo--
]]
    funcao ctx:__add(txt)
*/

//permite o comando para atualizar a string: ctx = ctx + "minha string ...";
funcao Md5Context:__add(este, txt) 
    Md5Context *ctx = $.Md5Context(este)->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
    size_t tam_s;
    const char *s, tam_s = $.lstring(txt);
    Md5Update(ctx, s, tam_s);
    retorne $.param(este); //retorna o próprio userdata.
fim



/*
--[[[
\fn ctx:finalize()
\resumo Finaliza o processo de hashing e retorna o resultado em formato binário (raw).
\resumo <strong>Atenção:</strong> Após a finalização, o objeto Md5Context é invalidado (liberado).
\retorno 1 string O hash MD5 como string binária (16 bytes).
\retorno 2 string Em caso de erro, retorna 'nil' seguido da mensagem de erro.
]]
funcao ctx:finalize()
*/

funcao Md5Context:final(este) #[export finalize]
    Md5Context *ctx;
    MD5_HASH digest;
    userdata *ud = $.Md5Context(este);
    ctx = (Md5Context *)ud->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
	
	Md5Finalise(ctx, &digest);
    if(ctx){
		free(ctx);
        ud->u = NULL;
    }
    lua_pushlstring(L, digest.bytes, MD5_HASH_SIZE);
    return 1;
fim



/*
--[[[
\fn ctx:finalize_hex()
\resumo Finaliza o processo de hashing e retorna o resultado em formato hexadecimal.
\resumo <strong>Atenção:</strong> Após a finalização, o objeto Md5Context é invalidado (liberado).
\retorno 1 string O hash MD5 como string hexadecimal (32 caracteres).
\retorno 2 string Em caso de erro, retorna 'nil' seguido da mensagem de erro.
]]
funcao ctx:finalize_hex()
*/

funcao Md5Context:final_hex(este) #[export finalize_hex]
    Md5Context *ctx;
    MD5_HASH digest;
    userdata *ud = $.Md5Context(este);
    ctx = (Md5Context *)ud->u;
    if(!ctx){
		retorne $.nulo, $.literal("Md5Context* inválido.");
	}
	Md5Finalise(ctx, &digest);
    if(ctx){
		free(ctx);
        ud->u = NULL;
    }
    
     /* o hex é o dobro de bytes por isso 2 vezes */
    char hex[2*MD5_HASH_SIZE+1];
    int i;
 
    for (i=0;i<16;i++) {
        sprintf(hex+2*i,"%02x",(unsigned char)digest.bytes[i]);
    }
    lua_pushlstring(L, hex, 2*MD5_HASH_SIZE);
    return 1;
fim


/*
--[[[
\fn pmd5.binario(s)
\resumo Calcula o hash MD5 de uma string completa em uma única etapa e retorna o resultado em formato binário (raw).
\param s string A string completa a ser hashada.
\retorno 1 string O hash MD5 como string binária (16 bytes).
\h4 Exemplo:
\codigo prisma
local bin = pmd5.binario("minha string");
imprima(bin:hex()); // Converte para hexadecimal usando a lib string nativa.
\codigo--
]]
funcao pmd5.binario(s)
*/

funcao pmd5(s) #[export binario]
    size_t tam_s;
    const char *s, tam_s = $.lstring(s);
    MD5_HASH digest;
    Md5Context ctx; 
    Md5Initialise(&ctx);
    Md5Update(&ctx, s, tam_s);
    Md5Finalise(&ctx, &digest);
    lua_pushlstring (L, digest.bytes, MD5_HASH_SIZE); 
    return 1;
fim 
 


/*
--[[[
\fn pmd5.hex(s)
\resumo Calcula o hash MD5 de uma string completa em uma única etapa e retorna o resultado em formato hexadecimal.
\param s string A string completa a ser hashada.
\retorno 1 string O hash MD5 como string hexadecimal (32 caracteres).
\h4 Exemplo:
\codigo prisma
imprima(pmd5.hex("minha string"));
// Saída: 2662c11867c48f8045995408b47120cc
\codigo--
]]
funcao md5.hex(s)
*/
funcao pmd5_hex(s) #[export hex]
   size_t tam_s;
    const char *s, tam_s = $.lstring(s);
    MD5_HASH digest;
    Md5Context ctx; 
    Md5Initialise(&ctx);
    Md5Update(&ctx, s, tam_s);
    Md5Finalise(&ctx, &digest);
    
    /* o hex é o dobro de bytes por isso 2 vezes */
    char hex[2*MD5_HASH_SIZE+1];
    int i;
 
    for (i=0;i<16;i++) {
        sprintf(hex+2*i,"%02x",(unsigned char)digest.bytes[i]);
    }
    lua_pushlstring(L, hex, 2*MD5_HASH_SIZE);
    return 1;
fim
