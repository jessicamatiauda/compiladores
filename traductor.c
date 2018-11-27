#include "parser.c"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define CANT_COMP_LEX 10

void json_trad();
void element_trad();
void array_trad();
void arrayB_trad();
void element_list_trad();
void element_listB_trad();
void object_trad();
void objectB_trad();
void attribute_trad();
void attributes_list_trad();
void attributes_listB_trad();
void attribute_name_trad();
void attribute_value_trad();
void match(int);
void string_sin_comillas(char* cadena);
int tabulacion=0;
void print_tab();


void string_sin_comillas(char* cadena){
    char string[TAMLEX];
    strcpy(string,cadena);
    int i=1;
    while(string[i] != '"'){
        printf("%c",string[i]);
        i++;
    }
}
void print_tab(){
    int i=0;
    for (i=1;i<=tabulacion;i++){
        printf(" ");
    }
}

void json_trad(){
    element_trad();
}

void element_trad(){

    if(t.compLex == L_LLAVE){
        object_trad();
    }
    else if(t.compLex == L_CORCHETE){
        array_trad();
    }
    
}


/*array -> [ arrayB*/
void array_trad(){  
    if(t.compLex == L_CORCHETE){
        
        match(L_CORCHETE);
        arrayB_trad();

    }
}

/*array' -> element-list ] | ]*/
void arrayB_trad(){
    
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        printf("\n");
        print_tab();
        printf("<item>\n");
        tabulacion+=4;
        element_list_trad();
        match(R_CORCHETE);
        print_tab();
        printf("</item>\n");
    }
    else if(t.compLex == R_CORCHETE){
        tabulacion-=4;
        match(R_CORCHETE);
    }

}

/*element-list -> element element-list'*/
void element_list_trad(){
    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        element_trad();
        element_listB_trad();
        
    }
}


/*element-list' ->  ,element element-list' | ε*/
void element_listB_trad(){
    if(t.compLex == R_CORCHETE){ 
        //tabulacion-=4;
        return;
    }
    
    if(t.compLex == COMA){  
        match(COMA);
        //  tabulacion-=4;
        print_tab();
        printf("</item>\n");
        print_tab();
        printf("<item>\n");
        tabulacion+=4;
        element_trad();
        element_listB_trad();

        
    }
    
}


/*  object -> { object'*/
void object_trad(){
    if(t.compLex == L_LLAVE){
        match(L_LLAVE);
        objectB_trad();
    }

}

/*   object' -> attributes-list} | }*/
void objectB_trad(){
    if(t.compLex == LITERAL_CADENA){
        attributes_list_trad();
        match(R_LLAVE);

    }
    else if(t.compLex == R_LLAVE){
        //tabulacion-=4;
        match(R_LLAVE);
    }
}

/*   attributes-list -> attribute attributes-list'*/
void attributes_list_trad(){
    if(t.compLex == LITERAL_CADENA){
        attribute_trad();
        attributes_listB_trad();
    }
}

/*   attributes-list' -> ,attribute attributes-list' | ε*/
void attributes_listB_trad(){
 
    if (t.compLex == R_LLAVE){
        tabulacion-=4;
        return;
    }
  
    if(t.compLex == COMA){
        match(COMA);
        attribute_trad();
        attributes_listB_trad();
    }
}

/*   attribute -> attribute-name : attribute-value*/
void attribute_trad(){
    if(t.compLex == LITERAL_CADENA){
        print_tab();
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
        printf("<");
        attribute_name_trad();
        printf(">");
        match(DOS_PUNTOS);
        attribute_value_trad();
        printf("</");
        string_sin_comillas(lexema);
        printf(">\n");    
    }
}

/*   attribute-name -> string*/
void attribute_name_trad(){
    if(t.compLex == LITERAL_CADENA){    
        string_sin_comillas(t.lexema);
        match(LITERAL_CADENA);
    }
}

/*attribute-value -> element | string | number | true | false | null*/
void attribute_value_trad(){

    if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        tabulacion +=4;
        element_trad();

    }
    else if(t.compLex == LITERAL_CADENA){
        printf("%s",t.lexema);
        match(LITERAL_CADENA);
    }
    else if(t.compLex == LITERAL_NUM){
        printf("%s",t.lexema);
        match(LITERAL_NUM);
    }
    else if(t.compLex == PR_TRUE){
        printf("%s",t.lexema);
        match(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE){
        printf("%s",t.lexema);
        match(PR_FALSE);
    }
    else if(t.compLex == PR_NULL){
        printf("%s",t.lexema);
        match(PR_NULL);
    }
}



/*Main*/
int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        sigLex();
        json();
        if(accept){
            fclose(archivo);
            archivo=fopen(args[1],"rt");
            sigLex();
            json_trad();
        }
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
