/* SORU OYUNU */
/* MUSTAFA Z.Y. MAKARNACI / 19253607 */
/* YILDIZ ÇAĞAN / 19253081 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <time.h>


int menu (void);										/* Menü 													*/
void oyna (void);										/* Oyunu başlatır.											*/
void soru_ekle (void);										/* Soruları ekler.											*/
void soru_guncelle (void);									/* Soruları günceller.										*/
void soru_sil (void);										/* Soruları siler.											*/
void en_yuksek_skor (void);									/* En yüksek üç skoru gösterir.								*/
void dosyaya_kaydet (void);									/* Verileri dosyaya kaydeder.								*/
void dosyadan_yukle (void);									/* Verileri dosyadan programa yükler. 						*/
void soru_listele (void);									/* Soruları listeler. 										*/
void skor_kaydet (void);									/* Skorları dosyaya kaydeder.								*/
void skor_yukle (void);										/* Skorları dosyadan yükler.								*/
int skor_bilgi_girisi (void);									/* Skorları ekler.											*/
void skorlar (void);										/* En yükse üç skoru listeler.								*/
char ayni_mi (char d1[], char d2[], char d3[], char d4[]);					/* Şıkların birbiriyle aynı olup olmadığın kontrol eder.	*/
void sorular (void);										/* Bütün soruların listelenmesini sağlar.					*/


struct sorular{
	int soru_id;
	char soru_metni[150];
	char sec_A[40];
	char sec_B[40];
	char sec_C[40];
	char sec_D[40];
	char dogru_cevap;
} soru[100];

struct skorlar{
	char kullanici_adi[40];
	int puan;
} skor[100];

void degistir(struct skorlar *x, struct skorlar *y); 		/* Skorları küçükten büyüğe doğru sıralar. 					*/

int soru_adedi = 0;
int skor_adedi = 0;

void main(void) {

	setlocale(LC_ALL, ""); /* Türkçe karakter desteği */
	
	 /* Verileri dosyadan diziye yükler. */
	dosyadan_yukle();
		skor_yukle();
	
	int secim;
	
	do{
		secim = menu();
		printf("\n");
		switch(secim){
			case 1 : oyna();
				break;
			case 2 : soru_ekle();
				break;
			case 3 : soru_guncelle();
				break;
			case 4 : soru_sil();
				break;
			case 5 : skorlar();
				break;
			case 6 : sorular();
				break;
		}
	}while(secim != 7);
}

/* FONKSİYONLAR */

/* MENÜ */
int menu(void)
{
	int sec;
	
	printf("[1]. Oyunu oyna\n");
	printf("[2]. Soru ekle\n");
	printf("[3]. Soru güncelle\n");
	printf("[4]. Soru sil\n");
	printf("[5]. En yüksek üç skor\n");
	printf("[6]. Bütün sorular\n");
	printf("[7]. Çıkış\n");
	
	do{
		printf("Lütfen seçim yapınız [1 - 7]:");
		sec = getche() - '0';
		printf("\n");
	}while(sec < 1 || sec > 7);

	return sec;
}

/* OYUN */
void oyna(void)
{
	int random, i, k_no;
	int puan = 0;
	char cevap;
	
	if(soru_adedi == 0){
		printf("Hiç soru yok!\nLütfen soru ekleyin.\n");
		printf("Ana menüye dönmek için bir tuşa basın:");
		getch();
		printf("\n\n");
		return;
	}
	
	k_no = skor_bilgi_girisi();
	if(k_no == -1) return;
	puan = skor[k_no].puan;
	
	for(i=0; i<5; i++){
	
		srand(time(NULL));
		random = 0 + (rand() % soru_adedi);
		
		printf("%s?\n", soru[random].soru_metni);
		printf("A) %-15s", soru[random].sec_A);
		printf("B) %-15s\n", soru[random].sec_B);
		printf("C) %-15s", soru[random].sec_C);
		printf("D) %-15s\n", soru[random].sec_D);
		printf("Cevabınız[A-B-C-D]:");
		
		don:
		cevap = toupper(getchar());
		getchar();
		if(cevap =='A' || cevap == 'B' || cevap == 'C' || cevap == 'D'){
			if(soru[random].dogru_cevap == cevap){
				puan++;
				printf("\nBildiniz! Anlık puan:%d\n\n", puan);
			}
			else{
				puan--;
				if(puan < 0) puan = 0;
				printf("\nBilemediniz! Anlık puan:%d\n\n", puan);
			}
		}
		else{
			printf("\nCevap[A-B-C-D] şıklarından biri olmalıdır:");
			goto don;
		}
	}
	
	printf("Toplam puan: %d", puan);
	skor[k_no].puan = puan;
	skor_kaydet();
	
	printf("\n");
	
	skorlar();
	
	printf("Devam etmek istiyor musunuz [E/H]:");
	if(toupper(getche()) == 'E') oyna();
	
	system("cls"); /* Ekranı temizler */
}

/* SORU EKLEME */
void soru_ekle(void)
{
	int i;
	char cevap;
	
	for(i = soru_adedi; i<=soru_adedi; i++){
		printf("Soru id: %d\n", i);
		soru[i].soru_id = i;
		
		printf("Soruyu yazın (çıkış için Enter tuşuna basın):");
		gets(soru[i].soru_metni);
		if(!*soru[i].soru_metni) break; /* Girilen değer boş ise döngüyü bitirir. */
		do{
				printf("A şıkkını girin:");
				gets(soru[i].sec_A);
				printf("B şıkkını girin:");
				gets(soru[i].sec_B);
				printf("C şıkkını girin:");
				gets(soru[i].sec_C);
				printf("D şıkkını girin:");
				gets(soru[i].sec_D);
				
				if(ayni_mi(soru[i].sec_A, soru[i].sec_B, soru[i].sec_C, soru[i].sec_D) == '0') break;
				
				printf("Şıklar birbirinden farklı olmalıdır!\n");
			}while(1);
			
		printf("Doğru cevap şıkkını girin [A-B-C-D]:");
		soru[i].dogru_cevap = toupper(getchar());
		getchar();
		
		printf("\nEklendi.\n");
		soru_adedi++;
		dosyaya_kaydet(); /* Dosyaya kayıt */
	}
	
	soru_adedi = i; /* Son girilen kayıt adedini kaydeder. */
	system("cls"); /* Ekranı temizler */
}

/* SORU GÜNCELLEME */
void soru_guncelle(void)
{
	int no;
	char deger[3];
	
	if(soru_adedi == 0){
		printf("Hiç soru yok!\nLütfen soru ekleyin.\n");
		printf("Çıkış için her hangi bir tuşa basın:");
		getch();
		printf("\n\n");
		return;
	}
	
	soru_listele();
	printf("Güncellemek istediğiniz sorunun no'sunu girin Çıkış için Enter tuşuna basın:");
	
	while(1)
	{	
		
		don:
		gets(deger);
		
		if(!*deger) return;
		no = (int) atoi(deger);
		//getchar();
		
		if(no < 0 || no > soru_adedi){
			 printf("Lütfen doğru id değerini girin:");	
			 goto don;
		}
		else{
			printf("Soruyu yazın(Çıkış için Enter tuşuna basın):");
			gets(soru[no].soru_metni);
			if(!*soru[no].soru_metni) break;
			strcat(soru[no].soru_metni,"?");
			
			do{
				soru[no].soru_id=no;
				printf("A şıkkını girin:");
				gets(soru[no].sec_A);
				printf("B şıkkını girin:");
				gets(soru[no].sec_B);
				printf("C şıkkını girin:");
				gets(soru[no].sec_C);
				printf("D şıkkını girin:");
				gets(soru[no].sec_D);
				
				if(ayni_mi(soru[no].sec_A, soru[no].sec_B, soru[no].sec_C, soru[no].sec_D) == '0') break;
				
				printf("Şıklar birbirinden farklı olmalıdır!\n");
			}while(1);
			
			printf("Doğru cevap şıkkını girin [A-B-C-D]:");
			soru[no].dogru_cevap = getchar();
			printf("\nGüncellendi.\n");
			dosyaya_kaydet();
		}
	}
	system("cls");
}

/* SORU SİLME */
void soru_sil(void)
{
	int i, no;
	char deger[3];
	
	if(soru_adedi == 0){
		printf("Hiç soru yok!\nLütfen soru ekleyin.\n");
		printf("Çıkış için her hangi bir tuşa basın:");
		getch();
		printf("\n\n");
		return;
	}
	
	do{
		i = 0;
		
		/* Soruların numaralarla beraber listelenmesi */
		while(*soru[i].soru_metni){
			printf("%d) %s?\n", soru[i].soru_id, soru[i].soru_metni);
			i++;
		}
		
		buraya_don:
		
		printf("ID girin (Çıkış için Enter tuşuna basın):");
		gets(deger);
		if(!*deger) break;
		
		no = (int) atoi(deger);
		getchar();
		
		if(no < 0 || no > soru_adedi){
			printf("Lütfen doğru id değerini girin:");
			goto buraya_don;
		}
		
		/* Numarası girilen sorunun silinmesi */
		else{
			for(i = no + 1; i <= soru_adedi; i++){
				soru[i-1] = soru[i];
				soru[i].soru_id = i-1;
			}
			soru_adedi--;
			printf("Silindi!");
		}
		
		dosyaya_kaydet();
		
		printf("Devam etmek istiyor musunuz[E/H]:\n");
		if(toupper(getch()) == 'H') break;
	}while(1);
	system("cls");
}

/* SORULARI DOSYAYA KAYDEDER */
void dosyaya_kaydet(void)
{
	FILE *fp;
	
	if((fp = fopen("sorular.txt", "w")) == NULL){
		printf("Dosya açılamadı!\n");
		exit(1);
	}
	
	fwrite(&soru_adedi, sizeof soru_adedi, 1, fp);
	fwrite(soru, sizeof soru, 1, fp);
	
	fclose(fp);
}

/* SORULARI DOSYADAN DİZİYE YÜKLER */
void dosyadan_yukle(void)
{
	FILE *fp;
	
	if((fp = fopen("sorular.txt", "r")) == NULL){
		printf("Dosya açılamadı!\n");
		return;
	}
	
	fread(&soru_adedi, sizeof soru_adedi, 1, fp);	
	fread(soru, sizeof soru, 1, fp);
	
	fclose(fp);
}

/* SORULARIN LİSTELENMESİ */
void soru_listele(void)
{
	int i=0;
	printf("Kayıtlı olan sorular\n\n");
	do{
		printf("\n%d) %s = ", soru[i].soru_id, soru[i].soru_metni);
		printf("%c\n", soru[i].dogru_cevap);
		printf("A) %-30s", soru[i].sec_A);
		printf("B) %-30s", soru[i].sec_B);
		printf("c) %-30s", soru[i].sec_C);
		printf("D) %-30s", soru[i].sec_D);
		
		printf("\n");
		i++;
		
	}while(*soru[i].soru_metni);
}

/* NUMARALARIYLA BERABER BÜTÜN SORULAR */
void sorular(void)
{

	int i=0;
	
	if(soru_adedi == 0){
		printf("Hiç soru yok!\nLütfen soru ekleyin.\n");
		printf("Çıkış için her hangi bir tuşa basın:");
		printf("\n");
		getch();
		return;
	}
	
	printf("Sorular\n");
	
	while(*soru[i].soru_metni){
		printf("%d) %s\n", soru[i].soru_id, soru[i].soru_metni);
		i++;
	}
	
	printf("\n");
}

/* SKORLARIN DOSYAYA KAYDEDİLMESİ */
void skor_kaydet(void)
{
	FILE *sp;
	
	if((sp = fopen("skorlar.txt", "w")) == NULL){
		printf("Dosya açılamadı!\n");
		exit(1);
	}
	
	fwrite(&skor_adedi, sizeof skor_adedi, 1, sp);
	fwrite(skor, sizeof skor, 1, sp);
	
	fclose(sp);
}

/* SKORLARIN DOSYADAN YÜKLENMESİ */
void skor_yukle(void)
{
	FILE *sp;
	if((sp = fopen("skorlar.txt", "r")) == NULL){
		printf("Dosya açılamadı!\n");
		return;
	}
	
	fread(&skor_adedi, sizeof skor_adedi, 1, sp);
	fread(skor, sizeof skor, 1, sp);
	
	fclose(sp);
}

/* SKOR İÇİN KULLANICI ADI EKLENMESİ */
int skor_bilgi_girisi(void)
{
	int i;
	char dizi[40];

	printf("Kullanıcı adı girin (Çıkış için Enter tuşuna basın):");
	gets(dizi);
	if(!*dizi) return -1;
	
	//printf("%s\n", dizi);
	
	for(i=0; i<=skor_adedi; i++){
		if(strcmp(skor[i].kullanici_adi, dizi)==0){
			printf("Merhaba, %3s puan: %d\n\n", skor[i].kullanici_adi,skor[i].puan);
			return i;
		}
	}
	
	strcpy(skor[skor_adedi].kullanici_adi,dizi);
	printf("\nYeni kayıt yapıldı!\nMerhaba, %s\n\n",skor[skor_adedi].kullanici_adi);
	skor[skor_adedi].puan = 0;
	skor_kaydet();
	skor_adedi++;
	return skor_adedi-1;
}

/* EN YÜKSEK ÜÇ SKOR */
void skorlar(void)
{
	int i,j;
	struct skorlar temp;

	for(i=0; i<skor_adedi; i++){
		for(j=0; j<skor_adedi; j++){
			if(skor[j].puan > skor[j+1].puan){
				degistir(&skor[j], &skor[j+1]);
			}
		}
	}
	
	if(skor_adedi == 0) printf("Hiç skor yok\n");
	else{
		printf("En iyi üç skor\n\n");
		for(i=skor_adedi, j=1; i>=skor_adedi-2; i--,j++)
		printf("%d) %d - %s\n", j, skor[i].puan, skor[i].kullanici_adi);
	}
	printf("\n");
}

/* SORU ŞIKLARININ AYNI OLUP OLMADIĞININ KONTROLÜ */
char ayni_mi(char d1[], char d2[], char d3[], char d4[])
{
	if(strcasecmp(d1, d2) == 0) return '1';
	if(strcasecmp(d1, d3) == 0) return '1';
	if(strcasecmp(d1, d4) == 0) return '1';
	if(strcasecmp(d2, d3) == 0) return '1';
	if(strcasecmp(d2, d4) == 0) return '1';
	if(strcasecmp(d3, d4) == 0) return '1';
	
	return '0';
}

/* SKORLAR BUBBLE SORT */
void degistir(struct skorlar *x, struct skorlar *y)
{
	struct skorlar temp;
	temp = *x;
	*x = *y;
	*y = temp;
}
