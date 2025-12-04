cmake_minimum_required(VERSION 3.18)
project(pmd5 LANGUAGES C)

# Use:
# cmake -S. -Bbuild
# cmake --build build

# 1. CRIAÇÃO DA BIBLIOTECA
add_library(pmd51.0 SHARED lib_src/pmd5.c vendor/WjCryptLib_Md5.c)

# 2. INCLUSÃO DE HEADERS
target_include_directories(pmd51.0 PUBLIC include vendor)

# 3. LINKAGEM NO WINDOWS
if(WIN32)
    # Apontando para a DLL conforme solicitado (Funciona bem no MinGW/GCC)
    set(PATH_TO_PRISMA "C:/Prisma/1.0/bin/libprisma1.0.dll")
    
    # Se estiver usando Visual Studio (MSVC) e der erro de linkagem, 
    # você precisará mudar a linha acima para o arquivo .lib correspondente (Import Library).
    
    if(EXISTS "${PATH_TO_PRISMA}")
        # O 'IMPLIB' abaixo é opcional, mas ajuda o CMake a entender que é uma import lib se necessário
        target_link_libraries(pmd51.0 PRIVATE ${PATH_TO_PRISMA})
    else()
        message(WARNING "DLL não encontrada em: ${PATH_TO_PRISMA}. Verifique o caminho.")
    endif()
endif()

# 4. DEFINIÇÃO DE PASTAS DE INSTALAÇÃO
if(UNIX)
    set(PRISMA_INSTALL_DIR "lib/prisma/1.0/clib")
elseif(WIN32)
    set(PRISMA_INSTALL_DIR "bin/clibs")
else()
    set(PRISMA_INSTALL_DIR "lib")
endif()

# 5. INSTALAÇÃO
install(TARGETS pmd51.0
    RUNTIME DESTINATION ${PRISMA_INSTALL_DIR}
    LIBRARY DESTINATION ${PRISMA_INSTALL_DIR}
    ARCHIVE DESTINATION ${PRISMA_INSTALL_DIR}
)
