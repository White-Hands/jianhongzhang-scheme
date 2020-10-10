#include<stdio.h>
#include<stdlib.h>
#include<pbc.h>
#include "pbc_inshort.h"
#define  Number_of_attribute 10

struct PublicKey {
	element_t g1;
	element_t g2;
	element_t h_hat;
	element_t h1tow_hat[Number_of_attribute];
	element_t g1_a;
	element_t eg1h_alpha;
	element_t h;
};

struct MasterSecertKey {
	element_t h_;
	element_t a;
	element_t alpha;
	element_t beta;
	element_t h1tow[Number_of_attribute];
};



struct UserPrivateKey {
	element_t sk1;
	element_t sk2;
	element_t sk1_rho[Number_of_attribute];
	element_t zeta;
	element_t L;
};

struct CipherText {
	element_t C;
	element_t C_;
	element_t sigma;
	element_t Ci[Number_of_attribute];
	element_t Ci_[Number_of_attribute];
};


struct TransformationKey {
	element_t sk1_zeta;
	element_t sk2_zeta;
	element_t sk1_rho_zeta[Number_of_attribute];
};

struct PartialDecryptionCiphertext{
	element_t C;
	element_t C_;
	element_t sigma;
	element_t Tmp;
};


void SystemInitiaion(struct PublicKey * PK, struct MasterSecertKey * msk, pairing_t pairing);

void KeyGen (struct UserPrivateKey * skW_, struct PublicKey *PK, struct MasterSecertKey *msk, pairing_t pairing); 

void Encryption (struct CipherText * CT,struct PublicKey *PK, struct MasterSecertKey *msk, pairing_t pairing) ;

void TransformationKeyGeneration (struct TransformationKey * TK, struct UserPrivateKey * skW_, pairing_t pairing) ;

void PartialDecrypt(struct PartialDecryptionCiphertext *CTp, struct CipherText * CT, struct TransformationKey * TK, struct PublicKey * PK, pairing_t pairing);

void Decrypt(struct UserPrivateKey * sk, struct PartialDecryptionCiphertext * CTp, pairing_t pairing);
