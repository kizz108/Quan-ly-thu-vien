# Quan-ly-thu-vien

Ung dung quan ly thu vien don gian:

- `Program.cs`: ung dung giao dien Windows Forms
- `main.cpp`: ban console C++ 

## Chuc nang hien co

### Quan ly doc gia
- Them, cap nhat, xoa doc gia
- Tim theo ma, ten, loai
- Sap xep theo ten, ma, gioi tinh, loai
- Nhap/xuat du lieu doc gia bang file TXT

### Quan ly sach
- Them, cap nhat, xoa sach
- Tim theo ma, ten, the loai
- Loc sach con hang / het hang
- Sap xep theo ma va so luong
- Nhap/xuat du lieu sach bang file TXT

### Muon / tra sach
- Lap phieu muon
- Gioi han so sach muon toi da theo loai doc gia
- Gioi han so ngay muon toi da theo loai doc gia
- Tra sach, tinh phat tre han va mat sach
- Xem danh sach sach dang muon
- Xem doc gia tre han
- Xem lich su giao dich

### Thong ke
- Tong hop theo the loai sach
- Tong so sach dang quan ly, dang muon, con trong kho
- Tong so doc gia theo gioi tinh va loai
- Tong ngay tre hien tai va tong tien phat da thu

## Quy tac nghiep vu

Loai doc gia:

- Thuong: toi da 3 sach, toi da 14 ngay
- VIP: toi da 10 sach, toi da 30 ngay
- Sinh vien: toi da 5 sach, toi da 21 ngay

Muc phat tre han trong ban WinForms C#:

- Tre 1-9 ngay: `20000 VND`
- Tre tu 10 ngay tro len: `50000 VND`
- Mat sach: phat bang gia tien cuon sach

## Cau truc repo

```text
.
|- Do_an_quan_ly_thu_vien.csproj
|- Quan-ly-thu-vien.sln
|- Program.cs        # WinForms C#
|- main.cpp          # Console C++
|- test.cpp          # Console C++ ban thu nghiem
|- bin/
|- obj/
```

## Yeu cau moi truong

### Ban WinForms C#
- Windows
- .NET SDK 10.0 hoac moi hon

### Ban C++
- Trinh bien dich ho tro C++
- Vi du: `g++`

## Cach chay

### 1. Chay ban WinForms C#

Build:

```powershell
dotnet build
```

Run:

```powershell
dotnet run
```
### 2. Chay ban console C++

Compile:

```powershell
g++ -std=c++17 -O2 -o main.exe main.cpp
```

Run:

```powershell
.\main.exe
```

## Dinh dang file TXT

### Doc gia

Header:

```text
Loai|MaDG|Ten|Tuoi|GioiTinh|SDT|Email|DiaChi
```

Vi du:

```text
Thuong|DG001|Nguyen Van A|20|Nam|0900000001|a@example.com|Ha Noi
VIP|DG002|Tran Thi B|30|Nu|0900000002|b@example.com|Da Nang
Sinh vien|DG003|Le Van C|21|Nam|0900000003|c@example.com|TP HCM
```

Gia tri hop le:

- `Loai`: `Thuong`, `VIP`, `Sinh vien`
- `GioiTinh`: `Nam`, `Nu`

### Sach

Header:

```text
MaSach|TenSach|TheLoai|GiaTien|SoLuong
```

Vi du:

```text
101|Dac Nhan Tam|Tam ly hoc|80000|5
102|Lap trinh C++|Khoa hoc/Ky thuat|200000|7
```

The loai dang duoc dung trong giao dien:

- `Kinh te`
- `Khoa hoc/Ky thuat`
- `Lich su`
- `Van hoc`
- `Tam ly hoc`
- `Tai lieu tham khao`


## Chuong trinh hien co
- Ban WinForms C# nap du lieu mau khi khoi dong.
- Du lieu doc gia va sach co the nhap/xuat bang TXT.