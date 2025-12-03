cmake_minimum_required(VERSION 3.18)
project(pmd5 LANGUAGES C)

# Use:
#cmake -S. -Bbuild
#cmake --build build


# Define a biblioteca como COMPARTILHADA (SHARED)
# Isso cria uma lib dinamicamente carregável (como .so, .dll) 
add_library(pmd51.0 SHARED lib_src/pmd5.c vendor/WjCryptLib_Md5.c)

# 1. Inclui o diretório de headers para compilação (necessário para a Lib)
# Os headers estão em 'include/'
target_include_directories(pmd51.0 PUBLIC include vendor)

# 2. Define os caminhos de instalação específicos do sistema

if(UNIX)
    # Linux: /usr/local/lib/prisma/1.0/clib/
    # Usamos 'lib' para libs compartilhadas (.so)
    set(PRISMA_INSTALL_DIR "lib/prisma/1.0/clib")
elseif(WIN32)
    # Windows: C:\Prisma\1.0\bin\clibs\
    # Usamos 'bin' para refletir a pasta que contém os binários (.dll)
    set(PRISMA_INSTALL_DIR "bin/clibs")
else()
    # Padrão
    set(PRISMA_INSTALL_DIR "lib")
endif()

# 3. Adiciona as regras de INSTALAÇÃO do binário (a biblioteca compartilhada)

# LIBRARY: Instala o binário principal (libpmd5.so ou pmd5.dll)
# ARCHIVE: Instala o arquivo de arquivo estático (libpmd5.a ou .lib), 
#          que é gerado junto com a lib compartilhada em alguns sistemas.
install(TARGETS pmd51.0
    LIBRARY DESTINATION ${PRISMA_INSTALL_DIR}
    ARCHIVE DESTINATION ${PRISMA_INSTALL_DIR}
)

# NOTA: Os headers não são instalados, pois esta é uma lib binding.
