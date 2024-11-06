#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>

#define SIZE 50
//file data memiliki 100 entitas, jika ingin menggunakan semua ubah SIZE menjadi 100

struct mahasiswa {
    std::string nama;
    std::string nim;    
    float ipk;
};

mahasiswa data[SIZE];

void menerimaInput(mahasiswa(&data)[SIZE]);
bool stringValid(std::string s);
bool NIMValid(std::string s);
bool ipkValid(std::string ip);
void tampilkanInfo(mahasiswa(&data)[SIZE], bool filterLulus = false);
bool lulus(mahasiswa m);
void printLine(mahasiswa &cetak);
void menerimaInputDariFile(mahasiswa(&data)[SIZE], const std::string& filename);



int main() {

    //menerimaInput(data);
    //menerimaInput untuk memasukkan input secara manual
    menerimaInputDariFile(data, "data.csv");
    //isi file data.csv : nama,nim,ipk (100 entitas)
    std::cout << "-------------------------DATA SEMUA MAHASISWA---------------------\n";    
    tampilkanInfo(data);
    std::cout << "\n\n";
    std::cout << "-------------------------DATA MAHASISWA LULUS---------------------\n";    
    tampilkanInfo(data, true);
    std::cout << "\n\n";    

}

void menerimaInput(mahasiswa(&data)[SIZE]) {

    std::vector<std::string> question = {
        "Nama             : ",
        "NIM              : ",        
        "IPK              : "
    };

    std::vector<std::string> marahinUser = {
        "Nama tidak boleh mengandung simbol aneh",
        "NIM hanya berupa angka antara 10 dan 20 digit",        
        "IPK antara 0.0 sampai 4.0"
    };

    std::string buffer;
    bool stringTidakValid;
    bool NIMTidakValid;    
    bool IPKTidakValid;

    for (int i = 0; i < SIZE; i++) {

        std::cout << "Masukkan Data Mahasiswa ke-" << (i + 1) << " : \n";

        //input nama mahasiswa        
        do {
            std::cout << question[0];
            std::getline(std::cin, buffer);
            stringTidakValid = !stringValid(buffer);
            if (stringTidakValid) {
                std::cout << std::endl << marahinUser[0] << std::endl;
            }

        } while (stringTidakValid);
        data[i].nama = buffer;

        //input nim mahasiswa
        do {
            std::cout << question[1];
            std::getline(std::cin, buffer);
            NIMTidakValid = !NIMValid(buffer);
            if (NIMTidakValid) {
                std::cout << std::endl << marahinUser[1] << std::endl;
            }
        } while (NIMTidakValid);
        data[i].nim = buffer;

        //input ipk mahasiswa
        do {
            std::cout << question[2];
            std::getline(std::cin, buffer);
            IPKTidakValid = !ipkValid(buffer);
            if (IPKTidakValid) {
                std::cout << std::endl << marahinUser[2] << std::endl;
            }
        } while (IPKTidakValid);
        data[i].ipk = round(std::stof(buffer) * 100) / 100;

        std::cout << "\n\n";

    }

}

bool stringValid(std::string s) {
    std::vector<char> allowedChar = {
        '.',    //titik
        ',',    //koma
        '\'',   //single quote
        ' ',    //spasi
        '-',    //dash
        '_'     //underscore
    };
    for (char c : s) {
        if (!isalnum(c) && std::find(allowedChar.begin(), allowedChar.end(), c) == allowedChar.end()) {
            return false;
        }
    }
    return true;
}

bool NIMValid(std::string s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool ipkValid(std::string ip) {
    try {
        float ipkTmp = std::stof(ip);
        return (ipkTmp >= 0.0 && ipkTmp <= 4.0);
    }
    catch (...) {
        return false;
    }
}

void tampilkanInfo(mahasiswa(&data)[SIZE], bool filterLulus) {
    std::cout << "+--------------------------------+-----------------------+-------+" << std::endl;
    std::cout << "| Nama                           | NIM                   |  IPK  |" << std::endl;
    std::cout << "+--------------------------------+-----------------------+-------+" << std::endl;

    for (auto& cetak : data) {
        if (filterLulus) {
            if (lulus(cetak)) {
                printLine(cetak);
            }
        }
        else {
            printLine(cetak);
        }
    }

    std::cout << "+--------------------------------+-----------------------+-------+" << std::endl;
}

bool lulus(mahasiswa m) {
    if (m.ipk < 2.75) return false;
    return true;
}

void printLine(mahasiswa &cetak) {
    std::string namaSingkat = cetak.nama.length() > 30 ? cetak.nama.substr(0, 27) + "..." : cetak.nama;
    std::cout << "| " << std::setw(31) << std::left << namaSingkat
        << "| " << std::setw(22) << cetak.nim
        << "| " << std::setw(5) << cetak.ipk << " |" << std::endl;
}

void menerimaInputDariFile(mahasiswa(&data)[SIZE], const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return;
    }

    std::string line;
    int i = 0;
    std::getline(file, line);

    while (std::getline(file, line) && i < SIZE) {
        std::istringstream iss(line);
        std::string nama, nim, ipkStr;

        std::getline(iss, nama, ',');
        std::getline(iss, nim, ',');
        std::getline(iss, ipkStr, ',');

        if (stringValid(nama) && NIMValid(nim) && ipkValid(ipkStr)) {
            data[i].nama = nama;
            data[i].nim = nim;
            data[i].ipk = std::stof(ipkStr);
            i++;
        }
    }

    file.close();
}
