using System;
using System.Collections.Generic;
using System.Globalization;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using static QuanLyThuVienUi.UiHelpers;

namespace QuanLyThuVienUi
{
    public abstract class DocGia
    {
        public string MaDG { get; set; } = "";
        public string Ten { get; set; } = "";
        public int Tuoi { get; set; }
        public string GioiTinh { get; set; } = "Nam";
        public string Sdt { get; set; } = "";
        public string Email { get; set; } = "";
        public string DiaChi { get; set; } = "";

        public abstract string Loai { get; }
        public abstract int SoSachMuonToiDa { get; }
        public abstract int SoNgayMuonToiDa { get; }
    }

    public sealed class DocGiaThuong : DocGia
    {
        public override string Loai => "Thường";
        public override int SoSachMuonToiDa => 3;
        public override int SoNgayMuonToiDa => 14;
    }

    public sealed class DocGiaVIP : DocGia
    {
        public override string Loai => "VIP";
        public override int SoSachMuonToiDa => 10;
        public override int SoNgayMuonToiDa => 30;
    }

    public sealed class DocGiaSinhVien : DocGia
    {
        public override string Loai => "Sinh viên";
        public override int SoSachMuonToiDa => 5;
        public override int SoNgayMuonToiDa => 21;
    }

    public sealed class Sach
    {
        public string MaSach { get; set; } = "";
        public string TenSach { get; set; } = "";
        public string TheLoai { get; set; } = "";
        public decimal GiaTien { get; set; }
        public int SoLuong { get; set; }
    }

    public sealed class PhieuMuon
    {
        public string MaSach { get; set; } = "";
        public string MaDG { get; set; } = "";
        public DateTime NgayMuon { get; set; }
        public DateTime NgayHenTra { get; set; }

        public bool BiTreHan(DateTime ngayKiemTra)
        {
            return NgayHenTra.Date < ngayKiemTra.Date;
        }

        public bool BiTreHanKhiTra(DateTime ngayTraThucTe)
        {
            return ngayTraThucTe.Date > NgayHenTra.Date;
        }
    }

    internal static class LibraryConstants
    {
        public static readonly string[] TheLoai =
        {
            "Kinh tế",
            "Khoa học/Kỹ thuật",
            "Lịch sử",
            "Văn học",
            "Tâm lý học",
            "Tài liệu tham khảo"
        };

        public static readonly string[] LoaiDocGia = { "Thường", "VIP", "Sinh viên" };
        public static readonly string[] GioiTinh = { "Nam", "Nữ" };
        public static readonly string[] TinhTrangSach = { "Còn", "Mất" };
    }

    public sealed class MainForm : Form
    {
        private readonly List<DocGia> dsDocGia = new List<DocGia>();
        private readonly List<Sach> dsSach = new List<Sach>();
        private readonly List<PhieuMuon> dsMuon = new List<PhieuMuon>();

        private readonly FlowLayoutPanel actionPanel;
        private readonly DataGridView grid;
        private readonly Label lblTitle;
        private readonly TextBox txtInfo;

        private static readonly Color AppBackground = Color.FromArgb(241, 245, 249);
        private static readonly Color SidebarBackground = Color.FromArgb(15, 23, 42);
        private static readonly Color PrimaryColor = Color.FromArgb(37, 99, 235);
        private static readonly Color PrimaryHoverColor = Color.FromArgb(29, 78, 216);
        private static readonly Color SurfaceColor = Color.White;
        private static readonly Color TextColor = Color.FromArgb(15, 23, 42);
        private static readonly Color MutedTextColor = Color.FromArgb(71, 85, 105);

        public MainForm()
        {
            Text = "Quản lý thư viện";
            Size = new Size(1180, 760);
            MinimumSize = new Size(980, 620);
            StartPosition = FormStartPosition.CenterScreen;
            Font = new Font("Segoe UI", 10F);
            BackColor = AppBackground;

            KhoiTaoDuLieuMau();

            var root = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 2,
                RowCount = 1,
                BackColor = AppBackground
            };
            root.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 215F));
            root.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            Controls.Add(root);

            var nav = new FlowLayoutPanel
            {
                Dock = DockStyle.Fill,
                FlowDirection = FlowDirection.TopDown,
                WrapContents = false,
                Padding = new Padding(16, 18, 16, 16),
                BackColor = SidebarBackground
            };
            root.Controls.Add(nav, 0, 0);

            nav.Controls.Add(new Label
            {
                Text = "Thư viện",
                ForeColor = Color.White,
                Font = new Font("Segoe UI", 18F, FontStyle.Bold),
                AutoSize = false,
                Width = 180,
                Height = 40,
                Margin = new Padding(0, 0, 0, 18)
            });

            AddNavButton(nav, "Độc giả", HienDocGia);
            AddNavButton(nav, "Sách", HienSach);
            AddNavButton(nav, "Mượn / Trả", HienMuonTra);
            AddNavButton(nav, "Thống kê", HienThongKe);

            var content = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 4,
                Padding = new Padding(18),
                BackColor = AppBackground
            };
            content.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            content.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            content.RowStyles.Add(new RowStyle(SizeType.Percent, 72F));
            content.RowStyles.Add(new RowStyle(SizeType.Percent, 28F));
            root.Controls.Add(content, 1, 0);

            lblTitle = new Label
            {
                AutoSize = true,
                Font = new Font("Segoe UI", 18F, FontStyle.Bold),
                ForeColor = TextColor,
                Padding = new Padding(0, 0, 0, 10)
            };
            content.Controls.Add(lblTitle, 0, 0);

            actionPanel = new FlowLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoSize = true,
                WrapContents = true,
                Padding = new Padding(0, 0, 0, 10),
                BackColor = AppBackground
            };
            content.Controls.Add(actionPanel, 0, 1);

            grid = new DataGridView
            {
                Dock = DockStyle.Fill,
                ReadOnly = true,
                AllowUserToAddRows = false,
                AllowUserToDeleteRows = false,
                AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill,
                AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.DisplayedCells,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect,
                MultiSelect = false,
                RowHeadersVisible = false,
                BackgroundColor = Color.White,
                BorderStyle = BorderStyle.None,
                CellBorderStyle = DataGridViewCellBorderStyle.SingleHorizontal,
                GridColor = Color.FromArgb(226, 232, 240),
                EnableHeadersVisualStyles = false
            };
            grid.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            grid.ColumnHeadersDefaultCellStyle.BackColor = Color.FromArgb(30, 41, 59);
            grid.ColumnHeadersDefaultCellStyle.ForeColor = Color.White;
            grid.ColumnHeadersDefaultCellStyle.Font = new Font("Segoe UI", 10F, FontStyle.Bold);
            grid.ColumnHeadersDefaultCellStyle.Padding = new Padding(6, 6, 6, 6);
            grid.DefaultCellStyle.BackColor = Color.White;
            grid.DefaultCellStyle.ForeColor = TextColor;
            grid.DefaultCellStyle.SelectionBackColor = Color.FromArgb(219, 234, 254);
            grid.DefaultCellStyle.SelectionForeColor = TextColor;
            grid.DefaultCellStyle.WrapMode = DataGridViewTriState.False;
            grid.DefaultCellStyle.Padding = new Padding(6, 4, 6, 4);
            grid.AlternatingRowsDefaultCellStyle.BackColor = Color.FromArgb(248, 250, 252);
            grid.RowTemplate.Height = 32;
            content.Controls.Add(grid, 0, 2);

            txtInfo = new TextBox
            {
                Dock = DockStyle.Fill,
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Vertical,
                Font = new Font("Consolas", 10F),
                BackColor = SurfaceColor,
                ForeColor = MutedTextColor,
                BorderStyle = BorderStyle.FixedSingle
            };
            content.Controls.Add(txtInfo, 0, 3);

            HienDocGia();
        }

        private void KhoiTaoDuLieuMau()
        {
            dsSach.AddRange(new[]
            {
                new Sach { MaSach = "101", TenSach = "Đắc Nhân Tâm", TheLoai = "Tâm lý học", GiaTien = 80000, SoLuong = 5 },
                new Sach { MaSach = "102", TenSach = "Tư duy nhanh và chậm", TheLoai = "Tâm lý học", GiaTien = 120000, SoLuong = 3 },
                new Sach { MaSach = "103", TenSach = "Kinh tế học vi mô", TheLoai = "Kinh tế", GiaTien = 150000, SoLuong = 4 },
                new Sach { MaSach = "104", TenSach = "Nguyên lý kế toán", TheLoai = "Kinh tế", GiaTien = 130000, SoLuong = 6 },
                new Sach { MaSach = "105", TenSach = "Lập trình C++", TheLoai = "Khoa học/Kỹ thuật", GiaTien = 200000, SoLuong = 7 },
                new Sach { MaSach = "106", TenSach = "Trí tuệ nhân tạo", TheLoai = "Khoa học/Kỹ thuật", GiaTien = 250000, SoLuong = 2 },
                new Sach { MaSach = "107", TenSach = "Lịch sử Việt Nam", TheLoai = "Lịch sử", GiaTien = 90000, SoLuong = 5 },
                new Sach { MaSach = "108", TenSach = "Thế giới cổ đại", TheLoai = "Lịch sử", GiaTien = 110000, SoLuong = 3 },
                new Sach { MaSach = "109", TenSach = "Tôi thấy hoa vàng trên cỏ xanh", TheLoai = "Văn học", GiaTien = 95000, SoLuong = 6 },
                new Sach { MaSach = "110", TenSach = "Truyện Kiều", TheLoai = "Văn học", GiaTien = 70000, SoLuong = 4 }
            });
        }

        private void AddNavButton(FlowLayoutPanel panel, string text, Action action)
        {
            var button = new Button
            {
                Text = text,
                Width = 180,
                Height = 44,
                Margin = new Padding(0, 0, 0, 10),
                TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(14, 0, 0, 0)
            };
            StyleNavButton(button);
            button.Click += (_, _) => action();
            panel.Controls.Add(button);
        }

        private void SetActions(params (string text, Action action)[] actions)
        {
            actionPanel.Controls.Clear();
            foreach (var item in actions)
            {
                var button = new Button
                {
                    Text = item.text,
                    Width = 150,
                    Height = 38,
                    Margin = new Padding(0, 0, 8, 8)
                };
                StyleActionButton(button);
                button.Click += (_, _) => item.action();
                actionPanel.Controls.Add(button);
            }
        }

        private static void StyleNavButton(Button button)
        {
            button.FlatStyle = FlatStyle.Flat;
            button.FlatAppearance.BorderSize = 0;
            button.BackColor = SidebarBackground;
            button.ForeColor = Color.FromArgb(226, 232, 240);
            button.Font = new Font("Segoe UI", 10.5F, FontStyle.Bold);
            button.Cursor = Cursors.Hand;
            button.MouseEnter += (_, _) => button.BackColor = Color.FromArgb(30, 41, 59);
            button.MouseLeave += (_, _) => button.BackColor = SidebarBackground;
        }

        private static void StyleActionButton(Button button)
        {
            button.FlatStyle = FlatStyle.Flat;
            button.FlatAppearance.BorderSize = 0;
            button.BackColor = PrimaryColor;
            button.ForeColor = Color.White;
            button.Font = new Font("Segoe UI", 9.5F, FontStyle.Bold);
            button.Cursor = Cursors.Hand;
            button.MouseEnter += (_, _) => button.BackColor = PrimaryHoverColor;
            button.MouseLeave += (_, _) => button.BackColor = PrimaryColor;
        }

        private void HienDocGia()
        {
            lblTitle.Text = "Quản lý độc giả";
            SetActions(
                ("Xem tất cả", () => RefreshDocGia(dsDocGia)),
                ("Thêm", ThemDocGia),
                ("Cập nhật", CapNhatDocGia),
                ("Xóa", XoaDocGia),
                ("Tìm theo mã", TimDocGia),
                ("Sắp xếp tên", SapXepDocGiaTheoTen),
                ("Nhập TXT", NhapDocGiaTuFile),
                ("Xuất TXT", XuatDocGiaRaFile));
            RefreshDocGia(dsDocGia);
        }

        private void RefreshDocGia(IEnumerable<DocGia> data)
        {
            grid.DataSource = data.Select(d => new
            {
                d.MaDG,
                Loai = d.Loai,
                d.Ten,
                d.Tuoi,
                d.GioiTinh,
                SDT = d.Sdt,
                d.Email,
                DiaChi = d.DiaChi,
                ToiDaSach = d.SoSachMuonToiDa,
                ToiDaNgay = d.SoNgayMuonToiDa
            }).ToList();
            SetGridHeaders(new Dictionary<string, string>
            {
                ["MaDG"] = "Mã độc giả",
                ["Loai"] = "Loại",
                ["Ten"] = "Tên",
                ["Tuoi"] = "Tuổi",
                ["GioiTinh"] = "Giới tính",
                ["SDT"] = "SĐT",
                ["Email"] = "Email",
                ["DiaChi"] = "Địa chỉ",
                ["ToiDaSach"] = "Tối đa sách",
                ["ToiDaNgay"] = "Tối đa ngày"
            });

            txtInfo.Text = "Tổng số độc giả: " + dsDocGia.Count + Environment.NewLine
                         + "Độc giả nam: " + dsDocGia.Count(d => d.GioiTinh.Equals("Nam", StringComparison.OrdinalIgnoreCase)) + Environment.NewLine
                         + "Độc giả nữ: " + dsDocGia.Count(d => d.GioiTinh.Equals("Nữ", StringComparison.OrdinalIgnoreCase));
        }

        private void ThemDocGia()
        {
            using (var form = new DocGiaForm(null))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                if (TimDocGiaTheoMa(form.MaDG) != null)
                {
                    ShowError("Trùng mã độc giả. Không thể thêm.");
                    return;
                }

                dsDocGia.Add(TaoDocGia(form));
                HienDocGia();
            }
        }

        private void CapNhatDocGia()
        {
            string? ma = GetSelectedValue("MaDG");
            DocGia? docGia = ma == null ? null : TimDocGiaTheoMa(ma);
            if (docGia == null)
            {
                ShowError("Hãy chọn độc giả cần cập nhật.");
                return;
            }

            using (var form = new DocGiaForm(docGia))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                int index = dsDocGia.FindIndex(d => d.MaDG == docGia.MaDG);
                dsDocGia[index] = TaoDocGia(form);
                HienDocGia();
            }
        }

        private void XoaDocGia()
        {
            string? ma = GetSelectedValue("MaDG");
            DocGia? docGia = ma == null ? null : TimDocGiaTheoMa(ma);
            if (docGia == null)
            {
                ShowError("Hãy chọn độc giả cần xóa.");
                return;
            }

            if (dsMuon.Any(pm => pm.MaDG == docGia.MaDG))
            {
                ShowError("Độc giả đang mượn sách, không thể xóa.");
                return;
            }

            if (Confirm("Xóa độc giả " + docGia.MaDG + "?"))
            {
                dsDocGia.Remove(docGia);
                HienDocGia();
            }
        }

        private void TimDocGia()
        {
            string? ma = TextPrompt.Show(this, "Tìm độc giả", "Mã độc giả:");
            if (string.IsNullOrWhiteSpace(ma))
            {
                return;
            }

            DocGia? docGia = TimDocGiaTheoMa(ma.Trim());
            if (docGia == null)
            {
                ShowError("Không tìm thấy độc giả.");
                return;
            }

            RefreshDocGia(new[] { docGia });
        }

        private void SapXepDocGiaTheoTen()
        {
            dsDocGia.Sort((a, b) => string.Compare(a.Ten, b.Ten, StringComparison.OrdinalIgnoreCase));
            HienDocGia();
        }

        private void NhapDocGiaTuFile()
        {
            using (var dialog = new OpenFileDialog())
            {
                dialog.Title = "Nhập danh sách độc giả";
                dialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";

                if (dialog.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                int them = 0;
                int capNhat = 0;
                int boQua = 0;

                foreach (string rawLine in File.ReadLines(dialog.FileName, Encoding.UTF8))
                {
                    string line = rawLine.Trim();
                    if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#"))
                    {
                        continue;
                    }

                    string[] fields = SplitFileLine(line);
                    if (fields.Length > 0 && fields[0].Equals("Loai", StringComparison.OrdinalIgnoreCase))
                    {
                        continue;
                    }

                    if (fields.Length != 8 || string.IsNullOrWhiteSpace(fields[1]) || string.IsNullOrWhiteSpace(fields[2]))
                    {
                        boQua++;
                        continue;
                    }

                    string? loai = NormalizeLoaiDocGia(fields[0]);
                    string? gioiTinh = NormalizeGioiTinh(fields[4]);
                    if (loai == null || gioiTinh == null || !int.TryParse(fields[3], out int tuoi) || tuoi < 0)
                    {
                        boQua++;
                        continue;
                    }

                    DocGia docGia = TaoDocGia(loai, fields[1], fields[2], tuoi, gioiTinh, fields[5], fields[6], fields[7]);
                    int index = dsDocGia.FindIndex(d => d.MaDG.Equals(docGia.MaDG, StringComparison.OrdinalIgnoreCase));
                    if (index == -1)
                    {
                        dsDocGia.Add(docGia);
                        them++;
                    }
                    else
                    {
                        dsDocGia[index] = docGia;
                        capNhat++;
                    }
                }

                HienDocGia();
                ShowError("Nhập độc giả hoàn tất. Thêm: " + them + ", cập nhật: " + capNhat + ", bỏ qua: " + boQua + ".");
            }
        }

        private void XuatDocGiaRaFile()
        {
            using (var dialog = new SaveFileDialog())
            {
                dialog.Title = "Xuất danh sách độc giả";
                dialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                dialog.FileName = "doc_gia.txt";

                if (dialog.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                List<string> lines = new List<string>
                {
                    "Loai|MaDG|Ten|Tuoi|GioiTinh|SDT|Email|DiaChi"
                };

                lines.AddRange(dsDocGia.Select(d => string.Join("|", new[]
                {
                    LoaiDocGiaToFileValue(d.Loai),
                    CleanFileField(d.MaDG),
                    CleanFileField(d.Ten),
                    d.Tuoi.ToString(CultureInfo.InvariantCulture),
                    GioiTinhToFileValue(d.GioiTinh),
                    CleanFileField(d.Sdt),
                    CleanFileField(d.Email),
                    CleanFileField(d.DiaChi)
                })));

                File.WriteAllLines(dialog.FileName, lines, Encoding.UTF8);
                ShowError("Đã xuất " + dsDocGia.Count + " độc giả ra file.");
            }
        }

        private DocGia TaoDocGia(DocGiaForm form)
        {
            DocGia docGia = form.Loai switch
            {
                "VIP" => new DocGiaVIP(),
                "Sinh viên" => new DocGiaSinhVien(),
                _ => new DocGiaThuong()
            };

            docGia.MaDG = form.MaDG;
            docGia.Ten = form.TenDocGia;
            docGia.Tuoi = form.Tuoi;
            docGia.GioiTinh = form.GioiTinh;
            docGia.Sdt = form.Sdt;
            docGia.Email = form.Email;
            docGia.DiaChi = form.DiaChi;
            return docGia;
        }

        private DocGia TaoDocGia(string loai, string ma, string ten, int tuoi, string gioiTinh, string sdt, string email, string diaChi)
        {
            DocGia docGia = loai switch
            {
                "VIP" => new DocGiaVIP(),
                "Sinh viên" => new DocGiaSinhVien(),
                _ => new DocGiaThuong()
            };

            docGia.MaDG = ma.Trim();
            docGia.Ten = ten.Trim();
            docGia.Tuoi = tuoi;
            docGia.GioiTinh = gioiTinh;
            docGia.Sdt = sdt.Trim();
            docGia.Email = email.Trim();
            docGia.DiaChi = diaChi.Trim();
            return docGia;
        }

        private void HienSach()
        {
            lblTitle.Text = "Quản lý sách";
            SetActions(
                ("Xem tất cả", () => RefreshSach(dsSach)),
                ("Thêm", ThemSach),
                ("Cập nhật", CapNhatSach),
                ("Xóa", XoaSach),
                ("Tìm theo mã", TimSachTheoMa),
                ("Tìm thể loại", TimSachTheoTheLoai),
                ("Sắp xếp tên", SapXepSachTheoTen),
                ("Sắp xếp số lượng", SapXepSachTheoSoLuong),
                ("Nhập TXT", NhapSachTuFile),
                ("Xuất TXT", XuatSachRaFile));
            RefreshSach(dsSach);
        }

        private void RefreshSach(IEnumerable<Sach> data)
        {
            grid.DataSource = data.Select(s => new
            {
                s.MaSach,
                s.TenSach,
                s.TheLoai,
                GiaTien = DinhDangTien(s.GiaTien),
                s.SoLuong
            }).ToList();
            SetGridHeaders(new Dictionary<string, string>
            {
                ["MaSach"] = "Mã sách",
                ["TenSach"] = "Tên sách",
                ["TheLoai"] = "Thể loại",
                ["GiaTien"] = "Giá tiền",
                ["SoLuong"] = "Số lượng"
            });

            txtInfo.Text = "Tổng số đầu sách: " + dsSach.Count + Environment.NewLine
                         + "Tổng số sách còn: " + dsSach.Sum(s => s.SoLuong) + Environment.NewLine
                         + "Số sách đang mượn: " + dsMuon.Count;
        }

        private void ThemSach()
        {
            using (var form = new SachForm(null))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                Sach? existing = TimSachTheoMaCore(form.MaSach);
                if (existing != null)
                {
                    int? soLuongThem = NumberPrompt.Show(this, "Tăng số lượng", "Mã sách đã tồn tại. Nhập số lượng tăng thêm:", 1, 100000);
                    if (soLuongThem.HasValue)
                    {
                        existing.SoLuong += soLuongThem.Value;
                        HienSach();
                    }
                    return;
                }

                dsSach.Add(new Sach
                {
                    MaSach = form.MaSach,
                    TenSach = form.TenSach,
                    TheLoai = form.TheLoai,
                    GiaTien = form.GiaTien,
                    SoLuong = form.SoLuong
                });
                HienSach();
            }
        }

        private void CapNhatSach()
        {
            string? ma = GetSelectedValue("MaSach");
            Sach? sach = ma == null ? null : TimSachTheoMaCore(ma);
            if (sach == null)
            {
                ShowError("Hãy chọn sách cần cập nhật.");
                return;
            }

            using (var form = new SachForm(sach))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                sach.GiaTien = form.GiaTien;
                sach.SoLuong = form.SoLuong;
                HienSach();
            }
        }

        private void XoaSach()
        {
            string? ma = GetSelectedValue("MaSach");
            Sach? sach = ma == null ? null : TimSachTheoMaCore(ma);
            if (sach == null)
            {
                ShowError("Hãy chọn sách cần xóa.");
                return;
            }

            if (dsMuon.Any(pm => pm.MaSach == sach.MaSach))
            {
                ShowError("Sách đang được mượn, không thể xóa.");
                return;
            }

            if (Confirm("Xóa sách " + sach.MaSach + "?"))
            {
                dsSach.Remove(sach);
                HienSach();
            }
        }

        private void TimSachTheoMa()
        {
            string? ma = TextPrompt.Show(this, "Tìm sách", "Mã sách:");
            if (string.IsNullOrWhiteSpace(ma))
            {
                return;
            }

            Sach? sach = TimSachTheoMaCore(ma.Trim());
            if (sach == null)
            {
                ShowError("Không tìm thấy sách.");
                return;
            }

            RefreshSach(new[] { sach });
        }

        private void TimSachTheoTheLoai()
        {
            string? theLoai = ChoicePrompt.Show(this, "Tìm theo thể loại", "Chọn thể loại:", LibraryConstants.TheLoai);
            if (theLoai == null)
            {
                return;
            }

            List<Sach> result = dsSach.Where(s => s.TheLoai == theLoai).ToList();
            if (result.Count == 0)
            {
                ShowError("Không có sách thuộc thể loại này.");
                return;
            }

            RefreshSach(result);
        }

        private void SapXepSachTheoTen()
        {
            dsSach.Sort((a, b) => string.Compare(a.TenSach, b.TenSach, StringComparison.OrdinalIgnoreCase));
            HienSach();
        }

        private void SapXepSachTheoSoLuong()
        {
            string? chon = ChoicePrompt.Show(this, "Sắp xếp số lượng", "Chọn kiểu sắp xếp:", new[] { "Tăng dần", "Giảm dần" });
            if (chon == null)
            {
                return;
            }

            if (chon == "Tăng dần")
            {
                dsSach.Sort((a, b) => a.SoLuong.CompareTo(b.SoLuong));
            }
            else
            {
                dsSach.Sort((a, b) => b.SoLuong.CompareTo(a.SoLuong));
            }

            HienSach();
        }

        private void NhapSachTuFile()
        {
            using (var dialog = new OpenFileDialog())
            {
                dialog.Title = "Nhập danh sách sách";
                dialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";

                if (dialog.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                int them = 0;
                int capNhat = 0;
                int boQua = 0;

                foreach (string rawLine in File.ReadLines(dialog.FileName, Encoding.UTF8))
                {
                    string line = rawLine.Trim();
                    if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#"))
                    {
                        continue;
                    }

                    string[] fields = SplitFileLine(line);
                    if (fields.Length > 0 && fields[0].Equals("MaSach", StringComparison.OrdinalIgnoreCase))
                    {
                        continue;
                    }

                    if (fields.Length != 5 || string.IsNullOrWhiteSpace(fields[0]) || string.IsNullOrWhiteSpace(fields[1]))
                    {
                        boQua++;
                        continue;
                    }

                    if (!decimal.TryParse(fields[3], NumberStyles.Number, CultureInfo.InvariantCulture, out decimal giaTien)
                        || !int.TryParse(fields[4], NumberStyles.Integer, CultureInfo.InvariantCulture, out int soLuong)
                        || giaTien < 0
                        || soLuong < 0)
                    {
                        boQua++;
                        continue;
                    }

                    var sach = new Sach
                    {
                        MaSach = fields[0].Trim(),
                        TenSach = fields[1].Trim(),
                        TheLoai = NormalizeTheLoai(fields[2]),
                        GiaTien = giaTien,
                        SoLuong = soLuong
                    };

                    int index = dsSach.FindIndex(s => s.MaSach.Equals(sach.MaSach, StringComparison.OrdinalIgnoreCase));
                    if (index == -1)
                    {
                        dsSach.Add(sach);
                        them++;
                    }
                    else
                    {
                        dsSach[index] = sach;
                        capNhat++;
                    }
                }

                HienSach();
                ShowError("Nhập sách hoàn tất. Thêm: " + them + ", cập nhật: " + capNhat + ", bỏ qua: " + boQua + ".");
            }
        }

        private void XuatSachRaFile()
        {
            using (var dialog = new SaveFileDialog())
            {
                dialog.Title = "Xuất danh sách sách";
                dialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                dialog.FileName = "sach.txt";

                if (dialog.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                List<string> lines = new List<string>
                {
                    "MaSach|TenSach|TheLoai|GiaTien|SoLuong"
                };

                lines.AddRange(dsSach.Select(s => string.Join("|", new[]
                {
                    CleanFileField(s.MaSach),
                    CleanFileField(s.TenSach),
                    CleanFileField(s.TheLoai),
                    s.GiaTien.ToString("0", CultureInfo.InvariantCulture),
                    s.SoLuong.ToString(CultureInfo.InvariantCulture)
                })));

                File.WriteAllLines(dialog.FileName, lines, Encoding.UTF8);
                ShowError("Đã xuất " + dsSach.Count + " sách ra file.");
            }
        }

        private void HienMuonTra()
        {
            lblTitle.Text = "Mượn và trả sách";
            SetActions(
                ("Sách đang mượn", () => RefreshPhieuMuon(dsMuon)),
                ("Mượn sách", MuonSach),
                ("Trả sách", TraSach),
                ("Độc giả trễ hạn", HienDocGiaTreHan));
            RefreshPhieuMuon(dsMuon);
        }

        private void RefreshPhieuMuon(IEnumerable<PhieuMuon> data)
        {
            grid.DataSource = data.Select(pm => new
            {
                pm.MaDG,
                TenDocGia = TimDocGiaTheoMa(pm.MaDG)?.Ten ?? "Không rõ",
                pm.MaSach,
                TenSach = TimSachTheoMaCore(pm.MaSach)?.TenSach ?? "Không rõ",
                NgayMuon = pm.NgayMuon.ToString("dd/MM/yyyy"),
                NgayHenTra = pm.NgayHenTra.ToString("dd/MM/yyyy"),
                TrangThai = pm.BiTreHan(DateTime.Today) ? "Trễ hạn" : "Đang mượn"
            }).ToList();
            SetGridHeaders(new Dictionary<string, string>
            {
                ["MaDG"] = "Mã độc giả",
                ["TenDocGia"] = "Tên độc giả",
                ["MaSach"] = "Mã sách",
                ["TenSach"] = "Tên sách",
                ["NgayMuon"] = "Ngày mượn",
                ["NgayHenTra"] = "Ngày hẹn trả",
                ["TrangThai"] = "Trạng thái"
            });

            txtInfo.Text = "Số phiếu đang mượn: " + dsMuon.Count + Environment.NewLine
                         + "Số phiếu trễ hạn: " + dsMuon.Count(pm => pm.BiTreHan(DateTime.Today));
        }

        private void MuonSach()
        {
            if (dsDocGia.Count == 0)
            {
                ShowError("Chưa có độc giả.");
                return;
            }

            if (!dsSach.Any(s => s.SoLuong > 0))
            {
                ShowError("Không có sách còn để mượn.");
                return;
            }

            using (var form = new MuonSachForm(dsSach.Where(s => s.SoLuong > 0), dsDocGia))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                Sach sach = form.SelectedSach;
                DocGia docGia = form.SelectedDocGia;

                if (dsMuon.Any(pm => pm.MaDG == docGia.MaDG && pm.MaSach == sach.MaSach))
                {
                    ShowError("Độc giả này đang mượn cuốn sách này.");
                    return;
                }

                int soSachDangMuon = dsMuon.Count(pm => pm.MaDG == docGia.MaDG);
                if (soSachDangMuon >= docGia.SoSachMuonToiDa)
                {
                    ShowError("Độc giả " + docGia.Loai + " chỉ được mượn tối đa " + docGia.SoSachMuonToiDa + " sách.");
                    return;
                }

                if (form.NgayHenTra.Date < form.NgayMuon.Date)
                {
                    ShowError("Ngày trả dự kiến không được nhỏ hơn ngày mượn.");
                    return;
                }

                int soNgayMuon = (form.NgayHenTra.Date - form.NgayMuon.Date).Days;
                if (soNgayMuon > docGia.SoNgayMuonToiDa)
                {
                    ShowError("Độc giả " + docGia.Loai + " chỉ được mượn tối đa " + docGia.SoNgayMuonToiDa + " ngày.");
                    return;
                }

                sach.SoLuong--;
                dsMuon.Add(new PhieuMuon
                {
                    MaSach = sach.MaSach,
                    MaDG = docGia.MaDG,
                    NgayMuon = form.NgayMuon.Date,
                    NgayHenTra = form.NgayHenTra.Date
                });

                HienMuonTra();
            }
        }

        private void TraSach()
        {
            if (dsMuon.Count == 0)
            {
                ShowError("Không có phiếu mượn nào.");
                return;
            }

            using (var form = new TraSachForm(dsMuon, dsDocGia, dsSach))
            {
                if (form.ShowDialog(this) != DialogResult.OK)
                {
                    return;
                }

                PhieuMuon phieu = form.SelectedPhieuMuon;
                Sach? sach = TimSachTheoMaCore(phieu.MaSach);
                if (sach == null)
                {
                    ShowError("Không tìm thấy sách trong danh mục.");
                    return;
                }

                if (form.NgayTraThucTe.Date < phieu.NgayMuon.Date)
                {
                    ShowError("Ngày trả thực tế không được nhỏ hơn ngày mượn.");
                    return;
                }

                decimal tienPhat = 0;
                if (phieu.BiTreHanKhiTra(form.NgayTraThucTe))
                {
                    tienPhat += 50000;
                }

                if (form.TinhTrangSach == "Mất")
                {
                    tienPhat += sach.GiaTien;
                }

                if (tienPhat > 0)
                {
                    string message = "Tổng tiền phạt: " + DinhDangTien(tienPhat) + Environment.NewLine
                                   + "Xác nhận nếu độc giả đã nộp phạt.";
                    if (MessageBox.Show(this, message, "Xác nhận nộp phạt", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) != DialogResult.Yes)
                    {
                        ShowError("Chưa nộp phạt. Không thể hoàn tất trả sách.");
                        return;
                    }
                }

                if (form.TinhTrangSach == "Còn")
                {
                    sach.SoLuong++;
                }

                dsMuon.Remove(phieu);
                HienMuonTra();
            }
        }

        private void HienDocGiaTreHan()
        {
            List<PhieuMuon> treHan = dsMuon.Where(pm => pm.BiTreHan(DateTime.Today)).ToList();
            RefreshPhieuMuon(treHan);
            txtInfo.AppendText(Environment.NewLine + "Ngày hiện tại: " + DateTime.Today.ToString("dd/MM/yyyy"));
        }

        private void HienThongKe()
        {
            lblTitle.Text = "Thống kê";
            SetActions(
                ("Thống kê tổng hợp", HienThongKeTongHop),
                ("Sách đang mượn", () => RefreshPhieuMuon(dsMuon)),
                ("Độc giả trễ hạn", HienDocGiaTreHan),
                ("Sắp xếp độc giả", SapXepDocGiaTheoTen),
                ("Sắp xếp theo tên", SapXepSachTheoTen),
                ("Sắp xếp số lượng", SapXepSachTheoSoLuong));
            HienThongKeTongHop();
        }

        private void HienThongKeTongHop()
        {
            var thongKeTheLoai = dsSach
                .GroupBy(s => s.TheLoai)
                .Select(g => new
                {
                    TheLoai = g.Key,
                    DauSach = g.Count(),
                    SoLuongCon = g.Sum(s => s.SoLuong)
                })
                .ToList();

            grid.DataSource = thongKeTheLoai;
            SetGridHeaders(new Dictionary<string, string>
            {
                ["TheLoai"] = "Thể loại",
                ["DauSach"] = "Đầu sách",
                ["SoLuongCon"] = "Số lượng còn"
            });

            int tongSachCon = dsSach.Sum(s => s.SoLuong);
            int soSachDangMuon = dsMuon.Count;
            int tongSachDangQuanLy = tongSachCon + soSachDangMuon;

            List<string> lines = new List<string>
            {
                "Tổng số đầu sách: " + dsSach.Count,
                "Tổng số sách đang quản lý: " + tongSachDangQuanLy,
                "Số sách còn trong thư viện: " + tongSachCon,
                "Số sách đang mượn: " + soSachDangMuon,
                "Đầu sách còn hàng: " + dsSach.Count(s => s.SoLuong > 0),
                "Đầu sách không còn hàng: " + dsSach.Count(s => s.SoLuong == 0),
                "",
                "Tổng số độc giả: " + dsDocGia.Count,
                "Độc giả nam: " + dsDocGia.Count(d => d.GioiTinh.Equals("Nam", StringComparison.OrdinalIgnoreCase)),
                "Độc giả nữ: " + dsDocGia.Count(d => d.GioiTinh.Equals("Nữ", StringComparison.OrdinalIgnoreCase)),
                "Độc giả Thường: " + dsDocGia.Count(d => d.Loai == "Thường"),
                "Độc giả VIP: " + dsDocGia.Count(d => d.Loai == "VIP"),
                "Độc giả sinh viên: " + dsDocGia.Count(d => d.Loai == "Sinh viên"),
                "",
                "Độc giả trễ hạn: " + dsMuon.Count(pm => pm.BiTreHan(DateTime.Today))
            };

            txtInfo.Text = string.Join(Environment.NewLine, lines);
        }

        private DocGia? TimDocGiaTheoMa(string ma)
        {
            return dsDocGia.FirstOrDefault(d => d.MaDG.Equals(ma, StringComparison.OrdinalIgnoreCase));
        }

        private Sach? TimSachTheoMaCore(string ma)
        {
            return dsSach.FirstOrDefault(s => s.MaSach.Equals(ma, StringComparison.OrdinalIgnoreCase));
        }

        private string? GetSelectedValue(string columnName)
        {
            if (grid.CurrentRow == null || !grid.Columns.Contains(columnName))
            {
                return null;
            }

            object? value = grid.CurrentRow.Cells[columnName].Value;
            return value?.ToString();
        }

        private void SetGridHeaders(Dictionary<string, string> headers)
        {
            foreach (DataGridViewColumn column in grid.Columns)
            {
                if (headers.TryGetValue(column.Name, out string? header))
                {
                    column.HeaderText = header;
                }

                column.MinimumWidth = 50;
                column.FillWeight = column.Name.Contains("Ten") || column.Name.Contains("DiaChi") || column.Name.Contains("Email") ? 160F : 100F;
            }
        }

        private static string[] SplitFileLine(string line)
        {
            return line.Split('|').Select(field => field.Trim()).ToArray();
        }

        private static string CleanFileField(string value)
        {
            return value.Replace("|", " ").Replace("\r", " ").Replace("\n", " ").Trim();
        }

        private static string? NormalizeLoaiDocGia(string value)
        {
            string key = value.Trim().ToLowerInvariant();
            if (key == "thuong" || key == "thường")
            {
                return "Thường";
            }

            if (key == "vip")
            {
                return "VIP";
            }

            if (key == "sinh vien" || key == "sinhvien" || key == "sinh viên")
            {
                return "Sinh viên";
            }

            return null;
        }

        private static string LoaiDocGiaToFileValue(string loai)
        {
            return loai switch
            {
                "VIP" => "VIP",
                "Sinh viên" => "Sinh vien",
                _ => "Thuong"
            };
        }

        private static string? NormalizeGioiTinh(string value)
        {
            string key = value.Trim().ToLowerInvariant();
            if (key == "nam")
            {
                return "Nam";
            }

            if (key == "nu" || key == "nữ")
            {
                return "Nữ";
            }

            return null;
        }

        private static string GioiTinhToFileValue(string gioiTinh)
        {
            return gioiTinh.Equals("Nữ", StringComparison.OrdinalIgnoreCase) ? "Nu" : "Nam";
        }

        private static string NormalizeTheLoai(string value)
        {
            string trimmed = value.Trim();
            string key = trimmed.ToLowerInvariant();
            return key switch
            {
                "kinh te" or "kinh tế" => "Kinh tế",
                "khoa hoc/ki thuat" or "khoa hoc/ky thuat" or "khoa học/kĩ thuật" or "khoa học/kỹ thuật" => "Khoa học/Kỹ thuật",
                "lich su" or "lịch sử" => "Lịch sử",
                "van hoc" or "văn học" => "Văn học",
                "tam ly hoc" or "tâm lý học" => "Tâm lý học",
                "tai lieu tham khao" or "tài liệu tham khảo" => "Tài liệu tham khảo",
                _ => trimmed
            };
        }

        private static string DinhDangTien(decimal soTien)
        {
            return decimal.Truncate(soTien).ToString("0") + " VND";
        }

        private void ShowError(string message)
        {
            MessageBox.Show(this, message, "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private bool Confirm(string message)
        {
            return MessageBox.Show(this, message, "Xác nhận", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes;
        }
    }

    internal sealed class DocGiaForm : Form
    {
        private readonly TextBox txtMa = new TextBox();
        private readonly TextBox txtTen = new TextBox();
        private readonly NumericUpDown numTuoi = new NumericUpDown();
        private readonly ComboBox cboGioiTinh = new ComboBox();
        private readonly TextBox txtSdt = new TextBox();
        private readonly TextBox txtEmail = new TextBox();
        private readonly TextBox txtDiaChi = new TextBox();
        private readonly ComboBox cboLoai = new ComboBox();

        public string MaDG => txtMa.Text.Trim();
        public string TenDocGia => txtTen.Text.Trim();
        public int Tuoi => (int)numTuoi.Value;
        public string GioiTinh => cboGioiTinh.Text;
        public string Sdt => txtSdt.Text.Trim();
        public string Email => txtEmail.Text.Trim();
        public string DiaChi => txtDiaChi.Text.Trim();
        public string Loai => cboLoai.Text;

        public DocGiaForm(DocGia? docGia)
        {
            Text = docGia == null ? "Thêm độc giả" : "Cập nhật độc giả";
            StartPosition = FormStartPosition.CenterParent;
            Size = new Size(430, 430);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            BackColor = Color.FromArgb(241, 245, 249);

            var layout = CreateDialogLayout(9);
            Controls.Add(layout);

            AddRow(layout, 0, "Mã độc giả", txtMa);
            AddRow(layout, 1, "Tên", txtTen);
            numTuoi.Minimum = 0;
            numTuoi.Maximum = 150;
            AddRow(layout, 2, "Tuổi", numTuoi);
            FillCombo(cboGioiTinh, LibraryConstants.GioiTinh);
            AddRow(layout, 3, "Giới tính", cboGioiTinh);
            AddRow(layout, 4, "SĐT", txtSdt);
            AddRow(layout, 5, "Email", txtEmail);
            AddRow(layout, 6, "Địa chỉ", txtDiaChi);
            FillCombo(cboLoai, LibraryConstants.LoaiDocGia);
            AddRow(layout, 7, "Loại độc giả", cboLoai);
            AddButtonRow(layout, 8);

            if (docGia != null)
            {
                txtMa.Text = docGia.MaDG;
                txtMa.Enabled = false;
                txtTen.Text = docGia.Ten;
                numTuoi.Value = docGia.Tuoi;
                cboGioiTinh.SelectedItem = docGia.GioiTinh;
                txtSdt.Text = docGia.Sdt;
                txtEmail.Text = docGia.Email;
                txtDiaChi.Text = docGia.DiaChi;
                cboLoai.SelectedItem = docGia.Loai;
            }
            else
            {
                cboGioiTinh.SelectedIndex = 0;
                cboLoai.SelectedIndex = 0;
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK && !ValidateRequired(MaDG, TenDocGia, Sdt, Email, DiaChi))
            {
                MessageBox.Show(this, "Vui lòng nhập đầy đủ thông tin.", "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Information);
                e.Cancel = true;
            }

            base.OnFormClosing(e);
        }

        private static bool ValidateRequired(params string[] values)
        {
            return values.All(v => !string.IsNullOrWhiteSpace(v));
        }
    }

    internal sealed class SachForm : Form
    {
        private readonly TextBox txtMa = new TextBox();
        private readonly TextBox txtTen = new TextBox();
        private readonly ComboBox cboTheLoai = new ComboBox();
        private readonly NumericUpDown numGia = new NumericUpDown();
        private readonly NumericUpDown numSoLuong = new NumericUpDown();

        public string MaSach => txtMa.Text.Trim();
        public string TenSach => txtTen.Text.Trim();
        public string TheLoai => cboTheLoai.Text;
        public decimal GiaTien => numGia.Value;
        public int SoLuong => (int)numSoLuong.Value;

        public SachForm(Sach? sach)
        {
            Text = sach == null ? "Thêm sách" : "Cập nhật sách";
            StartPosition = FormStartPosition.CenterParent;
            Size = new Size(430, 320);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            BackColor = Color.FromArgb(241, 245, 249);

            var layout = CreateDialogLayout(6);
            Controls.Add(layout);

            AddRow(layout, 0, "Mã sách", txtMa);
            AddRow(layout, 1, "Tên sách", txtTen);
            FillCombo(cboTheLoai, LibraryConstants.TheLoai);
            AddRow(layout, 2, "Thể loại", cboTheLoai);
            numGia.Minimum = 0;
            numGia.Maximum = 1000000000;
            numGia.Increment = 10000;
            AddRow(layout, 3, "Giá tiền", numGia);
            numSoLuong.Minimum = 0;
            numSoLuong.Maximum = 100000;
            AddRow(layout, 4, "Số lượng", numSoLuong);
            AddButtonRow(layout, 5);

            if (sach != null)
            {
                txtMa.Text = sach.MaSach;
                txtTen.Text = sach.TenSach;
                cboTheLoai.SelectedItem = sach.TheLoai;
                numGia.Value = sach.GiaTien;
                numSoLuong.Value = sach.SoLuong;

                txtMa.Enabled = false;
                txtTen.Enabled = false;
                cboTheLoai.Enabled = false;
            }
            else
            {
                cboTheLoai.SelectedIndex = 0;
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK && (string.IsNullOrWhiteSpace(MaSach) || string.IsNullOrWhiteSpace(TenSach)))
            {
                MessageBox.Show(this, "Vui lòng nhập đầy đủ thông tin.", "Thông báo", MessageBoxButtons.OK, MessageBoxIcon.Information);
                e.Cancel = true;
            }

            base.OnFormClosing(e);
        }
    }

    internal sealed class MuonSachForm : Form
    {
        private readonly ComboBox cboSach = new ComboBox();
        private readonly ComboBox cboDocGia = new ComboBox();
        private readonly DateTimePicker dtNgayMuon = new DateTimePicker();
        private readonly DateTimePicker dtNgayHenTra = new DateTimePicker();

        public Sach SelectedSach => ((SelectItem<Sach>)cboSach.SelectedItem!).Value;
        public DocGia SelectedDocGia => ((SelectItem<DocGia>)cboDocGia.SelectedItem!).Value;
        public DateTime NgayMuon => dtNgayMuon.Value.Date;
        public DateTime NgayHenTra => dtNgayHenTra.Value.Date;

        public MuonSachForm(IEnumerable<Sach> sach, IEnumerable<DocGia> docGia)
        {
            Text = "Mượn sách";
            StartPosition = FormStartPosition.CenterParent;
            Size = new Size(480, 270);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            BackColor = Color.FromArgb(241, 245, 249);

            var layout = CreateDialogLayout(5);
            Controls.Add(layout);

            cboSach.DropDownStyle = ComboBoxStyle.DropDownList;
            cboSach.DataSource = sach.Select(s => new SelectItem<Sach>(s.MaSach + " - " + s.TenSach + " (còn " + s.SoLuong + ")", s)).ToList();
            AddRow(layout, 0, "Sách", cboSach);

            cboDocGia.DropDownStyle = ComboBoxStyle.DropDownList;
            cboDocGia.DataSource = docGia.Select(d => new SelectItem<DocGia>(d.MaDG + " - " + d.Ten + " [" + d.Loai + "]", d)).ToList();
            AddRow(layout, 1, "Độc giả", cboDocGia);

            SetupDatePicker(dtNgayMuon);
            SetupDatePicker(dtNgayHenTra);
            dtNgayMuon.Value = DateTime.Today;
            dtNgayHenTra.Value = DateTime.Today.AddDays(7);
            AddRow(layout, 2, "Ngày mượn", dtNgayMuon);
            AddRow(layout, 3, "Hẹn trả", dtNgayHenTra);
            AddButtonRow(layout, 4);
        }
    }

    internal sealed class TraSachForm : Form
    {
        private readonly ComboBox cboPhieu = new ComboBox();
        private readonly DateTimePicker dtNgayTra = new DateTimePicker();
        private readonly ComboBox cboTinhTrang = new ComboBox();

        public PhieuMuon SelectedPhieuMuon => ((SelectItem<PhieuMuon>)cboPhieu.SelectedItem!).Value;
        public DateTime NgayTraThucTe => dtNgayTra.Value.Date;
        public string TinhTrangSach => cboTinhTrang.Text;

        public TraSachForm(IEnumerable<PhieuMuon> phieuMuon, IEnumerable<DocGia> docGia, IEnumerable<Sach> sach)
        {
            Text = "Trả sách";
            StartPosition = FormStartPosition.CenterParent;
            Size = new Size(520, 245);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            BackColor = Color.FromArgb(241, 245, 249);

            var layout = CreateDialogLayout(4);
            Controls.Add(layout);

            cboPhieu.DropDownStyle = ComboBoxStyle.DropDownList;
            cboPhieu.DataSource = phieuMuon.Select(pm =>
            {
                string tenDG = docGia.FirstOrDefault(d => d.MaDG == pm.MaDG)?.Ten ?? "Không rõ";
                string tenSach = sach.FirstOrDefault(s => s.MaSach == pm.MaSach)?.TenSach ?? "Không rõ";
                string text = pm.MaDG + " - " + tenDG + " | " + pm.MaSach + " - " + tenSach;
                return new SelectItem<PhieuMuon>(text, pm);
            }).ToList();
            AddRow(layout, 0, "Phiếu mượn", cboPhieu);

            SetupDatePicker(dtNgayTra);
            dtNgayTra.Value = DateTime.Today;
            AddRow(layout, 1, "Ngày trả", dtNgayTra);

            FillCombo(cboTinhTrang, LibraryConstants.TinhTrangSach);
            cboTinhTrang.SelectedIndex = 0;
            AddRow(layout, 2, "Tình trạng", cboTinhTrang);
            AddButtonRow(layout, 3);
        }
    }

    internal sealed class SelectItem<T>
    {
        public SelectItem(string text, T value)
        {
            Text = text;
            Value = value;
        }

        public string Text { get; }
        public T Value { get; }

        public override string ToString()
        {
            return Text;
        }
    }

    internal static class TextPrompt
    {
        public static string? Show(IWin32Window owner, string title, string label)
        {
            using (var form = new Form())
            using (var textBox = new TextBox())
            {
                form.Text = title;
                form.StartPosition = FormStartPosition.CenterParent;
                form.Size = new Size(360, 150);
                form.FormBorderStyle = FormBorderStyle.FixedDialog;
                form.MaximizeBox = false;
                form.MinimizeBox = false;
                form.BackColor = Color.FromArgb(241, 245, 249);

                var layout = CreateDialogLayout(2);
                form.Controls.Add(layout);
                AddRow(layout, 0, label, textBox);
                AddButtonRow(layout, 1);

                return form.ShowDialog(owner) == DialogResult.OK ? textBox.Text.Trim() : null;
            }
        }
    }

    internal static class ChoicePrompt
    {
        public static string? Show(IWin32Window owner, string title, string label, IEnumerable<string> values)
        {
            using (var form = new Form())
            using (var combo = new ComboBox())
            {
                form.Text = title;
                form.StartPosition = FormStartPosition.CenterParent;
                form.Size = new Size(380, 155);
                form.FormBorderStyle = FormBorderStyle.FixedDialog;
                form.MaximizeBox = false;
                form.MinimizeBox = false;
                form.BackColor = Color.FromArgb(241, 245, 249);

                var layout = CreateDialogLayout(2);
                form.Controls.Add(layout);
                FillCombo(combo, values);
                if (combo.Items.Count > 0)
                {
                    combo.SelectedIndex = 0;
                }
                AddRow(layout, 0, label, combo);
                AddButtonRow(layout, 1);

                return form.ShowDialog(owner) == DialogResult.OK ? combo.Text : null;
            }
        }
    }

    internal static class NumberPrompt
    {
        public static int? Show(IWin32Window owner, string title, string label, int min, int max)
        {
            using (var form = new Form())
            using (var number = new NumericUpDown())
            {
                form.Text = title;
                form.StartPosition = FormStartPosition.CenterParent;
                form.Size = new Size(420, 155);
                form.FormBorderStyle = FormBorderStyle.FixedDialog;
                form.MaximizeBox = false;
                form.MinimizeBox = false;
                form.BackColor = Color.FromArgb(241, 245, 249);

                number.Minimum = min;
                number.Maximum = max;
                number.Value = min;

                var layout = CreateDialogLayout(2);
                form.Controls.Add(layout);
                AddRow(layout, 0, label, number);
                AddButtonRow(layout, 1);

                return form.ShowDialog(owner) == DialogResult.OK ? (int)number.Value : null;
            }
        }
    }

    internal static class UiHelpers
    {
        public static TableLayoutPanel CreateDialogLayout(int rows)
        {
            var layout = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 2,
                RowCount = rows,
                Padding = new Padding(14),
                BackColor = Color.FromArgb(241, 245, 249)
            };
            layout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120F));
            layout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            for (int i = 0; i < rows - 1; i++)
            {
                layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 36F));
            }
            layout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            return layout;
        }

        public static void AddRow(TableLayoutPanel layout, int row, string label, Control control)
        {
            var lbl = new Label
            {
                Text = label,
                AutoSize = true,
                Anchor = AnchorStyles.Left,
                Padding = new Padding(0, 6, 0, 0),
                ForeColor = Color.FromArgb(15, 23, 42),
                Font = new Font("Segoe UI", 9.5F, FontStyle.Bold)
            };
            control.Dock = DockStyle.Fill;
            control.Font = new Font("Segoe UI", 10F);
            layout.Controls.Add(lbl, 0, row);
            layout.Controls.Add(control, 1, row);
        }

        public static void AddButtonRow(TableLayoutPanel layout, int row)
        {
            var panel = new FlowLayoutPanel
            {
                FlowDirection = FlowDirection.RightToLeft,
                Dock = DockStyle.Fill
            };

            var btnOk = new Button { Text = "Đồng ý", DialogResult = DialogResult.OK, Width = 100, Height = 34 };
            var btnCancel = new Button { Text = "Hủy", DialogResult = DialogResult.Cancel, Width = 90, Height = 34 };
            StyleDialogButton(btnOk, Color.FromArgb(37, 99, 235), Color.White);
            StyleDialogButton(btnCancel, Color.White, Color.FromArgb(15, 23, 42));
            panel.Controls.Add(btnOk);
            panel.Controls.Add(btnCancel);

            layout.Controls.Add(panel, 0, row);
            layout.SetColumnSpan(panel, 2);

            Form? form = layout.FindForm();
            if (form != null)
            {
                form.AcceptButton = btnOk;
                form.CancelButton = btnCancel;
            }
        }

        public static void FillCombo(ComboBox combo, IEnumerable<string> values)
        {
            combo.DropDownStyle = ComboBoxStyle.DropDownList;
            combo.Items.Clear();
            foreach (string value in values)
            {
                combo.Items.Add(value);
            }
        }

        public static void SetupDatePicker(DateTimePicker picker)
        {
            picker.Format = DateTimePickerFormat.Custom;
            picker.CustomFormat = "dd/MM/yyyy";
        }

        private static void StyleDialogButton(Button button, Color backColor, Color foreColor)
        {
            button.FlatStyle = FlatStyle.Flat;
            button.FlatAppearance.BorderSize = 1;
            button.FlatAppearance.BorderColor = Color.FromArgb(203, 213, 225);
            button.BackColor = backColor;
            button.ForeColor = foreColor;
            button.Font = new Font("Segoe UI", 9.5F, FontStyle.Bold);
            button.Cursor = Cursors.Hand;
        }
    }

    internal static class Program
    {
        [STAThread]
        private static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
