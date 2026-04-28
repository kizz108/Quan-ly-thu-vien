#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
#include <iomanip>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <ctime>
#include <utility>

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

string nhapChuoiKhongRong(const string& thongBao) {
    string value;
    do {
        cout << thongBao;
        getline(cin, value);
        if (value.empty()) {
            cout << "Khong duoc de trong. Vui long nhap lai!\n";
        }
    } while (value.empty());
    return value;
}

string trimCopy(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

vector<string> splitByPipe(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);
    while (getline(ss, field, '|')) {
        fields.push_back(trimCopy(field));
    }

    if (!line.empty() && line.back() == '|') {
        fields.push_back("");
    }

    return fields;
}

string dinhDangTien(double soTien) {
    return to_string(static_cast<long long>(soTien)) + " VND";
}

bool xacNhanNopPhat(double soTienPhat) {
    cout << "Tong tien phat: " << dinhDangTien(soTienPhat) << '\n';

    int chon;
    do {
        cout << "Xac nhan nop phat:\n";
        cout << "1. Da nop phat\n";
        cout << "2. Chua nop phat\n";
        chon = nhapSoNguyen("Chon: ", 1);

        if (chon == 1) {
            return true;
        }

        if (chon == 2) {
            return false;
        }

        cout << "Lua chon khong hop le. Vui long chon lai!\n";
    } while (true);
}

double tinhTienPhatTreHan(int soNgayTre) {
    if (soNgayTre <= 0) {
        return 0.0;
    }
    // Phat 20k cho moi ngay neu tre <= 9 ngay
    // Phat 50k cho moi ngay neu tre >= 10 ngay
    return soNgayTre <= 9 ? soNgayTre * 20000.0 : soNgayTre * 50000.0;
}

bool laNamNhuan(int nam) {
    return (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
}

int soNgayTrongThang(int thang, int nam) {
    switch (thang) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return laNamNhuan(nam) ? 29 : 28;
    default:
        return 0;
    }
}

bool tachNgay(const string& ngay, int& d, int& m, int& y) {
    string temp = ngay;
    replace(temp.begin(), temp.end(), '-', '/');

    stringstream ss(temp);
    char c1, c2;
    if (!(ss >> d >> c1 >> m >> c2 >> y)) {
        return false;
    }

    if (c1 != '/' || c2 != '/') {
        return false;
    }

    if (y < 1900 || m < 1 || m > 12) {
        return false;
    }

    int maxDay = soNgayTrongThang(m, y);
    return d >= 1 && d <= maxDay;
}

int ngayToNumber(const string& ngay) {
    int d, m, y;
    if (!tachNgay(ngay, d, m, y)) {
        return -1;
    }
    return y * 10000 + m * 100 + d;
}

int ngayToSerialDay(const string& ngay) {
    int d, m, y;
    if (!tachNgay(ngay, d, m, y)) {
        return -1;
    }

    int total = 0;
    for (int nam = 1900; nam < y; nam++) {
        total += laNamNhuan(nam) ? 366 : 365;
    }

    for (int thang = 1; thang < m; thang++) {
        total += soNgayTrongThang(thang, y);
    }

    return total + d;
}

int khoangCachNgay(const string& tuNgay, const string& denNgay) {
    int start = ngayToSerialDay(tuNgay);
    int end = ngayToSerialDay(denNgay);
    if (start == -1 || end == -1) {
        return -1;
    }
    return end - start;
}

int ngayHienTaiToNumber() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    if (local == nullptr) {
        return 0;
    }
    return (local->tm_year + 1900) * 10000 + (local->tm_mon + 1) * 100 + local->tm_mday;
}

string ngayHienTaiString() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    if (local == nullptr) {
        return "khong xac dinh";
    }

    int d = local->tm_mday;
    int m = local->tm_mon + 1;
    int y = local->tm_year + 1900;

    string result;
    if (d < 10) result += "0";
    result += to_string(d) + "/";
    if (m < 10) result += "0";
    result += to_string(m) + "/" + to_string(y);
    return result;
}

string nhapNgayHopLe(const string& thongBao) {
    string ngay;
    while (true) {
        cout << thongBao;
        cin >> ngay;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int d, m, y;
        if (tachNgay(ngay, d, m, y)) {
            return ngay;
        }

        cout << "Ngay khong hop le. Nhap theo dang dd/mm/yyyy.\n";
    }
}

string chonGioiTinh() {
    int chon;
    do {
        cout << "Gioi tinh:\n";
        cout << "1. Nam\n";
        cout << "2. Nu\n";
        chon = nhapSoNguyen("Chon: ", 1);

        if (chon == 1) return "nam";
        if (chon == 2) return "nu";

        cout << "Lua chon khong hop le. Vui long chon lai!\n";
    } while (true);
}

vector<string> danhSachTheLoai() {
    return {
        "Kinh te",
        "Khoa hoc/Ki thuat",
        "Lich su",
        "Van hoc",
        "Tam ly hoc",
        "Tai lieu tham khao"
    };
}

string chonTheLoai() {
    vector<string> ds = danhSachTheLoai();
    int chon;

    do {
        cout << "The loai sach:\n";
        for (int i = 0; i < static_cast<int>(ds.size()); i++) {
            cout << i + 1 << ". " << ds[i] << '\n';
        }

        chon = nhapSoNguyen("Chon: ", 1);
        if (chon >= 1 && chon <= static_cast<int>(ds.size())) {
            return ds[chon - 1];
        }

        cout << "Lua chon khong hop le. Vui long chon lai!\n";
    } while (true);
}

string chonTinhTrangSach() {
    int chon;
    do {
        cout << "Tinh trang sach:\n";
        cout << "1. Con\n";
        cout << "2. Mat\n";
        chon = nhapSoNguyen("Chon: ", 1);

        if (chon == 1) return "con";
        if (chon == 2) return "mat";

        cout << "Lua chon khong hop le. Vui long chon lai!\n";
    } while (true);
}

class DocGia {
protected:
    string maDG, ten, gioiTinh, sdt, email, diaChi;
    int tuoi{};

public:
    DocGia() = default;
    virtual ~DocGia() = default;

    string getMaDG() const { return maDG; }
    string getTen() const { return ten; }
    string getGioiTinh() const { return gioiTinh; }
    int getTuoi() const { return tuoi; }
    string getSdt() const { return sdt; }
    string getEmail() const { return email; }
    string getDiaChi() const { return diaChi; }

    void setMaDG(const string& ma) { maDG = ma; }

    void setThongTin(const string& ma, const string& tenMoi, int tuoiMoi, const string& gioiTinhMoi,
                     const string& sdtMoi, const string& emailMoi, const string& diaChiMoi) {
        maDG = ma;
        ten = tenMoi;
        tuoi = tuoiMoi;
        gioiTinh = gioiTinhMoi;
        sdt = sdtMoi;
        email = emailMoi;
        diaChi = diaChiMoi;
    }

    virtual string getLoaiDocGia() const = 0;
    virtual int getSoSachMuonToiDa() const = 0;
    virtual int getSoNgayMuonToiDa() const = 0;

    virtual void nhapThongTin() {
        ten = nhapChuoiKhongRong("Ten: ");
        tuoi = nhapSoNguyen("Tuoi: ", 0);
        gioiTinh = chonGioiTinh();
        sdt = nhapChuoiKhongRong("SDT: ");
        email = nhapChuoiKhongRong("Email: ");
        diaChi = nhapChuoiKhongRong("Dia chi: ");
    }

    virtual void capNhat() {
        cout << "Nhap thong tin moi cho doc gia:\n";
        nhapThongTin();
    }

    virtual void xuat() const {
        cout << left
             << setw(12) << maDG
             << setw(16) << getLoaiDocGia()
             << setw(24) << ten
             << setw(8) << tuoi
             << setw(12) << gioiTinh
             << setw(16) << sdt
             << setw(30) << email
             << setw(26) << diaChi
             << setw(10) << getSoSachMuonToiDa()
             << setw(12) << getSoNgayMuonToiDa()
             << '\n';
    }

    virtual string toFileLine() const {
        stringstream ss;
        ss << getLoaiDocGia() << '|'
           << maDG << '|'
           << ten << '|'
           << tuoi << '|'
           << gioiTinh << '|'
           << sdt << '|'
           << email << '|'
           << diaChi;
        return ss.str();
    }
};

class DocGiaThuong : public DocGia {
public:
    string getLoaiDocGia() const override { return "Thuong"; }
    int getSoSachMuonToiDa() const override { return 3; }
    int getSoNgayMuonToiDa() const override { return 14; }
};

class DocGiaVIP : public DocGia {
public:
    string getLoaiDocGia() const override { return "VIP"; }
    int getSoSachMuonToiDa() const override { return 10; }
    int getSoNgayMuonToiDa() const override { return 30; }
};

class DocGiaSinhVien : public DocGia {
public:
    string getLoaiDocGia() const override { return "Sinh vien"; }
    int getSoSachMuonToiDa() const override { return 5; }
    int getSoNgayMuonToiDa() const override { return 21; }
};

int chonLoaiDocGia() {
    int chon;
    do {
        cout << "Loai doc gia:\n";
        cout << "1. Doc gia thuong\n";
        cout << "2. Doc gia VIP\n";
        cout << "3. Doc gia sinh vien\n";
        chon = nhapSoNguyen("Chon: ", 1);

        if (chon >= 1 && chon <= 3) {
            return chon;
        }

        cout << "Lua chon khong hop le. Vui long chon lai!\n";
    } while (true);
}

unique_ptr<DocGia> taoDocGiaTheoLoai(int loai) {
    switch (loai) {
    case 1: return unique_ptr<DocGia>(new DocGiaThuong());
    case 2: return unique_ptr<DocGia>(new DocGiaVIP());
    case 3: return unique_ptr<DocGia>(new DocGiaSinhVien());
    default: return unique_ptr<DocGia>(new DocGiaThuong());
    }
}

unique_ptr<DocGia> taoDocGiaTheoLoai(const string& loaiRaw) {
    string loai = toLowerCopy(trimCopy(loaiRaw));
    if (loai == "thuong") {
        return unique_ptr<DocGia>(new DocGiaThuong());
    }

    if (loai == "vip") {
        return unique_ptr<DocGia>(new DocGiaVIP());
    }

    if (loai == "sinh vien" || loai == "sinhvien") {
        return unique_ptr<DocGia>(new DocGiaSinhVien());
    }

    return nullptr;
}

class Sach {
private:
    string maSach, tenSach, theLoai;
    double giaTien{};
    int soLuong{};

public:
    Sach() = default;

    Sach(const string& ma, const string& ten, const string& loai,
     double gia, int sl) : maSach(ma), tenSach(ten), theLoai(loai), giaTien(gia), soLuong(sl) {}

    string getMaSach() const { return maSach; }
    string getTenSach() const { return tenSach; }
    string getTheLoai() const { return theLoai; }
    double getGiaTien() const { return giaTien; }
    int getSoLuong() const { return soLuong; }

    void setMaSach(const string& ma) { maSach = ma; }

    void setThongTin(const string& ma, const string& ten, const string& loai, double gia, int sl) {
        maSach = ma;
        tenSach = ten;
        theLoai = loai;
        giaTien = gia;
        soLuong = sl;
    }

    void tangSoLuong(int x) {
        if (x > 0) {
            soLuong += x;
        }
    }

    bool giamSoLuong() {
        if (soLuong <= 0) {
            return false;
        }
        soLuong--;
        return true;
    }

    void nhapThongTinMoi() {
        tenSach = nhapChuoiKhongRong("Ten sach: ");
        theLoai = chonTheLoai();
        giaTien = nhapSoThuc("Gia tien: ", 0.0);
        soLuong = nhapSoNguyen("So luong: ", 0);
    }

    void capNhat() {
        cout << "Cap nhat sach: chi sua gia tien va so luong.\n";
        giaTien = nhapSoThuc("Gia tien moi: ", 0.0);
        soLuong = nhapSoNguyen("So luong moi: ", 0);
    }

    void xuat() const {
        string giaTienHienThi = to_string(static_cast<long long>(giaTien)) + " VND";
        cout << left
             << setw(12) << maSach
             << setw(40) << tenSach
             << setw(30) << theLoai
             << setw(18) << giaTienHienThi
             << setw(10) << soLuong
             << '\n';
    }

    string toFileLine() const {
        stringstream ss;
        ss << maSach << '|'
           << tenSach << '|'
           << theLoai << '|'
           << static_cast<long long>(giaTien) << '|'
           << soLuong;
        return ss.str();
    }
};

class PhieuMuon {
private:
    string maSach, maDG, ngayMuon, ngayHenTra;

public:
    PhieuMuon(const string& ms, const string& mdg, const string& nm, const string& nht)
        : maSach(ms), maDG(mdg), ngayMuon(nm), ngayHenTra(nht) {}

    string getMaSach() const { return maSach; }
    string getMaDG() const { return maDG; }
    string getNgayMuon() const { return ngayMuon; }
    string getNgayHenTra() const { return ngayHenTra; }

    bool biTreHan() const {
        int ngayHenTraNumber = ngayToNumber(ngayHenTra);
        int today = ngayHienTaiToNumber();
        return ngayHenTraNumber != -1 && today != 0 && ngayHenTraNumber < today;
    }

    bool biTreHanKhiTra(const string& ngayTraThucTe) const {
        int ngayTraNumber = ngayToNumber(ngayTraThucTe);
        int ngayHenTraNumber = ngayToNumber(ngayHenTra);
        return ngayTraNumber != -1 && ngayHenTraNumber != -1 && ngayTraNumber > ngayHenTraNumber;
    }

    void xuat() const {
        cout << left
             << setw(12) << maDG
             << setw(12) << maSach
             << setw(14) << ngayMuon
             << setw(14) << ngayHenTra
             << '\n';
    }
};

struct LichSuMuonTra {
    string maDG;
    string tenDocGia;
    string maSach;
    string tenSach;
    string ngayMuon;
    string ngayHenTra;
    string ngayTraThucTe;
    string tinhTrangSach;
    int soNgayTre;            // so ngay tre hen tra de thong ke
    int soNgayTreQuaQuyDinh;  // so ngay vuot qua han muc cho phep de tinh phat
    double tienPhatTreHan;
    double tienPhatMatSach;
    double tongTienPhat;
};

// Enum xac dinh tieu chi sap xep
enum SortCriteria {
    NONE = 0,
    MA_DOC_GIA = 1,
    GIOI_TINH = 2,
    LOAI_DOC_GIA = 3
};

// Struct de luu trang thai sap xep
struct SortState {
    SortCriteria criteria = NONE;
    bool ascending = true;  // true = tang, false = giam
};

class ThuVien {
private:
    vector<unique_ptr<DocGia>> dsDocGia;
    vector<Sach> dsSach;
    vector<PhieuMuon> dsMuon;
    vector<LichSuMuonTra> dsLichSu;
    SortState sortState;  // Luu trang thai sap xep hien tai

public:
    ThuVien() {
        dsSach = {
            Sach("101", "Dac Nhan Tam", "Tam ly hoc", 80000, 5),
            Sach("102", "Tu duy nhanh va cham", "Tam ly hoc", 120000, 3),
            Sach("103", "Kinh te hoc vi mo", "Kinh te", 150000, 4),
            Sach("104", "Nguyen ly ke toan", "Kinh te", 130000, 6),
            Sach("105", "Lap trinh C++", "Khoa hoc/Ki thuat", 200000, 7),
            Sach("106", "Tri tue nhan tao", "Khoa hoc/Ki thuat", 250000, 2),
            Sach("107", "Lich su Viet Nam", "Lich su", 90000, 5),
            Sach("108", "The gioi co dai", "Lich su", 110000, 3),
            Sach("109", "Toi thay hoa vang tren co xanh", "Van hoc", 95000, 6),
            Sach("110", "Truyen Kieu", "Van hoc", 70000, 4)
        };

        dsDocGia.push_back(make_unique<DocGiaThuong>());
        dsDocGia.back()->setMaDG("DG001");
        dsDocGia.back()->setThongTin("DG001", "Nguyen Van Anh", 28, "nam", "0901234567", "anh.nguyen@example.com", "Ha Noi");

        dsDocGia.push_back(make_unique<DocGiaVIP>());
        dsDocGia.back()->setMaDG("DG002");
        dsDocGia.back()->setThongTin("DG002", "Tran Thi Minh", 35, "nu", "0912345678", "minh.tran@example.com", "Ho Chi Minh");

        dsDocGia.push_back(make_unique<DocGiaSinhVien>());
        dsDocGia.back()->setMaDG("DG003");
        dsDocGia.back()->setThongTin("DG003", "Le Van Nam", 21, "nam", "0987654321", "nam.le@student.edu.vn", "Da Nang");

        dsDocGia.push_back(make_unique<DocGiaThuong>());
        dsDocGia.back()->setMaDG("DG004");
        dsDocGia.back()->setThongTin("DG004", "Pham Thi Hong", 30, "nu", "0945678123", "hong.pham@example.com", "Can Tho");

        dsDocGia.push_back(make_unique<DocGiaVIP>());
        dsDocGia.back()->setMaDG("DG005");
        dsDocGia.back()->setThongTin("DG005", "Do Quoc Bao", 42, "nam", "0978123456", "bao.do@example.com", "Hai Phong");
    }

    void inTieuDeDocGia() const {
        cout << left
             << setw(12) << "Ma DG"
             << setw(16) << "Loai"
             << setw(24) << "Ten"
             << setw(8) << "Tuoi"
             << setw(12) << "Gioi tinh"
             << setw(16) << "SDT"
             << setw(30) << "Email"
             << setw(26) << "Dia chi"
             << setw(10) << "Toi da"
             << setw(12) << "So ngay"
             << '\n';
        cout << string(166, '-') << '\n';
    }

    void inTieuDeSach() const {
        cout << left
             << setw(12) << "Ma sach"
             << setw(40) << "Ten sach"
             << setw(30) << "The loai"
             << setw(18) << "Gia tien"
             << setw(10) << "So luong"
             << '\n';
        cout << string(128, '-') << '\n';
    }

    void inTieuDePhieuMuon() const {
        cout << left
             << setw(12) << "Ma DG"
             << setw(12) << "Ma sach"
             << setw(30) << "Ngay muon"
             << setw(30) << "Hen tra"
             << '\n';
        cout << string(128, '-') << '\n';
    }

    int timViTriDocGia(const string& ma) const {
        for (int i = 0; i < static_cast<int>(dsDocGia.size()); i++) {
            if (dsDocGia[i]->getMaDG() == ma) {
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

    int timViTriPhieuMuon(const string& maDG, const string& maSach) const {
        for (int i = 0; i < static_cast<int>(dsMuon.size()); i++) {
            if (dsMuon[i].getMaDG() == maDG && dsMuon[i].getMaSach() == maSach) {
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

    int demSachDangMuonCuaDocGia(const string& maDG) const {
        int count = 0;
        for (const auto& pm : dsMuon) {
            if (pm.getMaDG() == maDG) {
                count++;
            }
        }
        return count;
    }

    bool sachDangDuocMuon(const string& maSach) const {
        for (const auto& pm : dsMuon) {
            if (pm.getMaSach() == maSach) {
                return true;
            }
        }
        return false;
    }

    string layTenDocGia(const string& maDG) const {
        int pos = timViTriDocGia(maDG);
        return pos == -1 ? "Khong ro" : dsDocGia[pos]->getTen();
    }

    string layTenSach(const string& maSach) const {
        int pos = timViTriSach(maSach);
        return pos == -1 ? "Khong ro" : dsSach[pos].getTenSach();
    }

    // Cac ham sap xep
    void sortByMaDG(bool ascending) {
        sort(dsDocGia.begin(), dsDocGia.end(),
             [ascending](const unique_ptr<DocGia>& a, const unique_ptr<DocGia>& b) {
                 int cmp = a->getMaDG().compare(b->getMaDG());
                 return ascending ? cmp < 0 : cmp > 0;
             });
    }

    void sortByGioiTinh(bool ascending) {
        sort(dsDocGia.begin(), dsDocGia.end(),
             [ascending](const unique_ptr<DocGia>& a, const unique_ptr<DocGia>& b) {
                 int cmp = a->getGioiTinh().compare(b->getGioiTinh());
                 return ascending ? cmp < 0 : cmp > 0;
             });
    }

    void sortByLoaiDocGia(bool ascending) {
        sort(dsDocGia.begin(), dsDocGia.end(),
             [ascending](const unique_ptr<DocGia>& a, const unique_ptr<DocGia>& b) {
                 int cmp = a->getLoaiDocGia().compare(b->getLoaiDocGia());
                 return ascending ? cmp < 0 : cmp > 0;
             });
    }

public:
    void xemDocGia() {
        cout << "\n===== DANH SACH DOC GIA =====\n";
        if (dsDocGia.empty()) {
            cout << "Danh sach doc gia dang rong.\n";
            return;
        }

        // Menu chon tieu chi sap xep
        int chon;
        do {
            cout << "\nTieu chi sap xep:\n";
            cout << "1. Sap xep theo Ma doc gia\n";
            cout << "2. Sap xep theo Gioi tinh\n";
            cout << "3. Sap xep theo Loai doc gia\n";
            cout << "0. Khong sap xep\n";
            chon = nhapSoNguyen("Chon: ", 0);

            if (chon >= 0 && chon <= 3) {
                break;
            }
            cout << "Lua chon khong hop le. Vui long chon lai!\n";
        } while (true);

        // Neu chon cung tieu chi nhu lan truoc, dao chieu
        SortCriteria selectedCriteria = static_cast<SortCriteria>(chon);
        if (selectedCriteria != NONE) {
            if (sortState.criteria == selectedCriteria) {
                sortState.ascending = !sortState.ascending;
            } else {
                sortState.criteria = selectedCriteria;
                sortState.ascending = true;  // Tieu chi moi, bat dau tu tang
            }

            // Thuc hien sap xep
            switch (sortState.criteria) {
            case MA_DOC_GIA:
                sortByMaDG(sortState.ascending);
                cout << "Sap xep theo Ma doc gia (" << (sortState.ascending ? "tang" : "giam") << ")\n";
                break;
            case GIOI_TINH:
                sortByGioiTinh(sortState.ascending);
                cout << "Sap xep theo Gioi tinh (" << (sortState.ascending ? "tang" : "giam") << ")\n";
                break;
            case LOAI_DOC_GIA:
                sortByLoaiDocGia(sortState.ascending);
                cout << "Sap xep theo Loai doc gia (" << (sortState.ascending ? "tang" : "giam") << ")\n";
                break;
            default:
                break;
            }
        }

        inTieuDeDocGia();
        for (const auto& dg : dsDocGia) {
            dg->xuat();
        }
    }

    void themDocGia() {
        cout << "\n===== THEM DOC GIA =====\n";
        string ma = nhapChuoiKhongRong("Ma doc gia: ");

        if (timViTriDocGia(ma) != -1) {
            cout << "Trung ma doc gia. Khong the them!\n";
            return;
        }

        unique_ptr<DocGia> dg = taoDocGiaTheoLoai(chonLoaiDocGia());
        dg->setMaDG(ma);
        dg->nhapThongTin();
        dsDocGia.push_back(move(dg));
        cout << "Them doc gia thanh cong!\n";
    }

    void capNhatDocGia() {
        cout << "\n===== CAP NHAT DOC GIA =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma doc gia can cap nhat: ");
        int pos = timViTriDocGia(ma);

        if (pos == -1) {
            cout << "Khong tim thay doc gia!\n";
            return;
        }

        dsDocGia[pos]->capNhat();
        cout << "Cap nhat doc gia thanh cong!\n";
    }

    void xoaDocGia() {
        cout << "\n===== XOA DOC GIA =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma doc gia can xoa: ");
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

    void timKiemDocGia() const {
        cout << "\n===== TIM KIEM DOC GIA =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma doc gia can tim: ");
        int pos = timViTriDocGia(ma);

        if (pos == -1) {
            cout << "Khong tim thay doc gia!\n";
            return;
        }

        inTieuDeDocGia();
        dsDocGia[pos]->xuat();
    }

    void nhapDocGiaTuFile() {
        cout << "\n===== NHAP DOC GIA TU FILE TXT =====\n";
        string tenFile = nhapChuoiKhongRong("Nhap duong dan file txt: ");
        ifstream fin(tenFile);
        if (!fin) {
            cout << "Khong mo duoc file!\n";
            return;
        }

        int them = 0, capNhat = 0, boQua = 0;
        string line;
        while (getline(fin, line)) {
            line = trimCopy(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            vector<string> f = splitByPipe(line);
            if (!f.empty() && toLowerCopy(f[0]) == "loai") {
                continue;
            }

            if (f.size() != 8) {
                boQua++;
                continue;
            }

            try {
                unique_ptr<DocGia> dg = taoDocGiaTheoLoai(f[0]);
                if (!dg || f[1].empty() || f[2].empty()) {
                    boQua++;
                    continue;
                }

                int tuoi = stoi(f[3]);
                if (tuoi < 0) {
                    boQua++;
                    continue;
                }

                string gioiTinh = toLowerCopy(f[4]);
                if (gioiTinh == "nam") {
                    gioiTinh = "nam";
                } else if (gioiTinh == "nu") {
                    gioiTinh = "nu";
                }

                dg->setThongTin(f[1], f[2], tuoi, gioiTinh, f[5], f[6], f[7]);
                int pos = timViTriDocGia(f[1]);
                if (pos == -1) {
                    dsDocGia.push_back(move(dg));
                    them++;
                } else {
                    dsDocGia[pos] = move(dg);
                    capNhat++;
                }
            } catch (...) {
                boQua++;
            }
        }

        cout << "Nhap doc gia hoan tat. Them: " << them
             << ", cap nhat: " << capNhat
             << ", bo qua: " << boQua << ".\n";
    }

    void xuatDocGiaRaFile() const {
        cout << "\n===== XUAT DOC GIA RA FILE TXT =====\n";
        string tenFile = nhapChuoiKhongRong("Nhap duong dan file txt: ");
        ofstream fout(tenFile);
        if (!fout) {
            cout << "Khong tao duoc file!\n";
            return;
        }

        fout << "Loai|MaDG|Ten|Tuoi|GioiTinh|SDT|Email|DiaChi\n";
        for (const auto& dg : dsDocGia) {
            fout << dg->toFileLine() << '\n';
        }

        cout << "Da xuat " << dsDocGia.size() << " doc gia ra file.\n";
    }

    void xemSach() const {
        cout << "\n===== DANH SACH SACH =====\n";
        if (dsSach.empty()) {
            cout << "Danh sach sach dang rong.\n";
            return;
        }

        inTieuDeSach();
        for (const auto& s : dsSach) {
            s.xuat();
        }
    }

    void themSach() {
        cout << "\n===== THEM SACH =====\n";
        string ma = nhapChuoiKhongRong("Ma sach: ");
        int pos = timViTriSach(ma);

        if (pos != -1) {
            cout << "Ma sach da ton tai. Khong tao sach moi.\n";
            int soLuongThem = nhapSoNguyen("Nhap so luong muon tang them: ", 1);
            dsSach[pos].tangSoLuong(soLuongThem);
            cout << "Da tang so luong sach thanh cong!\n";
            return;
        }

        Sach s;
        s.setMaSach(ma);
        s.nhapThongTinMoi();
        dsSach.push_back(s);
        cout << "Them sach thanh cong!\n";
    }

    void capNhatSach() {
        cout << "\n===== CAP NHAT SACH =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma sach can cap nhat: ");
        int pos = timViTriSach(ma);

        if (pos == -1) {
            cout << "Khong tim thay sach!\n";
            return;
        }

        dsSach[pos].capNhat();
        cout << "Cap nhat sach thanh cong!\n";
    }

    void xoaSach() {
        cout << "\n===== XOA SACH =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma sach can xoa: ");
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
        cout << "\n===== TIM KIEM SACH THEO MA =====\n";
        string ma = nhapChuoiKhongRong("Nhap ma sach can tim: ");
        int pos = timViTriSach(ma);

        if (pos == -1) {
            cout << "Khong tim thay sach!\n";
            return;
        }

        inTieuDeSach();
        dsSach[pos].xuat();
    }

    void timKiemSachTheoTheLoai() const {
        cout << "\n===== TIM KIEM SACH THEO THE LOAI =====\n";
        string theLoai = chonTheLoai();
        bool found = false;

        for (const auto& s : dsSach) {
            if (toLowerCopy(s.getTheLoai()) == toLowerCopy(theLoai)) {
                if (!found) {
                    inTieuDeSach();
                }
                s.xuat();
                found = true;
            }
        }

        if (!found) {
            cout << "Khong co sach thuoc the loai nay!\n";
        }
    }

    void nhapSachTuFile() {
        cout << "\n===== NHAP SACH TU FILE TXT =====\n";
        string tenFile = nhapChuoiKhongRong("Nhap duong dan file txt: ");
        ifstream fin(tenFile);
        if (!fin) {
            cout << "Khong mo duoc file!\n";
            return;
        }

        int them = 0, capNhat = 0, boQua = 0;
        string line;
        while (getline(fin, line)) {
            line = trimCopy(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            vector<string> f = splitByPipe(line);
            if (!f.empty() && toLowerCopy(f[0]) == "masach") {
                continue;
            }

            if (f.size() != 5 || f[0].empty() || f[1].empty()) {
                boQua++;
                continue;
            }

            try {
                double giaTien = stod(f[3]);
                int soLuong = stoi(f[4]);
                if (giaTien < 0 || soLuong < 0) {
                    boQua++;
                    continue;
                }

                Sach sach(f[0], f[1], f[2], giaTien, soLuong);
                int pos = timViTriSach(f[0]);
                if (pos == -1) {
                    dsSach.push_back(sach);
                    them++;
                } else {
                    dsSach[pos] = sach;
                    capNhat++;
                }
            } catch (...) {
                boQua++;
            }
        }

        cout << "Nhap sach hoan tat. Them: " << them
             << ", cap nhat: " << capNhat
             << ", bo qua: " << boQua << ".\n";
    }

    void xuatSachRaFile() const {
        cout << "\n===== XUAT SACH RA FILE TXT =====\n";
        string tenFile = nhapChuoiKhongRong("Nhap duong dan file txt: ");
        ofstream fout(tenFile);
        if (!fout) {
            cout << "Khong tao duoc file!\n";
            return;
        }

        fout << "MaSach|TenSach|TheLoai|GiaTien|SoLuong\n";
        for (const auto& s : dsSach) {
            fout << s.toFileLine() << '\n';
        }

        cout << "Da xuat " << dsSach.size() << " sach ra file.\n";
    }

    void muonSach() {
        cout << "\n===== MUON SACH =====\n";
        string maSach = nhapChuoiKhongRong("Ma sach: ");
        string maDG = nhapChuoiKhongRong("Ma doc gia: ");

        int posSach = timViTriSach(maSach);
        int posDG = timViTriDocGia(maDG);

        if (posSach == -1 || posDG == -1) {
            cout << "Sai ma sach hoac ma doc gia. Khong lap phieu muon!\n";
            return;
        }

        if (dsSach[posSach].getSoLuong() <= 0) {
            cout << "Sach da het. Khong the muon!\n";
            return;
        }

        if (timViTriPhieuMuon(maDG, maSach) != -1) {
            cout << "Doc gia nay dang muon cuon sach nay. Khong lap trung phieu!\n";
            return;
        }

        int soSachDangMuon = demSachDangMuonCuaDocGia(maDG);
        int soSachMuonToiDa = dsDocGia[posDG]->getSoSachMuonToiDa();
        if (soSachDangMuon >= soSachMuonToiDa) {
            cout << "Doc gia " << dsDocGia[posDG]->getLoaiDocGia()
                 << " chi duoc muon toi da " << soSachMuonToiDa << " sach.\n";
            return;
        }

        string ngayMuon = nhapNgayHopLe("Ngay muon (dd/mm/yyyy): ");
        string ngayHenTra = nhapNgayHopLe("Ngay tra du kien (dd/mm/yyyy): ");

        if (ngayToNumber(ngayHenTra) < ngayToNumber(ngayMuon)) {
            cout << "Ngay tra du kien khong duoc nho hon ngay muon!\n";
            return;
        }

        int soNgayMuon = khoangCachNgay(ngayMuon, ngayHenTra);
        int soNgayMuonToiDa = dsDocGia[posDG]->getSoNgayMuonToiDa();
        if (soNgayMuon > soNgayMuonToiDa) {
            cout << "Doc gia " << dsDocGia[posDG]->getLoaiDocGia()
                 << " chi duoc muon toi da " << soNgayMuonToiDa << " ngay.\n";
            return;
        }

        dsSach[posSach].giamSoLuong();
        dsMuon.push_back(PhieuMuon(maSach, maDG, ngayMuon, ngayHenTra));
        cout << "Lap phieu muon thanh cong. So luong sach da giam 1.\n";
    }

    void traSach() {
        cout << "\n===== TRA SACH =====\n";
        string maDG = nhapChuoiKhongRong("Ma doc gia: ");
        string maSach = nhapChuoiKhongRong("Ma sach: ");

        int posMuon = timViTriPhieuMuon(maDG, maSach);
        if (posMuon == -1) {
            cout << "Khong tim thay phieu muon hop le!\n";
            return;
        }

        int posSach = timViTriSach(maSach);
        if (posSach == -1) {
            cout << "Khong tim thay sach trong danh muc!\n";
            return;
        }

        string ngayTraThucTe = nhapNgayHopLe("Ngay tra thuc te (dd/mm/yyyy): ");
        const PhieuMuon& phieuMuon = dsMuon[posMuon];
        if (ngayToNumber(ngayTraThucTe) < ngayToNumber(phieuMuon.getNgayMuon())) {
            cout << "Ngay tra thuc te khong duoc nho hon ngay muon!\n";
            return;
        }

        string tinhTrang = chonTinhTrangSach();
        double tienPhatTreHan = 0.0;
        double tienPhatMatSach = 0.0;

        int posDG = timViTriDocGia(phieuMuon.getMaDG());
        int soNgayTre = max(0, khoangCachNgay(phieuMuon.getNgayHenTra(), ngayTraThucTe));
        int soNgayTreQuaQuyDinh = 0;
        if (posDG != -1) {
            int ngayMuonSerial = ngayToSerialDay(phieuMuon.getNgayMuon());
            int ngayTraSerial = ngayToSerialDay(ngayTraThucTe);
            int soNgayMuonToiDa = dsDocGia[posDG]->getSoNgayMuonToiDa();
            int ngayMuonToiDaSerial = ngayMuonSerial + soNgayMuonToiDa - 1;
            if (ngayTraSerial > ngayMuonToiDaSerial) {
                soNgayTreQuaQuyDinh = ngayTraSerial - ngayMuonToiDaSerial;
            }
        }

        if (soNgayTreQuaQuyDinh > 0) {
            tienPhatTreHan = tinhTienPhatTreHan(soNgayTreQuaQuyDinh);
            cout << "Tra sach vuot qua han muc cho phep " << soNgayTreQuaQuyDinh
                 << " ngay. Phat tre han: " << dinhDangTien(tienPhatTreHan) << '\n';
        } else if (soNgayTre > 0) {
            cout << "Tra sach muon qua hen tra du kien nhung van trong quyen muon toi da. Khong bi phat tre han." << '\n';
        }

        if (tinhTrang == "mat") {
            tienPhatMatSach = dsSach[posSach].getGiaTien();
            cout << "Sach bi mat. Phat mat sach (gia goc): "
                 << dinhDangTien(tienPhatMatSach) << '\n';
        }

        double tienPhat = tienPhatTreHan + tienPhatMatSach;
        if (tienPhat > 0) {
            cout << "Tong tien phat: " << dinhDangTien(tienPhat) << '\n';
            if (!xacNhanNopPhat(tienPhat)) {
                cout << "Chua nop phat. Khong the hoan tat tra sach!\n";
                return;
            }
        }

        if (tinhTrang == "con") {
            dsSach[posSach].tangSoLuong(1);
            cout << "Sach con tot. Da tang lai so luong sach.\n";
        } else {
            cout << "Sach bi mat. Khong tang lai so luong sach.\n";
        }

        LichSuMuonTra lichSu;
        lichSu.maDG = phieuMuon.getMaDG();
        lichSu.tenDocGia = layTenDocGia(phieuMuon.getMaDG());
        lichSu.maSach = phieuMuon.getMaSach();
        lichSu.tenSach = layTenSach(phieuMuon.getMaSach());
        lichSu.ngayMuon = phieuMuon.getNgayMuon();
        lichSu.ngayHenTra = phieuMuon.getNgayHenTra();
        lichSu.ngayTraThucTe = ngayTraThucTe;
        lichSu.tinhTrangSach = tinhTrang;
        lichSu.soNgayTre = soNgayTre;
        lichSu.soNgayTreQuaQuyDinh = soNgayTreQuaQuyDinh;
        lichSu.tienPhatTreHan = tienPhatTreHan;
        lichSu.tienPhatMatSach = tienPhatMatSach;
        lichSu.tongTienPhat = tienPhat;
        dsLichSu.push_back(lichSu);

        dsMuon.erase(dsMuon.begin() + posMuon);
        cout << "Lap phieu tra sach thanh cong!\n";
    }

    void xemPhieuDangMuon() const {
        cout << "\n===== DANH SACH SACH DANG MUON =====\n";
        if (dsMuon.empty()) {
            cout << "Khong co sach dang muon.\n";
            return;
        }

        inTieuDePhieuMuon();
        for (const auto& pm : dsMuon) {
            pm.xuat();
        }
    }

    void xemLichSuMuonTra() const {
        cout << "\n===== LICH SU MUON TRA =====\n";
        if (dsLichSu.empty()) {
            cout << "Chua co giao dich tra sach nao.\n";
            return;
        }

        cout << left
             << setw(12) << "Ma DG"
             << setw(24) << "Ten doc gia"
             << setw(12) << "Ma sach"
             << setw(30) << "Ten sach"
             << setw(14) << "Ngay muon"
             << setw(14) << "Hen tra"
             << setw(14) << "Ngay tra"
             << setw(10) << "Tre"
             << setw(16) << "Phat tre han"
             << setw(16) << "Phat mat sach"
             << setw(16) << "Tong phat"
             << '\n';
        cout << string(164, '-') << '\n';

        for (const auto& lichSu : dsLichSu) {
            cout << left
                 << setw(12) << lichSu.maDG
                 << setw(24) << lichSu.tenDocGia
                 << setw(12) << lichSu.maSach
                 << setw(30) << lichSu.tenSach
                 << setw(14) << lichSu.ngayMuon
                 << setw(14) << lichSu.ngayHenTra
                 << setw(14) << lichSu.ngayTraThucTe
                 << setw(10) << lichSu.soNgayTre
                 << setw(16) << dinhDangTien(lichSu.tienPhatTreHan)
                 << setw(16) << dinhDangTien(lichSu.tienPhatMatSach)
                 << setw(16) << dinhDangTien(lichSu.tongTienPhat)
                 << '\n';
        }
    }

    void thongKeTongHop() const {
        cout << "\n===== THONG KE TONG HOP =====\n";

        int tongDauSach = static_cast<int>(dsSach.size());
        int tongSachCon = 0;
        int dauSachCon = 0;
        int dauSachHet = 0;
        map<string, pair<int, int>> thongKeTheLoai; // first: dau sach, second: so luong con

        for (const auto& s : dsSach) {
            tongSachCon += s.getSoLuong();
            if (s.getSoLuong() > 0) {
                dauSachCon++;
            } else {
                dauSachHet++;
            }

            thongKeTheLoai[s.getTheLoai()].first++;
            thongKeTheLoai[s.getTheLoai()].second += s.getSoLuong();
        }

        int soSachDangMuon = static_cast<int>(dsMuon.size());
        int tongSachDangQuanLy = tongSachCon + soSachDangMuon;

        cout << "Tong so dau sach: " << tongDauSach << '\n';
        cout << "Tong so sach dang quan ly: " << tongSachDangQuanLy << '\n';
        cout << "So sach con trong thu vien: " << tongSachCon << '\n';
        cout << "So sach dang muon: " << soSachDangMuon << '\n';
        cout << "Dau sach con hang: " << dauSachCon << '\n';
        cout << "Dau sach khong con hang: " << dauSachHet << '\n';

        cout << "\n--- So sach theo the loai ---\n";
        if (thongKeTheLoai.empty()) {
            cout << "Chua co sach nao.\n";
        } else {
            cout << left
                 << setw(26) << "The loai"
                 << setw(14) << "Dau sach"
                 << setw(14) << "So luong con"
                 << '\n';
            cout << string(54, '-') << '\n';
            for (const auto& item : thongKeTheLoai) {
                cout << left
                     << setw(26) << item.first
                     << setw(14) << item.second.first
                     << setw(14) << item.second.second
                     << '\n';
            }
        }

        int nam = 0, nu = 0;
        map<string, int> thongKeLoaiDocGia;
        for (const auto& dg : dsDocGia) {
            string gioiTinh = toLowerCopy(dg->getGioiTinh());
            if (gioiTinh == "nam") {
                nam++;
            } else if (gioiTinh == "nu") {
                nu++;
            }
            thongKeLoaiDocGia[dg->getLoaiDocGia()]++;
        }

        double tongTienPhatThuDuoc = 0.0;
        for (const auto& lichSu : dsLichSu) {
            tongTienPhatThuDuoc += lichSu.tongTienPhat;
        }

        cout << "\nTong so doc gia: " << dsDocGia.size() << '\n';
        cout << "Doc gia nam: " << nam << '\n';
        cout << "Doc gia nu: " << nu << '\n';
        for (const auto& item : thongKeLoaiDocGia) {
            cout << "Doc gia " << item.first << ": " << item.second << '\n';
        }

        cout << "\nTong so giao dich tra sach: " << dsLichSu.size() << '\n';
        cout << "Tong tien phat da thu: " << dinhDangTien(tongTienPhatThuDuoc) << '\n';

        xemDocGiaTreHan();
    }

    void xemDocGiaTreHan() const {
        cout << "\n===== DANH SACH DOC GIA TRE HAN =====\n";
        cout << "Ngay hien tai: " << ngayHienTaiString() << '\n';

        bool found = false;
        cout << left
             << setw(12) << "Ma DG"
             << setw(24) << "Ten doc gia"
             << setw(12) << "Ma sach"
             << setw(30) << "Ten sach"
             << setw(30) << "Ngay muon"
             << setw(14) << "Hen tra"
             << '\n';
        cout << string(106, '-') << '\n';

        int todaySerial = ngayToSerialDay(ngayHienTaiString());
        for (const auto& pm : dsMuon) {
            int posDG = timViTriDocGia(pm.getMaDG());
            if (posDG == -1) {
                continue;
            }

            int ngayMuonSerial = ngayToSerialDay(pm.getNgayMuon());
            int ngayMuonToiDaSerial = ngayMuonSerial + dsDocGia[posDG]->getSoNgayMuonToiDa();
            if (todaySerial > ngayMuonToiDaSerial) {
                cout << left
                     << setw(12) << pm.getMaDG()
                     << setw(24) << layTenDocGia(pm.getMaDG())
                     << setw(12) << pm.getMaSach()
                     << setw(30) << layTenSach(pm.getMaSach())
                     << setw(14) << pm.getNgayMuon()
                     << setw(14) << pm.getNgayHenTra()
                     << '\n';
                found = true;
            }
        }

        if (!found) {
            cout << "Khong co doc gia tre han.\n";
        }
    }

    void sapXepDocGiaTheoTen() {
        sort(dsDocGia.begin(), dsDocGia.end(), [](const unique_ptr<DocGia>& a, const unique_ptr<DocGia>& b) {
            return toLowerCopy(a->getTen()) < toLowerCopy(b->getTen());
        });
        cout << "Da sap xep doc gia theo ten.\n";
        xemDocGia();
    }

    void sapXepSachTheoMaSach() {
        int chon;
        do {
            cout << "\nSap xep sach theo ma sach:\n";
            cout << "1. Tang dan\n";
            cout << "2. Giam dan\n";
            chon = nhapSoNguyen("Chon: ", 1);

            if (chon == 1) {
                sort(dsSach.begin(), dsSach.end(), [](const Sach& a, const Sach& b) {
                    return toLowerCopy(a.getMaSach()) < toLowerCopy(b.getMaSach());
                });
                cout << "Da sap xep sach theo ma sach tang dan.\n";
                break;
            }

            if (chon == 2) {
                sort(dsSach.begin(), dsSach.end(), [](const Sach& a, const Sach& b) {
                    return toLowerCopy(a.getMaSach()) > toLowerCopy(b.getMaSach());
                });
                cout << "Da sap xep sach theo ma sach giam dan.\n";
                break;
            }

            cout << "Lua chon khong hop le. Vui long chon lai!\n";
        } while (true);

        xemSach();
    }

    void sapXepSachTheoSoLuong() {
        int chon;
        do {
            cout << "\nSap xep sach theo so luong:\n";
            cout << "1. Tang dan\n";
            cout << "2. Giam dan\n";
            chon = nhapSoNguyen("Chon: ", 1);

            if (chon == 1) {
                sort(dsSach.begin(), dsSach.end(), [](const Sach& a, const Sach& b) {
                    return a.getSoLuong() < b.getSoLuong();
                });
                cout << "Da sap xep sach theo so luong tang dan.\n";
                break;
            }

            if (chon == 2) {
                sort(dsSach.begin(), dsSach.end(), [](const Sach& a, const Sach& b) {
                    return a.getSoLuong() > b.getSoLuong();
                });
                cout << "Da sap xep sach theo so luong giam dan.\n";
                break;
            }

            cout << "Lua chon khong hop le. Vui long chon lai!\n";
        } while (true);

        xemSach();
    }
};

void menuQuanLyDocGia(ThuVien& tv) {
    int chon;
    do {
        cout << "\n===== QUAN LY DOC GIA =====\n";
        cout << "1. Xem danh sach doc gia\n";
        cout << "2. Them doc gia\n";
        cout << "3. Cap nhat thong tin doc gia\n";
        cout << "4. Xoa doc gia\n";
        cout << "5. Tim kiem doc gia theo ma\n";
        cout << "6. Nhap danh sach doc gia tu file txt\n";
        cout << "7. Xuat danh sach doc gia ra file txt\n";
        cout << "0. Quay lai menu chinh\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: tv.xemDocGia(); break;
        case 2: tv.themDocGia(); break;
        case 3: tv.capNhatDocGia(); break;
        case 4: tv.xoaDocGia(); break;
        case 5: tv.timKiemDocGia(); break;
        case 6: tv.nhapDocGiaTuFile(); break;
        case 7: tv.xuatDocGiaRaFile(); break;
        case 0: break;
        default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (chon != 0);
}

void menuQuanLySach(ThuVien& tv) {
    int chon;
    do {
        cout << "\n===== QUAN LY SACH =====\n";
        cout << "1. Xem danh sach sach\n";
        cout << "2. Them sach\n";
        cout << "3. Cap nhat sach\n";
        cout << "4. Xoa sach\n";
        cout << "5. Tim kiem sach theo ma\n";
        cout << "6. Tim kiem sach theo the loai\n";
        cout << "7. Nhap danh sach sach tu file txt\n";
        cout << "8. Xuat danh sach sach ra file txt\n";
        cout << "9. Sap xep sach theo ma sach\n";
        cout << "0. Quay lai menu chinh\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: tv.xemSach(); break;
        case 2: tv.themSach(); break;
        case 3: tv.capNhatSach(); break;
        case 4: tv.xoaSach(); break;
        case 5: tv.timKiemSachTheoMa(); break;
        case 6: tv.timKiemSachTheoTheLoai(); break;
        case 7: tv.nhapSachTuFile(); break;
        case 8: tv.xuatSachRaFile(); break;
        case 9: tv.sapXepSachTheoMaSach(); break;
        case 0: break;
        default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (chon != 0);
}

void menuThongKe(ThuVien& tv) {
    int chon;
    do {
        cout << "\n===== THONG KE =====\n";
        cout << "1. Xem thong ke tong hop\n";
        cout << "2. Xem danh sach sach dang muon\n";
        cout << "3. Xem danh sach doc gia tre han\n";
        cout << "4. Sap xep doc gia theo ten\n";
        cout << "5. Sap xep sach theo ma sach\n";
        cout << "6. Sap xep sach theo so luong\n";
        cout << "0. Quay lai menu chinh\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: tv.thongKeTongHop(); break;
        case 2: tv.xemPhieuDangMuon(); break;
        case 3: tv.xemDocGiaTreHan(); break;
        case 4: tv.sapXepDocGiaTheoTen(); break;
        case 5: tv.sapXepSachTheoMaSach(); break;
        case 6: tv.sapXepSachTheoSoLuong(); break;
        case 0: break;
        default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (chon != 0);
}

void menuChinh() {
    ThuVien tv;
    int chon;

    do {
        cout << "\n========== QUAN LY THU VIEN ==========" << '\n';
        cout << "1. Quan ly doc gia\n";
        cout << "2. Quan ly sach\n";
        cout << "3. Muon sach\n";
        cout << "4. Tra sach\n";
        cout << "5. Thong ke\n";
        cout << "0. Thoat\n";
        chon = nhapSoNguyen("Chon: ", 0);

        switch (chon) {
        case 1: menuQuanLyDocGia(tv); break;
        case 2: menuQuanLySach(tv); break;
        case 3: tv.muonSach(); break;
        case 4: tv.traSach(); break;
        case 5: menuThongKe(tv); break;
        case 0: cout << "Thoat chuong trinh.\n"; break;
        default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (chon != 0);
}

int main() {
    menuChinh();
    return 0;
}
