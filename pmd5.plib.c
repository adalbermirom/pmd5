/**
*  $Id: pmd5.c,v 1.0  dez/2025 Exp $
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
 * Função auxiliar que converte bytes binários para hex, cada byte vira 2, ex: 10 = 0a;
 * params:
 *    1 src_hash: é o hash binário que já foi gerado para ser convertido a hex
 *    2 hash_len é o tamanho exato do tipo de hahs.
 *    3 hex_out é o array de bytes onde será gravado a saída (hex) ela deve ter o dobro de tamanho.
 *    4 hex_len é o tamanho do array de saída, seu tamanho sempre é o dobro + 1 de tamanho.*/
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
    
    int ret = byte_to_hex(digest.bytes, MD5_HASH_SIZE, hex, 2 * MD5_HASH_SIZE + 1);
    if( ret != 0) { /* != 0: error */
		lua_pushnil(L);
		lua_pushliteral(L, "MD5 erro: ao converter para hex.");
		return 2;
	}
    lua_pushlstring(L, hex, 2*MD5_HASH_SIZE);
    return 1;
fim








/* Implementação hmac md5 */

#define BLOCKSIZE 64  // 512 bits / 8 = 64 bytes
#define HASHLEN 16    // 128 bits / 8 = 16 bytes

// Assinatura da função hash subjacente (DEVE ser implementada)
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

// --------------------------------------------------------

/**
 * Função HMAC (Hash-based Message Authentication Code)
 * * @param key       Chave secreta de entrada.
 * @param key_len   Comprimento da chave.
 * @param message   Mensagem a ser autenticada.
 * @param msg_len   Comprimento da mensagem.
 * @param output    Ponteiro para armazenar o resultado do HMAC (deve ter HASHLEN bytes).
 */

typedef struct {
    Md5Context ctx;               // O contexto do hash SHA-512 (estado interno)
    unsigned char o_key_pad[BLOCKSIZE]; // Precisamos guardar isso para a etapa final
} HmacMd5Context;

void hmac_md5_init(HmacMd5Context *ctx, const unsigned char *key, size_t key_len) {
    unsigned char K[BLOCKSIZE];
    unsigned char i_key_pad[BLOCKSIZE];

    // 1. Prepara a chave (Lógica idêntica à sua anterior)
    memset(K, 0x00, BLOCKSIZE);
    if (key_len > BLOCKSIZE) {
        // Assume-se que hash() aqui é one-shot ou você faz o init/update/final do hash
        hash(key, key_len, K); 
    } else {
        memcpy(K, key, key_len);
    }

    // 2. Gera os Pads e guarda o o_key_pad no contexto
    for (int i = 0; i < BLOCKSIZE; i++) {
        i_key_pad[i] = K[i] ^ 0x36;
        ctx->o_key_pad[i] = K[i] ^ 0x5c; // Salva para o final!
    }

    // 3. Inicia o Hash e já processa o i_key_pad
    Md5Initialise(&ctx->ctx);
    Md5Update(&ctx->ctx, i_key_pad, BLOCKSIZE);

    // Limpeza da pilha local
    memset(K, 0, BLOCKSIZE);
    memset(i_key_pad, 0, BLOCKSIZE);
}

void hmac_md5_update(HmacMd5Context *ctx, const unsigned char *data, size_t len) {
    // Apenas repassa para o SHA-512
    Md5Update(&ctx->ctx, data, len);
}

void hmac_md5_final(HmacMd5Context *ctx, unsigned char *output) {
    unsigned char inner_hash[HASHLEN];
    MD5_HASH digest;

    // 1. Finaliza o hash interno (i_key_pad + mensagem)
    Md5Finalise(&ctx->ctx, &digest);
    memcpy(inner_hash, digest.bytes, HASHLEN);

    // 2. Calcula o hash externo (o_key_pad + inner_hash)
    // Aqui precisamos de um novo contexto temporário de hash
    Md5Context outer_ctx;
    Md5Initialise(&outer_ctx);
    Md5Update(&outer_ctx, ctx->o_key_pad, BLOCKSIZE);
    Md5Update(&outer_ctx, inner_hash, HASHLEN);
    
    Md5Finalise(&outer_ctx, &digest);
    memcpy(output, digest.bytes, HASHLEN);

    // Limpeza final do contexto
    memset(ctx->o_key_pad, 0, BLOCKSIZE);
    memset(inner_hash, 0, HASHLEN);
}


static int md5_hmac(const unsigned char *key, size_t key_len, 
                const unsigned char *msg, size_t msg_len, 
                unsigned char *output) {
    
    HmacMd5Context ctx;

    // O fluxo fica linear e elegante
    hmac_md5_init(&ctx, key, key_len);
    hmac_md5_update(&ctx, msg, msg_len);
    hmac_md5_final(&ctx, output);

    return 0; // Sucesso
}

/*
--[[[
\fn hmac_inicialize
\resumo Cria um novo contexto para cálculo incremental de HMAC-MD5.
\param chave {string} - A chave secreta inicial.
\retorno {userdata} - Um objeto de contexto (HmacMd5Context) pronto para uso.

\h3 Exemplo:
\codigo
    local md5 = inclua'pmd5';
    
    local chave_secreta = 'uma chave secreta qualquer para segurança!'
    local ctx = md5.hmac_inicialize(chave_secreta);
    ctx:atualize(dados);
    local resultado = ctx:finalize();
    local res_hex = string.hex(resultado);
    imprima(res_hex);
\codigo--
]]
funcao md5.hmac_inicialize(chave)
*/
funcao md5_hmac_init(arg_key) #[export hmac_inicialize]
    size_t key_len;
    HmacMd5Context *ctx;
    
    const char *key, key_len = $.lstring(arg_key);
    ctx = (HmacMd5Context *)calloc(1, sizeof(HmacMd5Context));
    if( ctx == NULL){
		lua_pushnil(L);
		lua_pushliteral(L, "Md5 hmac erro: memoria insuficiente para HmacMd5Context");
		return 2;
	}
    
    hmac_md5_init(ctx, key, key_len);
    
    userdata* new = $.new_userdata(HmacMd5Context);
    new->u = ctx;
    
    retorne $.userdata(new);    
fim


/*
--[[[
\fn atualize
\resumo Adiciona um pedaço de dados ao contexto de hash atual.
\param dados {string} - O fragmento de texto ou binário para processar.
\retorno {userdata} - Retorna o próprio contexto (self) para permitir encadeamento.
\h3 Exemplo:
\codigo
    local md5 = inclua'pmd5'; //cálculo md5 e hmac
    
    local chave = 'minha chave secreta!!!!!!';
    local meu_arquivo = 'hmac.prisma';//troque por um arquivo válido para o teste.
    funcao principal(arg)
         //abrindo um arquivo para leitura.
        local a, err = es.abra(meu_arquivo, 'leitura');
        se nao a entao erro(err); fim //se der erro...
        
        //iniciando o hmac contexto.
        local ctx = md5.hmac_inicialize(chave);
        
        local tambloco = 4096;
        
        enquanto verdadeiro inicio //laço infinito, controlado por dentro.
            dados = a:leia(tambloco); //Lê 4096 bytes por vez.
            se nao dados entao
                quebre;
            fim
            ctx:atualize(dados); //se ainda tiver dados, eles são processados pelo hmac.            
        fim
        
        a:feche();
        
        //finalizando:
        
        local res = ctx:finalize();
        res = string.hex(res);
        imprima(res, meu_arquivo);
        
        retorne 0;
    fim

\codigo--
]]
funcao HmacMd5Context:atualize(dados)
*/
funcao HmacMd5Context:update(este, txt, tam_txt) #[export atualize]
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt, tam_txt = $.lstring(txt);
    ctx = (HmacMd5Context*)$.HmacMd5Context(este)->u;

    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; // Ou disparar um erro com lua_error
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    retorne $.param(este); /*retorna o próprio userdata*/
fim


/*
--[[[
\fn __add
\resumo Operador de adição (+) para atualizar dados.
\param dados {string} - Texto a ser adicionado ao hash.
\retorno {userdata} - O próprio contexto.
\codigo prisma
  ctx = ctx + "dados"
\codigo--
]]
funcao HmacMd5Context:__add(dados)
*/
funcao HmacMd5Context:__add(este, txt, tam_txt)
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt, tam_txt = $.lstring(txt);
    ctx = (HmacMd5Context*)$.HmacMd5Context(este)->u;
    
    /* verificação do ponteiro por segurança*/
    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; // Ou disparar um erro com lua_error
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    retorne $.param(este); /*retorna o próprio userdata*/
fim

/*
--[[[
\fn __bshl
\resumo Operador Shift Left (<<) para atualização estilo stream.
\param dados {string} - Texto a ser adicionado ao hash.
\retorno {userdata} - O próprio contexto.
\codigo 
    ctx = ctx << "dados"
    //o mesmo que:
    ctx = ctx:atualize("dados");
\codigo--
]]
funcao HmacMd5Context:__bshl(dados)
*/
funcao HmacMd5Context:__bshl(este, txt, tam_txt) 
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt, tam_txt = $.lstring(txt);
    ctx = (HmacMd5Context*)$.HmacMd5Context(este)->u;
    
    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; // Ou disparar um erro com lua_error
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    retorne $.param(este); /*retorna o próprio userdata*/
fim


/*
--[[[
\fn __call
\resumo Permite usar o objeto como função para atualizar dados.
\param dados {string} - Texto a ser adicionado ao hash.
\retorno {userdata} - O próprio contexto.
\codigo
    ctx("dados")
\codigo--
]]
funcao HmacMd5Context:__call(dados)
*/
funcao HmacMd5Context:__call(este, txt, tam_txt)
    HmacMd5Context *ctx;
    size_t tam_txt;
    const char *txt, tam_txt = $.lstring(txt);
    ctx = (HmacMd5Context*)$.HmacMd5Context(este)->u;
    
    if (!ctx) {
        lua_pushnil(L);
        lua_pushstring(L, "Erro: Tentativa de atualizar contexto já finalizado.");
        return 2; // Ou disparar um erro com lua_error
    }
    
    hmac_md5_update(ctx, (const unsigned char *)txt, tam_txt);
    retorne $.param(este); /*retorna o próprio userdata*/
fim

funcao HmacMd5Context:__gc(este, txt, tam_txt)
    HmacMd5Context *ctx;
    userdata *este = (userdata*)$.HmacMd5Context(este);
    ctx = (HmacMd5Context *)este->u;
    if(ctx){
		free(ctx);
		este->u = NULL;
	}
fim


funcao HmacMd5Context:__tipo(este, txt, tam_txt)
    HmacMd5Context *ctx;
    userdata *este = (userdata*)$.HmacMd5Context(este);
    ctx = (HmacMd5Context *)este->u;
    if(!ctx){
		retorne $.nulo, $.string("HmacMd5Context já liberado.");
	}
	retorne $.literal("userdata::HmacMd5Context");
fim


funcao HmacMd5Context:__convstring(este, txt, tam_txt)
    HmacMd5Context *ctx;
    userdata *este = (userdata*)$.HmacMd5Context(este);
    ctx = (HmacMd5Context *)este->u;
    if(!ctx){
		retorne $.nulo, $.string("HmacMd5Context invalido ou já liberado.");
	}
	lua_pushfstring(L, "userdata::HmacMd5Context(%p)", ctx);
	return 1;
fim



/*
--[[[
\fn finalize
\resumo Encerra o cálculo, libera a memória do contexto e devolve o hash.
\retorno {string} - O hash binário final (64 bytes). O contexto torna-se inválido após esta chamada.
]]
funcao HmacMd5Context:finalize()
*/
funcao HmacMd5Context:final(este) #[export finalize]
    HmacMd5Context *ctx;
    userdata *este = $.HmacMd5Context(este);
    ctx = (HmacMd5Context*)este->u;
    if( !ctx ){
		lua_pushnil(L);
		lua_pushliteral(L, "Md5 hmac erro: HmacMd5Context inválido, ou já liberado");
		return 2;
	}
    unsigned char output[HASHLEN];
    hmac_md5_final(ctx, output);
    lua_pushlstring(L, output, HASHLEN);
    
    /* limpando a memória */
    if(ctx){
		free(ctx);
		este->u = NULL;		
	}
    
    return 1;
fim




/*
--[[[
\fn finalize_hex
\resumo Encerra o cálculo, libera a memória do contexto e devolve o hash.
\retorno {string} - O hash binário final (64 bytes). O contexto torna-se inválido após esta chamada.
]]
funcao HmacMd5Context:finalize_hex()
*/
funcao HmacMd5Context:final_hex(este) #[export finalize_hex]
    HmacMd5Context *ctx;
    userdata *este = $.HmacMd5Context(este);
    ctx = (HmacMd5Context*)este->u;
    if( !ctx ){
		lua_pushnil(L);
		lua_pushliteral(L, "Md5 hmac erro: HmacMd5Context inválido, ou já liberado");
		return 2;
	}
    unsigned char output[HASHLEN];
    hmac_md5_final(ctx, output);
    
    char hex [ 2 * HASHLEN + 1 ];
    byte_to_hex(output, HASHLEN, hex, 2 * HASHLEN + 1);
    
    lua_pushlstring(L, hex, 2*HASHLEN);
    
    /* limpando a memória */
    if(ctx){
		free(ctx);
		este->u = NULL;		
	}
    
    return 1;
fim



/*
--[[[
\fn hmac
\resumo Calcula o HMAC-MD5 de uma mensagem completa de uma só vez.
\param chave {string} - A chave secreta usada para o hashing.
\param mensagem {string} - A mensagem (texto ou binário) a ser autenticada.
\retorno {string} - O hash binário resultante (64 bytes), ou nulo em caso de erro.
]]
funcao md5.hmac(chave, mensagem)
*/
funcao hmac_md5(chave, txt) #[export hmac]
    
    int status;
    size_t tam_key, tam_txt;
    
    // Nota: Assumindo que a sintaxe $.lstring funciona corretamente no seu ambiente Prisma
    const char *chave_ptr, tam_key = $.lstring(chave);
    const char *txt_ptr, tam_txt = $.lstring(txt);
    
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
fim

/*
--[[[
\fn hmac_hex
\resumo Calcula o HMAC-MD5 de uma mensagem completa de uma só vez, mas retorna em hexadecimal.
\param chave {string} - A chave secreta usada para o hashing.
\param mensagem {string} - A mensagem (texto ou binário) a ser autenticada.
\retorno {string} - O hash hexadecimal resultante, ou nulo em caso de erro.
]]
funcao md5.hmac_hex(chave, mensagem)
*/
funcao hmac_md5_hex(chave, txt) #[export hmac_hex]
    
    int status;
    size_t tam_key, tam_txt;
    
    // Nota: Assumindo que a sintaxe $.lstring funciona corretamente no seu ambiente Prisma
    const char *chave_ptr, tam_key = $.lstring(chave);
    const char *txt_ptr, tam_txt = $.lstring(txt);
    
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
fim



