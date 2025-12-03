# 📦 pmd5 - Binding MD5 para Prisma-1.0


## 📝 Descrição

A biblioteca **`pmd5`** é um *binding* em C para o algoritmo de *hashing* MD5 para **Prisma-v1.0**. 

Ela oferece duas formas de uso:

1.  **Funções *One-Shot***: Cálculo rápido do hash em uma única chamada:
```lua
    local res = pmd5.binario(txt);
    imprima( string.hex(res));
```
2.  **API Incremental**: Para calcular o hash de grandes volumes de dados (*streams*) ou múltiplas partes:
    ```lua
    local contexto = pmd5.inicialize();
    contexto:atualize(txt1);
    contexto:atualize(txt2);
    ...
    local res = contexto:finalize_hex();
    imprima(res);
    ```

O código é construído usando o `plibc`.

---

## 🚀 Instalação e Compilação

Esta biblioteca utiliza **CMake** para gerenciar a compilação e instalação.

### Requisitos

* Interpretador Prisma (v1.0.107+)
* Compilador C (GCC, Clang ou MSVC)
* CMake (v3.18+)

### Compilação (Linux/macOS)

O CMake utiliza a variável `CMAKE_INSTALL_PREFIX` (que por padrão é `/usr/local`) e a variável `PRISMA_INSTALL_DIR` definida no `CMakeLists.txt` para instalar o módulo (`pmd5.so`) no caminho correto do Prisma.

Use os comandos abaixo:

```bash
mkdir build
cd build
cmake ..
sudo make install
```

**Para instalar em outro local, use:** `cmake .. -D CMAKE_INSTALL_PREFIX=/caminho/personalizado`

---
## 💡 Como Usar
A biblioteca exporta duas funções principais (hex, binario) e uma API incremental (inicialize).

### 1. Funções One-Shot (Uso Simples)
Cálculo de hash em uma única chamada.

```lua
local pmd5 = inclua'pmd5'
local texto = "exemplo de string para hashing";

//-- Retorna o hash como string hexadecimal (padrão 32 caracteres)
imprima(pmd5.hex(texto)); 
//-- Saída: 2662c11867c48f8045995408b47120cc

//-- Retorna o hash como string binária (16 bytes)
local binario = pmd5.binario(texto);
imprima(binario:hex()); 
// Saída: 2662c11867c48f8045995408b47120cc
```
### 2. API Incremental (Objetos MD5_CTX)
Permite atualizar o hash com múltiplas strings (ideal para processamento de arquivos grandes ou em streaming).

Criação e Atualização
O objeto MD5_CTX suporta três métodos amigáveis para atualização:

ctx:atualize(txt)

Operador de call: ctx(txt)

Operador de shift (<<): ctx << txt

Operador de soma (+): ctx = ctx + txt


```lua
local pmd5 = inclua'pmd5'
local ctx = pmd5.inicialize();

//-- Atualização via operador de call e shift (encadeamento)
ctx = ctx("dados1") << "dados2" << "dados3";

//-- Atualização via operador de adição (necessita reatribuição)
ctx = ctx + "dados4" + "dados5";

//-- Atualização via método explícito
ctx:atualize("dados6");
```

### Finalização do Hash

A finalização faz a correta liberação da memória do objeto ctx, e retorna o valor final do hash.

```lua
//-- Exemplo 1: Finalização Hexadecimal
local ctx_hex = pmd5.inicialize() << "dados para hash";
local res_hex = ctx_hex:finalize_hex(); 
imprima("Hash Hex: ", res_hex); 
//-- O objeto ctx_hex é liberado após esta chamada.

//-- Exemplo 2: Finalização Binária (para usar funções da lib string)
local ctx_bin = pmd5.inicialize() << "dados para hash";
local res_bin = ctx_bin:finalize(); 
imprima("Hash Hex: ", res_bin:hex()); 
//-- O objeto ctx_bin é liberado.
```

---
## 📜 Licença

Este projeto é **software livre e desvinculado (free and unencumbered)** dedicado ao **Domínio Público** sob a **The Unlicense**.

O texto completo da dedicação pode ser encontrado no arquivo **[LICENSE](LICENSE)** neste repositório.

Para mais saber mais sobre `Unlicense`, visite: [http://unlicense.org/](http://unlicense.org/)


## 🧑‍💻 Contribuição
Sinta-se à vontade para enviar pull requests ou abrir issues se encontrar problemas.
