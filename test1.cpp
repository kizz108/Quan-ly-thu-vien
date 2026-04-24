#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string toLowerCopy(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

string formatCurrency(long long value) {
    return to_string(value) + " VND";
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

long long nhapSoNguyenLon(const string& thongBao, long long minValue = 0) {
    long long value;
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

string nhapChuoi(const string& thongBao) {
    string value;
    cout << thongBao;
    getline(cin, value);
    return value;
}

bool parseDateDMY(const string& dateStr, tm& outDate) {
    istringstream iss(dateStr);
    int day, month, year;
    char sep1, sep2;

    if (!(iss >> day >> sep1 >> month >> sep2 >> year)) {
        return false;
    }
    if (sep1 != '/' || sep2 != '/') {
        return false;
    }
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
        return false;
    }

    tm temp = {};
    temp.tm_mday = day;
    temp.tm_mon = month - 1;
    temp.tm_year = year - 1900;
    temp.tm_hour = 0;
    temp.tm_min = 0;
    temp.tm_sec = 0;

    outDate = temp;
    return true;
}

bool isOverdueDate(const string& dueDate) {
    tm due = {};
    if (!parseDateDMY(dueDate, due)) {
        return false;
    }

    time_t dueTime = mktime(&due);

    time_t now = time(nullptr);
    tm today = *localtime(&now);
    today.tm_hour = 0;
    today.tm_min = 0;
    today.tm_sec = 0;
    time_t todayTime = mktime(&today);

    return dueTime < todayTime;
}

class DocGia {
private:
    string maDG, ten, gioiTinh, sdt, email, diaChi;
    int tuoi;

public:
    DocGia() : tuoi(0) {}

    string getMaDG() const { return maDG; }
    string getGioiTinh() const { return gioiTinh; }
    string getTen() const { return ten; }
    int getTuoi() const { return tuoi; }

    void nhap() {
        maDG = nhapChuoi("Ma doc gia: ");
        ten = nhapChuoi("Ten: ");
        tuoi = nhapSoNguyen("Tuoi: ", 0);

        int chonGT;
        do {
            cout << "Gioi tinh:\n";
            cout << "1. Nam\n";
            cout << "2. Nu\n";
            chonGT = nhapSoNguyen("Chon: ", 1);
            if (chonGT == 1) {
                gioiTinh = "nam";
            } else if (chonGT == 2) {
                gioiTinh = "nu";
            } else {
                cout << "Lua chon khong hop le!\n";
            }
        } while (chonGT != 1 && chonGT != 2);

        sdt = nhapChuoi("SDT: ");
        email = nhapChuoi("Email: ");
        diaChi = nhapChuoi("Dia chi: ");
    }

    void capNhat() {
        ten = nhapChuoi("Ten moi: ");
        tuoi = nhapSoNguyen("Tuoi moi: ", 0);

        int chonGT;
        do {
            cout << "Gioi tinh:\n";
            cout << "1. Nam\n";
            cout << "2. Nu\n";
            chonGT = nhapSoNguyen("Chon: ", 1);
            if (chonGT == 1) {
                gioiTinh = "nam";
            } else if (chonGT == 2) {
                gioiTinh = "nu";
            } else {
                cout << "Lua chon khong hop le!\n";
            }
        } while (chonGT != 1 && chonGT != 2);

        sdt = nhapChuoi("SDT moi: ");
        email = nhapChuoi("Email moi: ");
        diaChi = nhapChuoi("Dia chi moi: ");
    }

    void xuat() const {
        cout << left
             << setw(12) << maDG
             << setw(20) << ten
             << setw(8) << tuoi
             << setw(12) << gioiTinh
             << setw(14) << sdt
             << setw(28) << email
             << setw(24) << diaChi
             << '\n';
    }
};

class Sach {
private:
    string maSach, tenSach, theLoai;
    long long giaTien;
    int soLuong;

public:
    Sach() : giaTien(0), soLuong(0) {}

    string getMaSach() const { return maSach; }
    string getTenSach() const { return tenSach; }
    string getTheLoai() const { return theLoai; }
    long long getGiaTien() const { return giaTien; }
    int getSoLuong() const { return soLuong; }

    void tangSoLuong(int x) { soLuong += x; }
    void giamSoLuong() {
        if (soLuong > 0) {
            soLuong--;
        }
    }

    static string chonTheLoai() {
        int luaChon;
        do {
            cout << "The loai:\n";
            cout << "1. Kinh te\n";
            cout << "2. Khoa hoc/Ki thuat\n";
            cout << "3. Lich su\n";
            cout << "4. Van hoc\n";
            cout << "5. Tam ly hoc\n";
            cout << "6. Tai lieu tham khao\n";
            luaChon = nhapSoNguyen("Chon: ", 1);

            switch (luaChon) {
            case 1: return "Kinh te";
            case 2: return "Khoa hoc/Ki thuat";
            case 3: return "Lich su";
            case 4: return "Van hoc";
            case 5: return "Tam ly hoc";
            case 6: return "Tai lieu tham khao";
            default:
                cout << "Lua chon khong hop le!\n";
                break;
            }
        } while (true);
    }

    void nhap() {
        maSach = nhapChuoi("Ma sach: ");
        tenSach = nhapChuoi("Ten sach: ");
        theLoai = chonTheLoai();
        giaTien = nhapSoNguyenLon("Gia tien: ", 0);
        soLuong = nhapSoNguyen("So luong: ", 0);
    }

    void capNhat() {
        tenSach = nhapChuoi("Ten sach moi: ");
        theLoai = chonTheLoai();
        giaTien = nhapSoNguyenLon("Gia tien moi: ", 0);
        soLuong = nhapSoNguyen("So luong moi: ", 0);
    }

    void xuat() const {
        cout << left
             << setw(12) << maSach
             << setw(28) << tenSach
             << setw(22) << theLoai
             << setw(16) << formatCurrency(giaTien)
             << setw(10) << soLuong
             << '\n';
    }
};

class PhieuMuon {
private:
    string maSach, maDG, ngayMuon, hanTra;

public:
    PhieuMuon(string ms, string mdg, string nm, string ht)
        : maSach(ms), maDG(mdg), ngayMuon(nm), hanTra(ht) {}

    string getMaSach() const { return maSach; }
    string getMaDG() const { return maDG; }
    string getNgayMuon() const { return ngayMuon; }
    string getHanTra() const { return hanTra; }

    void xuat() const {
        cout << left
             << setw(12) << maDG
             << setw(12) << maSach
             << setw(14) << ngayMuon
             << setw(14) << hanTra
             << '\n';
    }
};

class ThuVien {
private:
    vector<DocGia> dsDocGia;
    vector<Sach> dsSach;
    vector<PhieuMuon> dsMuon;

    int timViTriDocGia(const string& ma) const {
        for (int i = 0; i < static_cast<int>(dsDocGia.size()); i++) {
            if (dsDocGia[i].getMaDG() == ma) {
                return i;
            }
        }
        return -1;
    }

    int timViTriSach(const string& ma) const {
        for (int i = 0; i < static_cast<int>(dsSach.size()); i++) {
            if (dsSach[i].getMaSach() == ma) {
                return i;
            }
        }
        return -1;
    }

    int timViTriPhieuMuon(const string& maSach, const string& maDG) const {
        for (int i = 0; i < static_cast<int>(dsMuon.size()); i++) {
            if (dsMuon[i].getMaSach() == maSach && dsMuon[i].getMaDG() == maDG) {
                return i;
            }
        }
        return -1;
    }

    bool docGiaDangMuonSach(const string& maDG) const {
        for (const auto& pm : dsMuon) {
            if (pm.getMaDG() == maDG) {
                return true;
            }
        }
        return false;
    }

    bool sachDangDuocMuon(const string& maSach) const {
        for (const auto& pm : dsMuon) {
            if (pm.getMaSach() == maSach) {
                return true;
            }
        }
        return false;
    }

    void inTieuDeDocGia() const {
        cout << left
             << setw(12) << "Ma Doc Gia"
             << setw(20) << "Ten"
             << setw(8) << "Tuoi"
             << setw(12) << "Gioi tinh"
             << setw(14) << "SDT"
             << setw(28) << "Email"
             << setw(24) << "Dia chi"
             << '\n';
        cout << string(118, '-') << '\n';
    }

    void inTieuDeSach() const {
        cout << left
             << setw(12) << "Ma sach"
             << setw(28) << "Ten sach"
             << setw(22) << "The loai"
             << setw(16) << "Gia tien"
             << setw(10) << "So luong"
             << '\n';
        cout << string(88, '-') << '\n';
    }

    void inTieuDePhieuMuon() const {
        cout << left
             << setw(12) << "Ma doc gia"
             << setw(12) << "Ma sach"
             << setw(14) << "Ngay muon"
             << setw(14) << "Han tra"
             << '\n';
        cout << string(52, '-') << '\n';
    }

public:
    void quanLyDocGia() {
        int chon;
        do {
            cout << "\n=== QUAN LY DOC GIA ===\n";
            cout << "1. Xem danh sach doc gia\n";
            cout << "2. Them doc gia\n";
            cout << "3. Cap nhat thong tin\n";
            cout << "4. Xoa doc gia\n";
            cout << "5. Tim kiem doc gia theo ma\n";
            cout << "0. Quay lai menu chinh\n";
            chon = nhapSoNguyen("Chon: ", 0);

            switch (chon) {
            case 1: xemDocGia(); break;
            case 2: themDocGia(); break;
            case 3: capNhatDocGia(); break;
            case 4: xoaDocGia(); break;
            case 5: timKiemDocGiaTheoMa(); break;
            case 0: break;
            default: cout << "Lua chon khong hop le!\n"; break;
            }
        } while (chon != 0);
    }

    void xemDocGia() const {
        if (dsDocGia.empty()) {
            cout << "Danh sach doc gia dang rong!\n";
            return;
        }

        inTieuDeDocGia();
        for (const auto& dg : dsDocGia) {
            dg.xuat();
        }
    }

    void themDocGia() {
        DocGia dg;
        dg.nhap();

        if (timViTriDocGia(dg.getMaDG()) != -1) {
            cout << "Trung ma doc gia!\n";
            return;
        }

        dsDocGia.push_back(dg);
        cout << "Them doc gia thanh cong!\n";
    }

    void capNhatDocGia() {
        string ma = nhapChuoi("Nhap ma doc gia can cap nhat: ");
        int pos = timViTriDocGia(ma);

        if (pos == -1) {
            cout << "Khong tim thay doc gia!\n";
            return;
        }

        dsDocGia[pos].capNhat();
        cout << "Cap nhat thanh cong!\n";
    }

    void xoaDocGia() {
        string ma = nhapChuoi("Nhap ma doc gia can xoa: ");
        int pos = timViTriDocGia(ma);

        if (pos == -1) {
            cout << "Khong tim thay doc gia!\n";
            return;
        }

        if (docGiaDangMuonSach(ma)) {
            cout << "Doc gia dang muon sach, khong the xoa!\n";
            return;
        }

        dsDocGia.erase(dsDocGia.begin() + pos);
        cout << "Xoa doc gia thanh cong!\n";
    }

    void timKiemDocGiaTheoMa() const {
        string ma = nhapChuoi("Nhap ma doc gia can tim: ");
        int pos = timViTriDocGia(ma);

        if (pos == -1) {
            cout << "Khong tim thay doc gia!\n";
            return;
        }

        inTieuDeDocGia();
        dsDocGia[pos].xuat();
    }

    void quanLySach() {
        int chon;
        do {
            cout << "\n=== QUAN LY SACH ===\n";
            cout << "1. Xem danh sach sach\n";
            cout << "2. Them sach\n";
            cout << "3. Cap nhat sach\n";
            cout << "4. Xoa sach\n";
            cout << "5. Tim kiem sach theo ma\n";
            cout << "6. Tim kiem sach theo the loai\n";
            cout << "0. Quay lai menu chinh\n";
            chon = nhapSoNguyen("Chon: ", 0);

            switch (chon) {
            case 1: xemSach(); break;
            case 2: themSach(); break;
            case 3: capNhatSach(); break;
            case 4: xoaSach(); break;
            case 5: timKiemSachTheoMa(); break;
            case 6: timKiemSachTheoTheLoai(); break;
            case 0: break;
            default: cout << "Lua chon khong hop le!\n"; break;
            }
        } while (chon != 0);
    }

    void xemSach() const {
        if (dsSach.empty()) {
            cout << "Danh sach sach dang rong!\n";
            return;
        }

        inTieuDeSach();
        for (const auto& s : dsSach) {
            s.xuat();
        }
    }

    void themSach() {
        Sach s;
        s.nhap();

        int pos = timViTriSach(s.getMaSach());
        if (pos != -1) {
            dsSach[pos].tangSoLuong(s.getSoLuong());
            cout << "Trung ma sach. Da cong them so luong!\n";
            return;
        }

        dsSach.push_back(s);
        cout << "Them sach thanh cong!\n";
    }

    void capNhatSach() {
        string ma = nhapChuoi("Nhap ma sach can cap nhat: ");
        int pos = timViTriSach(ma);

        if (pos == -1) {
            cout << "Khong tim thay sach!\n";
            return;
        }

        dsSach[pos].capNhat();
        cout << "Cap nhat sach thanh cong!\n";
    }

    void xoaSach() {
        string ma = nhapChuoi("Nhap ma sach can xoa: ");
        int pos = timViTriSach(ma);

        if (pos == -1) {
            cout << "Khong tim thay sach!\n";
            return;
        }

        if (sachDangDuocMuon(ma)) {
            cout << "Sach dang duoc muon, khong the xoa!\n";
            return;
        }

        dsSach.erase(dsSach.begin() + pos);
        cout << "Xoa sach thanh cong!\n";
    }

    void timKiemSachTheoMa() const {
        string ma = nhapChuoi("Nhap ma sach can tim: ");
        int pos = timViTriSach(ma);

        if (pos == -1) {
            cout << "Khong tim thay sach!\n";
            return;
        }

        inTieuDeSach();
        dsSach[pos].xuat();
    }

    void timKiemSachTheoTheLoai() const {
        string loai = Sach::chonTheLoai();
        bool timThay = false;

        inTieuDeSach();
        for (const auto& s : dsSach) {
            if (toLowerCopy(s.getTheLoai()) == toLowerCopy(loai)) {
                s.xuat();
                timThay = true;
            }
        }

        if (!timThay) {
            cout << "Khong co sach thuoc the loai nay!\n";
        }
    }

    void muonSach() {
        string maSach = nhapChuoi("Ma sach: ");
        string maDG = nhapChuoi("Ma doc gia: ");
        string ngayMuon = nhapChuoi("Ngay muon (dd/mm/yyyy): ");
        string hanTra = nhapChuoi("Ngay tra (dd/mm/yyyy): ");

        int posSach = timViTriSach(maSach);
        int posDG = timViTriDocGia(maDG);

        if (posSach == -1 || posDG == -1) {
            cout << "Sai ma sach hoac ma doc gia. Khong lap duoc phieu!\n";
            return;
        }

        if (dsSach[posSach].getSoLuong() <= 0) {
            cout << "Sach da het. Khong lap duoc phieu!\n";
            return;
        }

        dsSach[posSach].giamSoLuong();
        dsMuon.push_back(PhieuMuon(maSach, maDG, ngayMuon, hanTra));
        cout << "Lap phieu muon thanh cong!\n";
    }

    void traSach() {
        string maDG = nhapChuoi("Ma doc gia: ");
        string maSach = nhapChuoi("Ma sach: ");
        string ngayTraThucTe = nhapChuoi("Ngay tra thuc te (dd/mm/yyyy): ");
        string tinhTrang = toLowerCopy(nhapChuoi("Tinh trang sach (con/mat): "));

        int posSach = timViTriSach(maSach);
        int posDG = timViTriDocGia(maDG);
        int posMuon = timViTriPhieuMuon(maSach, maDG);

        if (posSach == -1 || posDG == -1 || posMuon == -1) {
            cout << "Khong tim thay thong tin muon tra hop le!\n";
            return;
        }

        if (tinhTrang == "con") {
            dsSach[posSach].tangSoLuong(1);
        } else if (tinhTrang != "mat") {
            cout << "Tinh trang khong hop le!\n";
            return;
        }

        dsMuon.erase(dsMuon.begin() + posMuon);
        cout << "Lap phieu tra sach thanh cong vao ngay " << ngayTraThucTe << "!\n";
    }

    void thongKe() {
        cout << "\n=== THONG KE ===\n";

        int tongSachBanSao = 0;
        int sachCon = 0;
        int sachHet = 0;
        map<string, int> soLuongTheoTheLoai;

        for (const auto& s : dsSach) {
            tongSachBanSao += s.getSoLuong();
            if (s.getSoLuong() > 0) {
                sachCon++;
            } else {
                sachHet++;
            }
            soLuongTheoTheLoai[s.getTheLoai()] += s.getSoLuong();
        }

        map<string, int> docGiaTheoGioiTinh;
        for (const auto& dg : dsDocGia) {
            docGiaTheoGioiTinh[toLowerCopy(dg.getGioiTinh())]++;
        }

        cout << "Tong so dau sach: " << dsSach.size() << '\n';
        cout << "Tong so ban sao sach: " << tongSachBanSao << '\n';
        cout << "So dau sach con hang: " << sachCon << '\n';
        cout << "So dau sach het hang: " << sachHet << '\n';

        cout << "\nSo luong sach theo the loai:\n";
        if (soLuongTheoTheLoai.empty()) {
            cout << "Chua co du lieu sach.\n";
        } else {
            for (const auto& p : soLuongTheoTheLoai) {
                cout << "- " << p.first << ": " << p.second << '\n';
            }
        }

        cout << "\nTong so doc gia: " << dsDocGia.size() << '\n';
        cout << "So doc gia theo gioi tinh:\n";
        if (docGiaTheoGioiTinh.empty()) {
            cout << "Chua co du lieu doc gia.\n";
        } else {
            for (const auto& p : docGiaTheoGioiTinh) {
                cout << "- " << p.first << ": " << p.second << '\n';
            }
        }

        cout << "\nSo sach dang muon: " << dsMuon.size() << '\n';

        cout << "\nDanh sach doc gia tre han:\n";
        bool coTreHan = false;
        inTieuDePhieuMuon();
        for (const auto& pm : dsMuon) {
            if (isOverdueDate(pm.getHanTra())) {
                pm.xuat();
                coTreHan = true;
            }
        }
        if (!coTreHan) {
            cout << "Khong co doc gia tre han.\n";
        }

        sapXepMenu();
    }

    void sapXepMenu() {
        int chon;
        do {
            cout << "\n--- SAP XEP ---\n";
            cout << "1. Sap xep doc gia theo ten (A-Z)\n";
            cout << "2. Sap xep doc gia theo tuoi (tang dan)\n";
            cout << "3. Sap xep sach theo ten (A-Z)\n";
            cout << "4. Sap xep sach theo so luong (giam dan)\n";
            cout << "0. Quay lai\n";
            chon = nhapSoNguyen("Chon: ", 0);

            switch (chon) {
            case 1:
                sort(dsDocGia.begin(), dsDocGia.end(),
                     [](const DocGia& a, const DocGia& b) {
                         return toLowerCopy(a.getTen()) < toLowerCopy(b.getTen());
                     });
                cout << "Da sap xep doc gia theo ten.\n";
                xemDocGia();
                break;
            case 2:
                sort(dsDocGia.begin(), dsDocGia.end(),
                     [](const DocGia& a, const DocGia& b) {
                         return a.getTuoi() < b.getTuoi();
                     });
                cout << "Da sap xep doc gia theo tuoi.\n";
                xemDocGia();
                break;
            case 3:
                sort(dsSach.begin(), dsSach.end(),
                     [](const Sach& a, const Sach& b) {
                         return toLowerCopy(a.getTenSach()) < toLowerCopy(b.getTenSach());
                     });
                cout << "Da sap xep sach theo ten.\n";
                xemSach();
                break;
            case 4:
                sort(dsSach.begin(), dsSach.end(),
                     [](const Sach& a, const Sach& b) {
                         return a.getSoLuong() > b.getSoLuong();
                     });
                cout << "Da sap xep sach theo so luong giam dan.\n";
                xemSach();
                break;
            case 0:
                break;
            default:
                cout << "Lua chon khong hop le!\n";
                break;
            }
        } while (chon != 0);
    }
};

void menu() {
    ThuVien tv;
    int chon;

    do {
        cout << "\n===== QUAN LY THU VIEN =====\n";
        cout << "1. Quan ly doc gia\n";
        cout << "2. Quan ly sach\n";
        cout << "3. Muon sach\n";
        cout << "4. Tra sach\n";
        cout << "5. Thong ke\n";
        cout << "0. Thoat\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: tv.quanLyDocGia(); break;
        case 2: tv.quanLySach(); break;
        case 3: tv.muonSach(); break;
        case 4: tv.traSach(); break;
        case 5: tv.thongKe(); break;
        case 0: break;
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
