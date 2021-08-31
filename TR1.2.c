#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>


char file_name[] = "list_data.txt";
char delim[] = ";";
char data_barang[200][3][200];
int jml_data_barang = 0;

char *substring(char *string, int position, int length) {
   	char *pointer;
   	int c;
 
   	pointer = malloc(length+1);
   
   	if (pointer == NULL) {
      	printf("Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
   	}
 
   	for (c = 0 ; c < length ; c++) {
      	*(pointer+c) = *(string+position-1);      
      	string++;  
   	}
 
   	*(pointer+c) = '\0';
 
   	return pointer;
}

void getData() {
	FILE *fp = fopen(file_name, "r");
    char line[200];
	
	memset(data_barang,0,sizeof(data_barang));
    jml_data_barang = 0;  
	   
	if (fp == NULL) {
		printf("Gagal membuka file %s", file_name);
		exit(EXIT_FAILURE);
	} else {
		int i = 0;
		while (fgets(line, sizeof(line), fp) != NULL) {
			char *token = strtok(line, delim);
    		
    		int j = 0;
			while (token != NULL) {
				strcpy(data_barang[i][j], token);
				token = strtok(NULL, delim);
				j++;
			}
			
			i++;
			jml_data_barang++;
		}
	}
	fclose(fp);
}

void viewData() {
    getData();
    system("@cls||clear");
    
    printf("============================================\n");
   	printf("-----------Menu Lihat Data Barang-----------\n");
   	SetColor(7);
   	printf("============================================\n");
   	
    if (jml_data_barang > 0) {
    	SetColor(8);
        printf("Nama Barang\tHarga\tStok\n");
        SetColor(7);
   		printf("============================================\n");
        int i;
		for (i = 0; i < jml_data_barang; i++) {
			char nama_brg[100];
			strcpy(nama_brg, data_barang[i][0]);
			if (strlen(nama_brg) > 15) {
		   		strcpy(nama_brg, substring(nama_brg, 1, 15));
			} else if (strlen(nama_brg) < 8) {
		   		snprintf(nama_brg, sizeof nama_brg, "%s%s", data_barang[i][0], "\t");
			}
            int harga_brg = atoi(data_barang[i][1]);
            int stok_brg = atoi(data_barang[i][2]);

            printf("%s\t%d\t%d \n",nama_brg,harga_brg,stok_brg);
        }
    } else {
        printf("Tidak ada data barang!\n");
    }
    printf("============================================\n");
    system("pause");
    menu_utama();
}

bool addToFile(char inputan[]) {
	FILE *fp = fopen(file_name,"a+");
	if (fp == NULL) {
		return false;
	} else {
		fputs(inputan, fp);
	}
	fclose(fp);
	return true;
}

void tambahData() {
	scanf("%c");
	system("@cls||clear");
	
	printf("============================================\n");
	SetColor(8);
   	printf("-----------Menu Tambah Data Barang----------\n");
   	SetColor(7);
   	printf("============================================\n\n");
   	
   	
   	char nama[100], harga[50], stok[50], hasil[200];
   	
	printf("Masukkan Nama Barang : ");
   	gets(nama);
   	if (strlen(nama) > 15) {
   		strcpy(nama, substring(nama, 1, 15));
	}
	
   	printf("Masukkan Harga Barang : ");
   	gets(harga);
   	int hargaa = atoi(harga);
   	if (hargaa < 1000) hargaa = 1000;
   	else if (hargaa > 9999999) hargaa = 9999999;
   	
   	printf("Masukkan Stok Barang : ");
   	gets(stok);
   	int stokk = atoi(stok);
   	if (stokk < 1) stokk = 1;
   	else if (stokk > 10000) stokk = 10000;
   	
   	printf("\n============================================\n");
	snprintf(hasil, sizeof hasil, "%s;%d;%d;\n", nama, hargaa, stokk);
	if (addToFile(hasil)) {
	printf("---------Sukses tambah data barang!---------\n");
	} else {
		printf("Gagal tambah data barang!\n");
	}
	printf("============================================\n");
	system("pause");
    menu_utama();
}

void hapusData() {
	scanf("%c");
	system("@cls||clear");
	
	printf("============================================\n");
	SetColor(8);
   	printf("-----------Menu Hapus Data Barang-----------\n");
   	SetColor(7);
   	printf("============================================\n\n");
   	
   	char nama[100];
   	printf("Masukkan Nama Barang : ");
   	gets(nama);
	if (jml_data_barang > 0) {
		FILE *fp = fopen(file_name, "r");
	    char line[200];
		   
		if (fp == NULL) {
			printf("Gagal membuka file %s", file_name);
			exit(EXIT_FAILURE);
		} else {
			bool ada = false, ada2 = false;
			int i = 0, j = 0, k = 0;
			char tampung[100][200];
			while (fgets(line, sizeof(line), fp) != NULL) {
				char *token = strtok(line, delim);
	    		j = 0;
				ada = false;
				while (token != NULL) {
					if (j == 0) {
						if (strcmp(data_barang[i][j],nama) == 0) {
							ada = true;
							ada2 = true; //memastikan jika ketemu
						}
					}
					token = strtok(NULL, delim);
					j++;
				}
				
				if (!ada) {
					//data barang yg bukan dihapus akan ditampung di array tampung
					snprintf(tampung[k], sizeof tampung[k], "%s;%s;%s;\n", data_barang[i][0], data_barang[i][1], data_barang[i][2]);
					k++;
				}
				
				i++;
			}
			
			fclose(fp);
			
			printf("\n============================================\n");
			if (ada2) {
				fp = fopen(file_name, "w"); //reset file jadi kosong
				fclose(fp);
				if (k > 0) {
					for (i = 0; i < k; i++) {
						addToFile(tampung[i]);
					}	
				}
				printf("--Sukses hapus (%s) dari data barang!--\n",nama);
			} else {
				printf("Nama barang (%s) tidak ditemukan!\n",nama);
			}
		}
   	} else {
   		printf("============================================\n");
   		printf("Tidak ada data barang!\n");
    }
    
    printf("============================================\n");
    system("pause");
    menu_utama();
}


void beli(){
	system("@cls||clear");
	int dalam, luar,lampu,knalpot,busi,blok;
	int uang, kembalian, total;
	
	printf("================SELAMAT DATANG===============\n");
	SetColor(8);
	printf("==============INGIN MEMBELI APA?=============\n\n");
	SetColor(7);
	
	/*printf("Ingin membeli berapa barang : ");
	scanf("%d", &barang);
	printf("No\t\t Nama Barang\t\t Jumlah\n");
	for(i=0;i<barang;i++){
		scanf("%i", &i);
		scanf("%s", &dalam[i]);
		scanf("%i", &jumlah);
	}*/
	
	printf("Masukan jumlah Ban dalam 		: ");
		scanf("%d",&dalam);
	printf("Masukan jumlah beli Ban luar 		: ");
		scanf("%d",&luar);
	printf("Masukan jumlah beli lampu panjang 	: ");
		scanf("%d",&lampu);
	printf("Masukan jumlah beli knalpot motor 	: ");
		scanf("%d",&knalpot);
	printf("masukan jumlah beli busi motor 		: ");
		scanf("%d", &busi);
	printf ("Masukan jumlah beli blok motor 		: ");
		scanf("%d", &blok);
		printf("\n");
	
	total=(dalam*45000)+(luar*100000)+(lampu*20000)+(knalpot*150000)+(busi*15000)+(blok*120000);
	
	printf("Masukan Uang tunai : Rp ");
	scanf("%d",&uang);
	
	system("@cls||clear");
	struct tm *Sys_T;

	printf("====================STRUK===================\n");
	
	//untuk mencetak datetime
    time_t Tval;
    Tval = time(NULL);
    Sys_T = localtime(&Tval); /* Untuk mendapatkan waktu lokal komputer */
    /* Sys_T = gmtime(&Tval); / / Untuk mendapatkan waktu komputer berbasis GMT/UTC */
 
    printf("", Sys_T->tm_mday);
    printf("", Sys_T->tm_mon+1); /* Ditambah 1 karena Januari dimulai dari 0 */
    printf("", 1900+Sys_T->tm_year); /* Ditambah 1900, karena tahun dimulai dari 1900 */
 
    printf("", Sys_T->tm_hour);
    printf("", Sys_T->tm_min);
    printf("", Sys_T->tm_sec);
 
    printf("", Sys_T->tm_wday); /* 0-6 melambangkan Minggu-Sabtu */
    printf("", Sys_T->tm_yday);
 
    printf("\t%s", asctime (Sys_T));
    /* asctime berfungsi untuk mengubah struct tm menjadi string c dengan format Www Mmm dd hh:mm:ss yyyy */
    //end datetime
	SetColor(8);
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	SetColor(7);
	
	printf("Ban dalam\t\t %d\n", dalam);
	printf("Ban luar\t\t %d\n", luar);
	printf("Lampu samping\t\t %d\n", lampu);
	printf("Knalpot\t\t\t %d\n", knalpot);
	printf("Busi Motor\t\t %d\n", busi);
	printf("Blok Motor\t\t %d\n", blok);
	
	SetColor(8);
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");
	SetColor(7);	
	printf("Uang      : Rp %d\n",uang);
	printf("Total     : Rp %d\n",total);
	kembalian=uang-total;
	
	printf("Kembalian : Rp %d\n",kembalian);
	if(kembalian<0){
		printf("============================================\n");
		SetColor(4);
		printf("---------MAAF UANG ANDA TIDAK CUKUP!--------\n\n");
		SetColor(7);
		printf("Press any key to continue...");
		
		getch();
		menu_akhir();
	}
	else{
		SetColor(7);
		printf("============================================\n");
		SetColor(7);
		printf("------SELAMAT BARANG BERHASIL DI BELI-------\n\n");
		printf("Press any key to continue...");
		
   		getch();
		menu_akhir();
	}
}


//warna text
void SetColor(unsigned short color) {
HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hConsoleOutput,color);
}
//end warna text

void menu_akhir(){
	system("@cls||clear");
	SetColor(8);
	printf("=========TERIMAKASIH========\n");
	SetColor(7);
	printf("Kami siap melayani kembali\n\n");
	printf("Press any key to continue...");
    getch();
    menu_utama();
}

void menu(int pil) {
	if (pil == 1) viewData();
	else if (pil == 2) tambahData();
	else if (pil == 3) hapusData();
	else if (pil == 4) beli();
}

void menu_utama() {
	int pilihan = 0;
	while (pilihan == 0) {
		system("@cls||clear");
		
		printf("============================================\n");
		SetColor(8);
		printf("-----------------MENU UTAMA-----------------\n");
		SetColor(7);
		printf("============================================\n\n");
		
		printf("1. Lihat Data Barang\n");
		printf("2. Tambah Data Barang\n");
		printf("3. Hapus Data Barang\n");
		printf("4. Transaksi\n");
		printf("5. Logout\n");
		printf("6. Exit\n");
		printf("Pilih Menu : ");
		scanf("%d",&pilihan);
		if (pilihan <= 0 || pilihan > 6) {
			pilihan = 0;
			system("@cls||clear");
			printf("Pilihan Menu Salah!");
			system("pause");
		}
	}
	
	if (pilihan == 5) login();
	else if (pilihan == 6) exit(EXIT_SUCCESS);
	else menu(pilihan);
}

void login() {
	system("@cls||clear");
	
	
	printf("=======SELAMAT DATANG======\n");
	SetColor(8);
	printf("-----------LOGIN-----------\n\n");
	char username[20], password[20];
	
	SetColor(7);
	printf("Username : ");
	gets(username);
	printf("Password : ");
	gets(password);
	
	system("cls");
    gotoxy(50,15);
    printf("%s - telah Login",username);
    getch();

}

//animasi loading
void gotoxy(int x,int y);
void loading(int x,int y);
gotoxy(int x,int y){
	
	HANDLE handle;
	COORD coord;
	coord.X =x;
	coord.Y =y;
	handle  = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle,coord);
}

void loading(int x,int y){
	system("cls");
	int i;
	gotoxy(x,y-1);printf("Loading...");
	
	
	for(i=x;i<=x+19;i++){
	    gotoxy(i,y);printf("%c",196);
		gotoxy(i,y+1);printf("%c",196);	
	}
	for(i=y;i<=y+1;i++){
	    gotoxy(x,i);printf("%c",179);
		gotoxy(x+19,i);printf("%c",179);	
	}
	gotoxy(x,y);printf("%c",218);
	gotoxy(x+19,y);printf("%c",191);
	gotoxy(x,y+1);printf("%c",192);
	gotoxy(x+19,y+1);printf("%c",217);
	for ( i = x; i<=x+19;i++){
		gotoxy(i,y);printf("%c",220);
		gotoxy(i,y+1);printf("%c",223);
		Sleep(20);
	}
	system("cls");
}
//end animasi loading

void main() {
	loading(30,12);
	getData();
	login();
	loading(30,12);
	menu_utama();
	menu_akhir();
}
