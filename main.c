#include "src/pbc_inshort.h"
#include "src/jianhong-zhang.h"

int main(){
	pairing_t pairing;
	init_pairing(pairing);	
	if(pairing_is_symmetric(pairing)==1)
		puts("This is a symmetric group");
	else
		puts("This is a asymmetric group");

//	t gt_test;
//	GT(gt_test,pairing);

	struct PublicKey PK;
	struct MasterSecertKey msk;
	SystemInitiaion(&PK,&msk,pairing);

//	Zr(msk.alpha,pairing);
//	element_printf("msk.alpha= %B\n",msk.alpha);
//	element_printf("PK.eg1h_alpha= %B\n",PK.eg1h_alpha);

//	element_pairing(gt_test,PK.g1,msk.h_);
//	element_pow_zn(gt_test,gt_test,msk.alpha);
//	element_printf("gt_test= %B\n",gt_test);

	struct UserPrivateKey skW_;
	KeyGen(&skW_, &PK, &msk, pairing);

	//element_printf("sk1_rho[5]= %B\n",skW_.sk1_rho[5]);


	struct CipherText CT;
	Encryption(&CT, &PK, &msk, pairing);
	/************************************************************
	element_set(fortest,Encryption(&CT, &PK, &msk, pairing));
	element_div(fortest,CT.C,fortest);
	element_invert(fortest,fortest);
	element_printf("fortest= %B\n",fortest);
************************************************************************/

	struct TransformationKey TK;
	TransformationKeyGeneration(&TK, &skW_, pairing);

	struct PartialDecryptionCiphertext CTp;
	PartialDecrypt(&CTp, &CT, &TK, &PK, pairing);

	Decrypt(&skW_, &CTp, pairing);


/*********************************	
	printf("sizeof element_t %d\n",sizeof(element_t));
	printf("sizeof CTp %d\n",sizeof(CTp));
	printf("sizeof CTp-C %d\n",sizeof(CTp.C));
	printf("sizeof CTp-C %d\n",sizeof(CTp.Tmp));
	printf("sizeof CTp-C %d\n",sizeof(CTp.C_));
	printf("sizeof CTp-C %d\n",sizeof(CTp.sigma));
	element_printf("CTpsigma= %B\n",CTp.Tmp);
*************************************/
	return 0;
}

