

//troca one char por other char se encotrados em orig na string received.


void pris_changeChar( const char *orig , char *received , char one, char other , int sizeorig )
{

	int i;
	for( i = 0 ; i < sizeorig ; i++ ){

		if( i == sizeorig - 1 ) { received[i] = '\0'; break; }
		received[i] = orig[i];
		if( received[i] == one ) received[i] = other;

	} //end for

}


int pris_multi_sz_len( const char *mult_sz) //necessario duplo \0\0 no fim da string
{
int i = 0;
int cont = 0;

while(*mult_sz){
	i += strlen(mult_sz) ;
	i++;
	mult_sz += (strlen(mult_sz)+1);
	cont++;
	}

return i + 1; //the size of multi_sz; including the \0 char;
}

//pega multistrngs do tipo "ola\0mundo\0em\0prisma\0\0" e separa os ítens em indices de tabelas prisma

//

void pris_Push_multistr_to_luatable(lua_State*L, char *ptr , int length)
{
	lua_pop(L,-1);
	lua_pop(L,-2);
	if( length <= 0 ) length = pris_multi_sz_len(ptr);

	//printf("+++ %i\n" , length);
	char receive[length];

	int i, cont=0 , n_args = 0;

	for(i = 0 ; i < length ; i++ )//primeiramente contamos quantas strings teremos, para criarmos a tabela no tamanho certo:
     {
		 if( ptr[i] == '\0') n_args++; //conta o número de \0 finais de strings.
	 }

	 lua_createtable(L, 0, n_args );
	 n_args = 1;
	for(i = 0 ; i < length ; i++ )
     {
		 receive[cont] = ptr[i];
		 if( ptr[i] == '\0'){

			 lua_pushstring(L,receive);  //empurra para pilha
			 lua_rawseti(L, -2, n_args );//define como indice de uma tabela

			 if(ptr[i+1] == '\0') break; //garantindo que nao leia o último zero
			  cont = 0; n_args++;

			 }else cont++;

	 }

	  //lua_pushvalue(L, -1); //return the lua table// nao precisa tabela criada já está no topo

	// printf("%i\n\n" , n_args);

}


// array of string C to lua table

void pris_strarray_to_table(lua_State*L,char **ext){//push string array into lua table
	int cont_ext = 0;
	lua_newtable(L); //cria uma tabela prisma em C
	while ( ext[cont_ext] ){ //processando cada item da matriz.
		lua_pushstring(L, ext[cont_ext] ); //pega cada string da matriz de string C
	    lua_rawseti(L, -2, cont_ext + 1 ); //+1 porque em prisma o indice comeca em 1 nao em 0;
		cont_ext = cont_ext + 1;
	}
	 //freev(ext); //liberando da memoria a matriz//tem que fazer um laco e liberar cada indice, so necessario se a matriz for retorno de malloc() ou calloc() ou realloc();
}

void pris_table_to_strarray(lua_State*L,int index,char **tab,int len){
   if(lua_istable(L,index)){
     int i = 0;
     for(i=0;i<len;i++){
       lua_rawgeti (L,index, i+1);
             
          tab[i] = (char*) luaL_checkstring(L,-1);
          lua_pop(L,1);
          //printf("%s\n",luaL_checkstring(L,-1));
       }
     }
}



//converter para inteiro lua 5.3
int prisma_getint(lua_State*L, int index){
	int ret = luaL_checknumber(L,index);
	return ret;
}

