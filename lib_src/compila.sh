#!/bin/bash

#script bash para compilar a lib gerada em sistemas linux.

#path includes:
pincludes=" -I./prisma "  
includes="  -I../include -I../vendor "
#libs para linkar ou outros fontes em C para compilar ou .o para compilar juntos
clibs="  ../vendor/WjCryptLib_Md5.c "

#diretorio da bilibotecas Nativas Prisma para instalação, altere de acordo com seu local de instalação.
prismaclib="/usr/local/lib/prisma/1.0/clib";

nomelib="../libpmd51.0.so";
nomec="pmd5.c";
echo "Compilando" $nomelib

gcc -fPIC  -O2 -shared -o $nomelib $nomec  $pincludes $includes $clibs 

chmod +x $nomelib


echo "Digite Sua senha para instalar:"

sudo  cp -a  $nomelib  $prismaclib

echo "Compilação terminada, ENTER para sair...";

read gg;

