#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
using namespace std;

string toLowerCopy(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

int nhapSoNguyen(const string& thongBao, int minValue = numeric_limits<int>::min()) {
    int value;
    while (true) {
        cout << thongBao;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Du lieu khong hop le. Vui long nhap lai!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double nhapSoThuc(const string& thongBao, double minValue = -numeric_limits<double>::max()) {
    double value;
    while (true) {
        cout << thongBao;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Du lieu khong hop le. Vui long nhap lai!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

class DocGia {
private:
    string maDG, ten, gioiTinh, sdt, email, diaChi;
    int tuoi;

public:
    DocGia() {}

    DocGia(string ma, string t, int age, string gt,
           string phone, string em, string dc) {
        maDG = ma;
        ten = t;
        tuoi = age;
        gioiTinh = gt;
        sdt = phone;
        email = em;
        diaChi = dc;
    }

    string getMaDG() const { return maDG; }
    string getGioiTinh() const { return gioiTinh; }
    string getTen() const { return ten; }

    void nhap() {
        cout << "Ma doc gia: ";
        cin >> maDG;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ten: ";
        getline(cin, ten);

        tuoi = nhapSoNguyen("Tuoi: ", 0);

        cout << "Gioi tinh: ";
        getline(cin, gioiTinh);

        cout << "SDT: ";
        getline(cin, sdt);

        cout << "Email: ";
        getline(cin, email);

        cout << "Dia chi: ";
        getline(cin, diaChi);
    }

    void xuat() {
        cout << maDG << " | " << ten << " | "
             << tuoi << " | " << gioiTinh << " | "
             << sdt << " | " << email << " | "
             << diaChi << endl;
    }

    void capNhat() {
        cout << "Ten moi: ";
        getline(cin, ten);

        tuoi = nhapSoNguyen("Tuoi moi: ", 0);

        cout << "Gioi tinh moi: ";
        getline(cin, gioiTinh);

        cout << "SDT moi: ";
        getline(cin, sdt);

        cout << "Email moi: ";
        getline(cin, email);

        cout << "Dia chi moi: ";
        getline(cin, diaChi);
    }
};

class Sach {
private:
    string maSach, tenSach, theLoai;
    double giaTien;
    int soLuong;

public:
    Sach() {}

    Sach(string ma, string ten, string loai,
         double gia, int sl) {
        maSach = ma;
        tenSach = ten;
        theLoai = loai;
        giaTien = gia;
        soLuong = sl;
    }

    string getMaSach() const { return maSach; }
    string getTheLoai() const { return theLoai; }
    int getSoLuong() const { return soLuong; }

    void tangSoLuong(int x) { soLuong += x; }
    void giamSoLuong() { if (soLuong > 0) soLuong--; }

    void nhap() {
        cout << "Ma sach: ";
        cin >> maSach;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ten sach: ";
        getline(cin, tenSach);

        cout << "The loai: ";
        getline(cin, theLoai);

        giaTien = nhapSoThuc("Gia tien: ", 0.0);
        soLuong = nhapSoNguyen("So luong: ", 0);
    }

    void xuat() {
        cout << maSach << " | " << tenSach << " | "
             << theLoai << " | " << giaTien
             << " | " << soLuong << endl;
    }

    void capNhat() {
        giaTien = nhapSoThuc("Gia tien moi: ", 0.0);
        soLuong = nhapSoNguyen("So luong moi: ", 0);
    }
};

class PhieuMuon {
private:
    string maSach, maDG, ngayMuon, ngayTra;

public:
    PhieuMuon(string ms, string mdg,
              string nm, string nt) {
        maSach = ms;
        maDG = mdg;
        ngayMuon = nm;
        ngayTra = nt;
    }

    string getMaSach() const { return maSach; }
    string getMaDG() const { return maDG; }

    void xuat() {
        cout << maDG << " | "
             << maSach << " | "
             << ngayMuon << " | "
             << ngayTra << endl;
    }
};

class ThuVien {
private:
    vector<DocGia> dsDocGia;
    vector<Sach> dsSach;
    vector<PhieuMuon> dsMuon;

public:
    bool kiemTraDocGia(string ma) {
        for (const auto& dg : dsDocGia)
            if (dg.getMaDG() == ma)
                return true;
        return false;
    }

    bool kiemTraSach(string ma) {
        for (const auto& s : dsSach)
            if (s.getMaSach() == ma)
                return true;
        return false;
    }

    bool docGiaDangMuonSach(const string& ma) {
        for (const auto& pm : dsMuon)
            if (pm.getMaDG() == ma)
                return true;
        return false;
    }

    int timViTriPhieuMuon(const string& maSach, const string& maDG) {
        for (int i = 0; i < dsMuon.size(); i++)
            if (dsMuon[i].getMaSach() == maSach && dsMuon[i].getMaDG() == maDG)
                return i;
        return -1;
    }

    int timViTriSach(string ma) {
        for (int i = 0; i < dsSach.size(); i++)
            if (dsSach[i].getMaSach() == ma)
                return i;
        return -1;
    }

    int timViTriDocGia(string ma) {
        for (int i = 0; i < dsDocGia.size(); i++)
            if (dsDocGia[i].getMaDG() == ma)
                return i;
        return -1;
    }

    void themDocGia() {
        DocGia dg;
        dg.nhap();

        if (kiemTraDocGia(dg.getMaDG())) {
            cout << "Trung ma doc gia!\n";
            return;
        }

        dsDocGia.push_back(dg);
        cout << "Them thanh cong!\n";
    }

    void xemDocGia() {
        for (auto dg : dsDocGia)
            dg.xuat();
    }

    void xoaDocGia() {
        string ma;
        cout << "Nhap ma doc gia can xoa: ";
        cin >> ma;

        int pos = timViTriDocGia(ma);
        if (pos == -1) {
            cout << "Khong tim thay!\n";
            return;
        }

        if (docGiaDangMuonSach(ma)) {
            cout << "Doc gia dang muon sach, khong the xoa!\n";
            return;
        }

        dsDocGia.erase(dsDocGia.begin() + pos);
        cout << "Da xoa!\n";
    }

    void themSach() {
        Sach s;
        s.nhap();

        int pos = timViTriSach(s.getMaSach());

        if (pos != -1) {
            dsSach[pos].tangSoLuong(s.getSoLuong());
            cout << "Trung ma -> cong them so luong!\n";
            return;
        }

        dsSach.push_back(s);
        cout << "Them sach thanh cong!\n";
    }

    void xemSach() {
        for (auto s : dsSach)
            s.xuat();
    }

    void muonSach() {
        string maSach, maDG, ngayMuon, ngayTra;

        cout << "Ma sach: ";
        cin >> maSach;

        cout << "Ma doc gia: ";
        cin >> maDG;

        cout << "Ngay muon: ";
        cin >> ngayMuon;

        cout << "Ngay tra: ";
        cin >> ngayTra;

        int posSach = timViTriSach(maSach);
        int posDG = timViTriDocGia(maDG);

        if (posSach == -1 || posDG == -1) {
            cout << "Sai ma!\n";
            return;
        }

        if (dsSach[posSach].getSoLuong() <= 0) {
            cout << "Het sach!\n";
            return;
        }

        dsSach[posSach].giamSoLuong();
        dsMuon.push_back(
            PhieuMuon(maSach, maDG, ngayMuon, ngayTra)
        );

        cout << "Muon thanh cong!\n";
    }

    void traSach() {
        string maSach, maDG, tinhTrang;

        cout << "Ma sach: ";
        cin >> maSach;

        cout << "Ma doc gia: ";
        cin >> maDG;

        cout << "Tinh trang (con/mat): ";
        cin >> tinhTrang;

        int posSach = timViTriSach(maSach);
        int posDG = timViTriDocGia(maDG);
        int posMuon = timViTriPhieuMuon(maSach, maDG);
        string tinhTrangChuanHoa = toLowerCopy(tinhTrang);

        if (posSach == -1 || posDG == -1) {
            cout << "Sai ma sach hoac ma doc gia!\n";
            return;
        }

        if (posMuon == -1) {
            cout << "Khong tim thay phieu muon hop le!\n";
            return;
        }

        if (tinhTrangChuanHoa == "con") {
            dsSach[posSach].tangSoLuong(1);
        } else if (tinhTrangChuanHoa != "mat") {
            cout << "Tinh trang khong hop le (chi nhan 'con' hoac 'mat').\n";
            return;
        }

        dsMuon.erase(dsMuon.begin() + posMuon);

        cout << "Tra sach thanh cong!\n";
    }

    void thongKe() {
        cout << "\nTong sach: " << dsSach.size();
        cout << "\nTong doc gia: " << dsDocGia.size();
        cout << "\nSo sach dang muon: " << dsMuon.size();

        int nam = 0, nu = 0, khac = 0;
        for (const auto& dg : dsDocGia) {
            string gt = toLowerCopy(dg.getGioiTinh());
            if (gt == "nam")
                nam++;
            else if (gt == "nu")
                nu++;
            else
                khac++;
        }

        cout << "\nNam: " << nam;
        cout << "\nNu: " << nu;
        cout << "\nKhac: " << khac << endl;
    }
};

void menu() {
    ThuVien tv;
    int chon;

    do {
        cout << "\n===== QUAN LY THU VIEN =====\n";
        cout << "1. Them doc gia\n";
        cout << "2. Xem doc gia\n";
        cout << "3. Xoa doc gia\n";
        cout << "4. Them sach\n";
        cout << "5. Xem sach\n";
        cout << "6. Muon sach\n";
        cout << "7. Tra sach\n";
        cout << "8. Thong ke\n";
        cout << "0. Thoat\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: tv.themDocGia(); break;
        case 2: tv.xemDocGia(); break;
        case 3: tv.xoaDocGia(); break;
        case 4: tv.themSach(); break;
        case 5: tv.xemSach(); break;
        case 6: tv.muonSach(); break;
        case 7: tv.traSach(); break;
        case 8: tv.thongKe(); break;
        default:
            cout << "Lua chon khong hop le!\n";
            break;
        }
    } while (chon != 0);
}

int main() {
    menu();
    return 0;
}