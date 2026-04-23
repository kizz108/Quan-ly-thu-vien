using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace UI_NongTrai
{
    public class Form1 : Form
    {
        private Button btnRun;
        private TextBox txtOutput;

        public Form1()
        {
            // Form
            this.Text = "Nông trại C++ + C#";
            this.Size = new Size(500, 400);
            this.StartPosition = FormStartPosition.CenterScreen;

            // Button
            btnRun = new Button();
            btnRun.Text = "Chạy chương trình C++";
            btnRun.Size = new Size(200, 40);
            btnRun.Location = new Point(150, 20);
            btnRun.Click += BtnRun_Click;
            this.Controls.Add(btnRun);

            // TextBox
            txtOutput = new TextBox();
            txtOutput.Multiline = true;
            txtOutput.ScrollBars = ScrollBars.Vertical;
            txtOutput.Size = new Size(400, 250);
            txtOutput.Location = new Point(50, 80);
            txtOutput.Font = new Font("Consolas", 10);
            this.Controls.Add(txtOutput);
        }

        private void BtnRun_Click(object sender, EventArgs e)
        {
            try
            {
                // Dùng 'using' để tự động giải phóng tài nguyên Process sau khi chạy xong
                using (Process p = new Process())
                {
                    // Biên dịch file C++ thành 1.exe ở đường dẫn này
                    p.StartInfo.FileName = @"main.exe";
                    p.StartInfo.UseShellExecute = false;
                    p.StartInfo.RedirectStandardOutput = true;
                    p.StartInfo.CreateNoWindow = true;
                    
                    // Cấu hình UTF8 đề phòng sau này C++ in ra Tiếng Việt có dấu
                    p.StartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;

                    p.Start();

                    string output = p.StandardOutput.ReadToEnd();
                    p.WaitForExit();

                    // Hiển thị đẹp hơn
                    txtOutput.Text = output.Replace("\n", Environment.NewLine);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Lỗi: " + ex.Message);
            }
        }
    }

    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.Run(new Form1());
        }
    }
}