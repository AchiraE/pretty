#include <stdio.h>
#include "blit.h"
#include <ctype.h>
#include <string.h>
img* parse(char* str);
int ttype(char c);
int main(){
	//char str[]="x - x**3/6 + O(x**5)";
	char str[100];
	scanf("%s",str);
	img* ans=parse(str);
	blit_cdraw(ans);
	blit_freeimg(ans);
	return 0;
}
#define NUMBER 0
#define CONST 1
#define OPERP 2
#define OPERM 3
#define OPERD 4

int ttype(char c){
	if(isdigit(c))return NUMBER;
	if(c=='+'||c=='-')return OPERP;
	if(c=='*')return OPERM;
	if(c=='/')return OPERD;
	return CONST;
}
img* parse(char* str){
	int i,l;
	img *final,*tmp_0, *tmp_1, *t, *_t;
	final=blit_createimg(0,0);
	tmp_0=blit_createimg(0,0);
	tmp_1=blit_createimg(0,0);
	l=strlen(str);
	printf("parse: str=%s,l=%d\n",str,l);
	//case const/num , put it to tmp_0
	//case * , put tmp_0 to tmp_1
	//case +- , put tmp_0 to tmp_1 ,then put tmp_1 to final 
	//case / , put tmp_0 to tmp_1, then put frac(tmp1,parse) to tmp_1
	//case ^ , put power(tmp_0,parse) to tmp_1
	for(i=0;i<l;i++){
		switch(ttype(str[i])){
			case NUMBER:
			case CONST:
			t=blit_gen(5,9);//TODO -- gen a font glyph
			_t=blit_con(tmp_0,t,1);
			blit_freeimg(tmp_0,t,0);
			tmp_0=_t;
			break;
			case OPERP:
			t=blit_con(tmp_1,tmp_0);
			_t=blit_con(final,t);
			blit_freeimg(final,t,tmp_0,tmp_1,0);
			t=blit_gen(5,9);
			final=blit_con(_t,t,1);
			blit_freeimg(_t,t,0);
			tmp_0=blit_createimg(0,0);
			tmp_1=blit_createimg(0,0);	
			break;
			case OPERD:
			i++;
			if(str[i]=='('){
				int j,count=0;
				for(j=i;;j++){
					if(str[j]=='(')count++;
					if(str[j]==')')count--;
					if(!count)break;
				}
				char *s;
				s=(char*)malloc(sizeof(char)*(j-i));
				strncpy(s,&str[i+1],j-i-1);
				s[j-i-1]=0;
				t=parse(s);	
				_t=blit_con(tmp_1,tmp_0);
				blit_freeimg(tmp_1,tmp_0,0);
				tmp_1=blit_frac(_t,t);
				blit_freeimg(t,_t,0);
				tmp_0=blit_createimg(0,0);
				i=j;
			}else{
				int j;
				for(j=i;j<l;j++){
					if(ttype(str[j])!=CONST&&ttype(str[j])!=NUMBER)
						break;
				}
				char *s;
				s=(char*)malloc(sizeof(char)*(j-i+1));
				strncpy(s,&str[i],j-i);
				s[j-i]=0;	
				t=parse(s);	
				_t=blit_con(tmp_1,tmp_0);
				blit_freeimg(tmp_1,tmp_0,0);
				tmp_1=blit_frac(_t,t);
				blit_freeimg(t,_t,0);
				tmp_0=blit_createimg(0,0);
				i=j-1;				
			}
			break;
			case OPERM:
			if(str[i+1]=='*'){
				//power
				i+=2;
				if(str[i]=='('){
					int j,count=0;
					for(j=i;;j++){
						if(str[j]=='(')count++;
						if(str[j]==')')count--;
						if(!count)break;
					}
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i));
					strncpy(s,&str[i+1],j-i-1);
					s[j-i-1]=0;
					t=parse(s);		
					_t=blit_power(tmp_0,t);
					blit_freeimg(t,tmp_0,0);
					t=blit_con(tmp_1,_t);
					blit_freeimg(tmp_1,_t,0);
					tmp_0=blit_createimg(0,0);
					tmp_1=t;	
					i=j;
				}
				else{
					int j;
					for(j=i;j<l;j++)
						if(ttype(str[j])!=CONST&&ttype(str[j])!=NUMBER)
							break;
					char *s;
					s=(char*)malloc(sizeof(char)*(j-i+1));
					strncpy(s,&str[i],j-i);
					s[j-i]=0;
					t=parse(s);		
					_t=blit_power(tmp_0,t);
					blit_freeimg(t,tmp_0,0);
					t=blit_con(tmp_1,_t);
					blit_freeimg(tmp_1,_t,0);
					tmp_0=blit_createimg(0,0);
					tmp_1=t;	
					i=j-1;
				}
			}else{
				//mul
				_t=blit_con(tmp_1,tmp_0);
				t=blit_gen(5,9);
				tmp_1=blit_con(_t,t,1);
				blit_freeimg(t,_t,tmp_0,0);
				tmp_0=blit_createimg(0,0);
			}
			break;
		}
	}
	t=blit_con(tmp_1,tmp_0);
	_t=blit_con(final,t);
	blit_freeimg(final,t,tmp_0,tmp_1,0);
	final=_t;
	return final;
}