#include"jianhong-zhang.h"

void SystemInitiaion(struct PublicKey * PK, struct MasterSecertKey * msk, pairing_t pairing){
	/*init msk*/
	G2(msk->h_,pairing);
	for(int i=0;i<Number_of_attribute;i++){
		G2(msk->h1tow[i],pairing);
	}
	Zr(msk->a,pairing);
	Zr(msk->alpha,pairing);
	Zr(msk->beta,pairing);


	/*init PK*/
	G1(PK->g1,pairing);
	G2(PK->g2,pairing);
	G1(PK->h_hat,pairing);
	for(int i=0;i<Number_of_attribute;i++){
		G1(PK->h1tow_hat[i],pairing);
	}
	GT(PK->eg1h_alpha,pairing);
	element_pairing(PK->eg1h_alpha,PK->g1,msk->h_);
	element_pow_zn(PK->eg1h_alpha,PK->eg1h_alpha,msk->alpha);

	G1(PK->g1_a,pairing);
	element_pow_zn(PK->g1_a,PK->g1,msk->a);
	
	G2(PK->h,pairing);
	element_pow_zn(PK->h,PK->g2,msk->a);
	element_pow_zn(PK->h,PK->h,msk->beta);

	puts("System Initiation Succeed!");
}


void Encryption (struct CipherText * CT,struct PublicKey *PK, struct MasterSecertKey *msk, pairing_t pairing) {
	/*init fi and secret value T about attribute encryption*/
	element_t fi[Number_of_attribute];
	for(int i=0;i<Number_of_attribute;i++){
		Zr(fi[i],pairing);
	}
	element_t T;
	Zr(T,pairing);
	element_set0(T);
	/* T = Sum(fi) */
	for(int i=0;i<Number_of_attribute;i++){
		element_add(T,T,fi[i]);
	}

	GT(CT->C,pairing);
	element_t M;
	GT(M,pairing);	// M is a random member in GT

	element_pow_zn(CT->C,PK->eg1h_alpha,T);
	element_mul(CT->C,CT->C,M);
	
	G1(CT->C_,pairing);
	element_pow_zn(CT->C_,PK->h_hat,T);

	/*CT->Ci and CT->Ci_*/

	element_t temp1;
	Zr(temp1,pairing);
	for(int i=0;i<Number_of_attribute;i++){
		G1(CT->Ci[i],pairing);
		G1(CT->Ci_[i],pairing);

		element_mul(temp1,msk->a,fi[i]);
		element_pow_zn(CT->Ci[i],PK->g1,temp1);

		element_pow_zn(CT->Ci_[i],PK->h1tow_hat[i],fi[i]);

	}

	element_t gamma;
	Zr(gamma,pairing);
	element_from_hash(gamma,M,sizeof(M));
//	element_printf("M in encryption\n%B",M);

	Zr(CT->sigma,pairing);
	element_from_hash(CT->sigma,CT->C,sizeof(element_t));
	element_pow_zn(CT->sigma,CT->sigma,gamma);
	puts("Encryption succeed!");

}


void TransformationKeyGeneration (struct TransformationKey * TK, struct UserPrivateKey * skW_, pairing_t pairing) {
	Zr(skW_->zeta,pairing);

	/*init sk1_zeta and sk2_zeta*/
	G2(TK->sk1_zeta,pairing);
	G2(TK->sk2_zeta,pairing);
	element_pow_zn(TK->sk1_zeta,skW_->sk1,skW_->zeta);
	element_pow_zn(TK->sk2_zeta,skW_->sk2,skW_->zeta);

	/*init sk1_zeta_rho*/
	for(int i=0;i<Number_of_attribute;i++) {
		G2(TK->sk1_rho_zeta[i],pairing);
		element_pow_zn(TK->sk1_rho_zeta[i],TK->sk1_rho_zeta[i],skW_->zeta);
	}
	puts("transformation key generation succeed!");
}


void PartialDecrypt(struct PartialDecryptionCiphertext *CTp, struct CipherText * CT, struct TransformationKey * TK, struct PublicKey * PK, pairing_t pairing){
	GT(CTp->C,pairing);	
	G1(CTp->C_,pairing);	
	Zr(CTp->sigma,pairing);	
	element_set(CTp->C,CT->C);
	element_set(CTp->C_,CT->C_);
	element_set(CTp->sigma,CT->sigma);
	GT(CTp->Tmp,pairing);
	element_set1(CTp->Tmp);


	element_t temp_up;
	element_t temp_down;
	element_t temp_Tmp[Number_of_attribute];
	GT(temp_up,pairing);
	GT(temp_down,pairing);
	for(int i=0;i<Number_of_attribute;i++){
		element_pairing(temp_up,CT->Ci[i],TK->sk1_rho_zeta[i]);
		element_pairing(temp_down,CT->Ci_[i],PK->h);
		GT(temp_Tmp[i],pairing);
		element_div(temp_Tmp[i],temp_up,temp_down);

	}
	for(int i=0;i<Number_of_attribute;i++){
		element_mul(CTp->Tmp,CTp->Tmp,temp_Tmp[i]);

	}
	puts("partial decryption succeed!");
}

void Decrypt(struct UserPrivateKey * sk, struct PartialDecryptionCiphertext * CTp, pairing_t pairing){
	element_t Tmp_hat;
	GT(Tmp_hat,pairing);
	element_t temp_down2;
	GT(temp_down2,pairing);

	element_t zeta_invert;
	Zr(zeta_invert,pairing);

	element_invert(zeta_invert,sk->zeta);
	element_pow_zn(Tmp_hat,CTp->Tmp,zeta_invert);

	element_pairing(temp_down2,CTp->C_,sk->sk1);

	element_div(Tmp_hat,Tmp_hat,temp_down2);

	element_t L_invert;
	Zr(L_invert,pairing);
	element_invert(L_invert,sk->L);

	element_t M;
	GT(M,pairing);
	element_pow_zn(M,Tmp_hat,L_invert);
/**************************************************************************
	element_printf("Tmp_hat_L-1 = %B\n",M);
****************************************************************************/
	element_mul(M,M,CTp->C);

	element_t gamma_;
	element_t sigma_;
	Zr(gamma_,pairing);
	Zr(sigma_,pairing);

	element_from_hash(gamma_,M,sizeof(M));
	element_from_hash(sigma_,CTp->C,sizeof(element_t));
	element_pow_zn(sigma_,sigma_,gamma_);

	if(element_cmp(sigma_,CTp->sigma))
		puts("Succeed decryption");
	else
		puts("decryption failed!");
//	element_printf("M in decryption\n%B\n",M);
}


void KeyGen (struct UserPrivateKey * skW_, struct PublicKey *PK, struct MasterSecertKey *msk, pairing_t pairing) {
	element_t r;
	Zr(r,pairing);
	Zr(skW_->L,pairing);

	/*init sk1 and sk2*/
	G2(skW_->sk1,pairing);
	G2(skW_->sk2,pairing);
	element_pow_zn(skW_->sk2,PK->g2,r);
	element_from_hash(skW_->L,skW_->sk2,sizeof(element_t));
	element_pow_zn(skW_->sk2,skW_->sk2,msk->a);
	element_pow_zn(skW_->sk1,PK->g2,msk->alpha);
	element_pow_zn(skW_->sk1,skW_->sk1,skW_->L);
	element_mul(skW_->sk1,skW_->sk1,skW_->sk2);

	element_pow_zn(skW_->sk2,PK->g2,r);

	/*init sk1_rho*/
	for(int i=0;i<Number_of_attribute;i++) {
		G2(skW_->sk1_rho[i],pairing);
		element_pow2_zn(skW_->sk1_rho[i],msk->h1tow[i],msk->beta,msk->h_,r);
	}
	puts("Key generation succeed!");
}


