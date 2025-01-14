#include "pbc.h"
#include <gmp.h>
#include <string.h>

pairing_t pairing;
pbc_param_t param;
mpz_t n,q1,q2,q3,q4;
element_t g,f;
mpz_t sk[2];
element_t pub[3];
mpz_t lamada,gama;

element_t * getg(){
	return &g;
}
element_t * getPub(){
	return pub;
}

void initCryCilent(){
	mpz_init_set_str(n,"16318652668508626553901997551321249670550415089957985779649649188681555977376806012375139978805122033918364202614531292848132025501823453758650922019829333429468797229824540751887126169063683937250201618895277235033040042353335512656744891531678329705209825358993259462267121126232937290589384782872313022173577055045795728610058337599616681110557342897993647311730591930489171432822924546291464234572509278214710654818160815291666652822918077308899351320084852209569421847926524661028162620217796702188802399509064675647379059153024208721911939534886938570943493341126695175885008342780803077318659273233459428322637",10);
	pbc_param_init_a1_gen(param,n);

	pairing_init_pbc_param(pairing,param);


}
void setCryPubKeypub(char * data, int type){
	element_init_G1(pub[type],pairing);
	element_from_bytes(pub[type],(unsigned char*)data);

}

void setCryg(char * data){
	element_init_G1(g,pairing);
	element_from_bytes(g,(unsigned char*)data);

}

void setCry(element_t* c,char * data){


	element_init_G1(c[0],pairing);
	element_from_bytes(c[0],(unsigned char*)data);

}

element_t* initKey(){
	printf("initialzation key\n");
	mpz_init(sk[0]);
	mpz_init(sk[1]);

	element_init_G1(pub[0],pairing);
	element_init_G1(pub[1],pairing);
	element_init_G1(pub[2],pairing);

	mpz_t h0;
	mpz_init(h0);
	mpz_mul(h0,q2,q3);
	mpz_mul(sk[0],h0,q4);

	mpz_t h1;
	mpz_init(h1);
	mpz_mul(h1,q1,q3);
	mpz_mul(sk[1],h1,q4);


	mpz_t h2;
	mpz_init(h2);
	mpz_mul(h2,q2,q3);
	element_pow_mpz(pub[0],g,h2);
	mpz_t h3;
	mpz_init(h3);
	mpz_mul(h3,q1,q3);
	element_pow_mpz(pub[1],g,h3);
	mpz_t h4;
	mpz_init(h4);
	mpz_mul(h4,q1,q2);
	element_pow_mpz(pub[2],g,h4);
	return pub;
}


void homomorphism(element_t c, element_t* cs,int size){
	element_init_G1(c,pairing);
	for(int i=0;i<size;i++)
		element_mul(c,c,cs[i]);
}





void encryption(element_t c,mpz_t m){

	printf("start encryption\n");
	mpz_t m2;
	mpz_init(m2);
	mpz_mul(m2,m,m);
	element_t r,c1,c2,c3;
	element_init_Zr(r,pairing);
	element_init_G1(c,pairing);
	element_init_G1(c1,pairing);
	element_init_G1(c2,pairing);
	element_init_G1(c3,pairing);
	element_random(r);
	element_pow_mpz(c1,pub[0],m);
	element_pow_mpz(c2,pub[1],m2);
	element_pow_zn(c3,pub[2],r);
	element_mul(c,c1,c2);
	element_mul(c,c,c3);


}


void decryption(int*  ms,element_t c){
	printf("decryption start\n");
	int T=1000000;
	memset(ms,-1,sizeof(int)*2);
	element_t ms1,ms2,ms3,ms4,v1,v2;
	element_init_G1(ms1,pairing);
	element_init_G1(ms2,pairing); 
	element_init_G1(ms3,pairing);
	element_init_G1(ms4,pairing);
	element_init_G1(v1,pairing);
	element_init_G2(v2,pairing);
	element_pow_mpz(ms1,c,sk[0]);
	element_pow_mpz(v1,pub[0],sk[0]);
	for(int i=1;i<T;i++){
		element_mul(ms3,ms3,v1);
		if(element_cmp(ms3,ms1)==0){
			ms[0]=i;
			break;
		}
	}
	element_pow_mpz(ms2,c,sk[1]);
	element_pow_mpz(v2,pub[1],sk[1]);
	for(int i=1;i<T;i++){
		element_mul(ms4,ms4,v2);
		if(element_cmp(ms4,ms2)==0){
			ms[1]=i;
			break;
		}
	}

}
void decryptionByte(int*  ms,char* data){
	element_t c;
	element_init_G1(c,pairing);
	element_from_bytes(c,(unsigned char *)data);
	decryption(ms,c);

}
element_t* initParam(){
	printf("param initialzation\n");
	mpz_init(q1);
	mpz_init(q2);
	mpz_init(q3);
	mpz_init(q4);
	mpz_init_set_str(n,"16318652668508626553901997551321249670550415089957985779649649188681555977376806012375139978805122033918364202614531292848132025501823453758650922019829333429468797229824540751887126169063683937250201618895277235033040042353335512656744891531678329705209825358993259462267121126232937290589384782872313022173577055045795728610058337599616681110557342897993647311730591930489171432822924546291464234572509278214710654818160815291666652822918077308899351320084852209569421847926524661028162620217796702188802399509064675647379059153024208721911939534886938570943493341126695175885008342780803077318659273233459428322637",10);
	mpz_init_set_str(q1,"10891858478033897760704150755061086598164410582191714404557464677216868958951983455119494054541383739609558073106372570602130536832014786366691205947339093",10);
	mpz_init_set_str(q2,"12389395596547640756517039214836044278418146506689546938858124643470140302041294912570539196819809440429091172753945342693877454067931079211656809044832917",10);
	mpz_init_set_str(q3,"10286304648887947679439664910510137977548325688349344821149027040856585024147675742880180855238721330788760702851996247963284723578481976190373271296715923",10);
	mpz_init_set_str(q4,"11756358096998277835308225015499595897057008464719297663428552033133752055541951658265715783525419547628057529943241262936429204451508922357212201718422999",10);
	pbc_param_init_a1_gen(param,n);





	pairing_init_pbc_param(pairing,param);



	pairing_t pairing2;
	pbc_param_t param2;
	pbc_param_init_a1_gen(param2,n);
	pairing_init_pbc_param(pairing2,param);
	element_t x,y,z;
	element_init_G1(g,pairing);
	element_init_G1(x,pairing);
	element_init_G1(y,pairing);
	element_init_G1(z,pairing);
	element_init_G1(f,pairing);
	element_random(g);
	element_printf("g %B\n",g);
	element_pow_mpz(f,g,q2);
	element_pow_mpz(f,f,q3);
	element_pow_mpz(f,f,q4);
	element_printf("f %B\n",f);
	//element_pow_mpz(x,g,q1);
	//element_printf("x %B\n",x);
	//element_pow_mpz(y,x,q2);
	//element_printf("y %B\n",y);
	//element_pow_mpz(z,y,q3);
	//element_printf("z %B\n",z);
	//element_pow_mpz(x,z,q4);
	//element_printf("x %B\n",x);
	return &g;

}

void initRegist(mpz_t ek,int k,int id){

	mpz_init_set_str(lamada,"30097008700300000230023001",10);
	mpz_init(gama);
	mpz_sub(gama,q1,lamada);
	mpz_init(ek); 
	mpz_set(ek,lamada);
	mpz_t as[1024];
	for(int i=0;i<k;i++){
		//TODO  need change
		mpz_init_set_si(as[i],i*24);

	}

	mpz_t x;
	mpz_init_set_si(x,id);
	for(int i=0;i<k-1;i++){

		mpz_mul(as[i],as[i],x);
		mpz_mul(x,x,x);
		mpz_add(ek,ek,as[i]);

	}

}

void aggrea(element_t* ac,element_t * cs,int len){

	element_init_G1(*ac,pairing);

	for(int i=0;i<len;i++)
	{
		element_mul(*ac,cs[i],*ac);
	}

}


/*
 int main(void){


	 //char param[1024];
	 //FILE *file=NULL;
	 //file=fopen("/home/xiaozhang/下载/b.param","r");
	 //size_t count=fread(param,1,1024,file);
	 //if(!count)pbc_die("in error");
	 /*
	  param initialization

	  * 
	  initParam();
	  initKey();

	  mpz_t m1,m2;
	  mpz_init_set_str(m1,"20",10);
	  mpz_init_set_str(m2,"10",10);
	  element_t cs[2];
	  encryption(cs[0],m1);
	  encryption(cs[1],m2);
	  element_printf("c1 is %B    c2  is %B\n ",cs[0],cs[1]);
	  element_t c;
	  homomorphism(c,cs,2);
	  int ms[2];
	  decryption(ms,c);
	  printf("m is %d\n",ms[0]);
	  printf("m2 is %d\n",ms[1]);
	  return 0;
	  }


	  */













	 